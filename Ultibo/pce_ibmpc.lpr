program pce_ibmpc;

{$mode objfpc}{$H+}

{$IFNDEF PLATFORM_PI3}
  {$IFNDEF PLATFORM_PI2}
    {$DEFINE PLATFORM_QEMU}
  {$ENDIF}
{$ENDIF}

//{$DEFINE ENABLE_DIRECT_DRAW}

uses
{$IFDEF PLATFORM_PI2}
  RaspberryPi2,
{$ENDIF}
{$IFDEF PLATFORM_PI3}
  RaspberryPi3,
{$ENDIF}
{$IFDEF PLATFORM_QEMU}
  QEMUVersatilePB,
{$ENDIF}
  GlobalConfig,
  GlobalConst,
  GlobalTypes,
  Platform,
  Threads,
  SysUtils,
  Classes,
  Ultibo,
  Syscalls,
  Console,
  FileSystem,
  FATFS,
  MMC,
  Framebuffer,
  keyboard,
  Mouse,
  scancode_to_pce;

{$linklib dos}
{$linklib ibmpc}
{$linklib m}

var
  FramebufferDevice     : PFramebufferDevice;
  FramebufferProperties : TFramebufferProperties;
  BufferStart           : Pointer;
  PageSize              : Integer;
  CurrentPage           : Integer;

  previousMouseX : integer;
  previousMouseY : integer;

  GfxWidth, GfxHeight   : Integer;

{$IFDEF ENABLE_DIRECT_DRAW}
  const C_BPP = 24;
{$ELSE}
  const C_BPP = 32;
{$ENDIF}

{$IFDEF PLATFORM_QEMU}
const RUNDELAY = 100;
{$ELSE}
const RUNDELAY = 200;
{$ENDIF}

//video = cga, vga, etc.
//speed = 1, 2, ..n
function IBMPC_LOOP(cfg : pchar; boot : pchar; cpu : pchar; speed : pchar; video : pchar) : integer; cdecl; external 'libibmpc' name 'IBMPC_LOOP';

///////////////////////////////

procedure Pal_getMousePosition(var x : integer; var y : integer); export; cdecl;
var
  data : TMouseData;
  count : LongWord;
begin

  if (MousePeek() = ERROR_SUCCESS) then
  begin
    MouseRead(@data, sizeof(data), count);
    x := data.OffsetX;
    y := data.OffsetY;
  end;
end;

function Pal_getScreenPitch() : integer; export; cdecl; //USED
begin
  Result := FramebufferProperties.Pitch;
end;

function Pal_getScreenBuffer() : pointer; export; cdecl; //USED
var
  left : integer;
begin
{$IFDEF ENABLE_DIRECT_DRAW}
  left := 0;
  Result := BufferStart + (CurrentPage * PageSize) + left;
{$ELSE}
  Result := nil;
{$ENDIF}
end;


var
  line : array[0..2048] of integer;

procedure Pal_drawBuffer(buffer : pointer; width, height, pitch : integer); export; cdecl; //used
var
  x, y : integer;
  src : pbyte;
  dst : pint;
  index : integer;
  offset : integer;
begin

  if (GfxWidth <> width) OR (GfxHeight <> height) then
  begin
    fillchar(BufferStart, PageSize, 0);
    GfxWidth := width;
    GfxHeight := height;
  end;

  CurrentPage := (CurrentPage + 1) mod 2;

  // if buffer is NIL, then it drew directly to the framebuffer and we can't do anything other than flip the screen
  // if buffer is non-NIL, then we're getting the buffer to draw ourselves, should we wish to scale it, etc.

  if (buffer <> nil) then
  begin
    // try scale x3
    if (height * 3 < FramebufferProperties.PhysicalHeight) and (width * 3 < FramebufferProperties.PhysicalWidth) then
    begin
      offset := (FramebufferProperties.PhysicalWidth - width*3) * 2;
      offset := offset + (FramebufferProperties.Pitch * ((FramebufferProperties.PhysicalHeight - height*3) DIV 2));
      dst := pint(BufferStart + (CurrentPage * PageSize) + offset);
      for y := 0 to height-1 do
      begin
        src := pbyte(buffer + pitch * y);
        index := 0;
        for x := 0 to width-1 do
        begin
          line[index] := ((src + 0)^) + ((src + 1)^ SHL 8) + ((src + 2)^ SHL 16) + (255 SHL 24);
          line[index + 1] := line[index];
          line[index + 2] := line[index];
          index := index + 3;
          src := src + 3;
        end;
        CopyMemory(dst, @line, width * 4 * 3);
        dst := dst + FramebufferProperties.Pitch SHR 2;
        CopyMemory(dst, @line, width * 4 * 3);
        dst := dst + FramebufferProperties.Pitch SHR 2;
        CopyMemory(dst, @line, width * 4 * 3);
        dst := dst + FramebufferProperties.Pitch SHR 2;
      end;
    end else
    // try scale x2
    if (height * 2 < FramebufferProperties.PhysicalHeight) and (width * 2 < FramebufferProperties.PhysicalWidth) then
    begin
      offset := (FramebufferProperties.PhysicalWidth - width*2) * 2;
      offset := offset + (FramebufferProperties.Pitch * ((FramebufferProperties.PhysicalHeight - height*2) DIV 2));
      dst := pint(BufferStart + (CurrentPage * PageSize) + offset);
      for y := 0 to height-1 do
      begin
        src := pbyte(buffer + pitch * y);
        index := 0;
        for x := 0 to width-1 do
        begin
          line[index] := ((src + 0)^) + ((src + 1)^ SHL 8) + ((src + 2)^ SHL 16) + (255 SHL 24);
          line[index + 1] := line[index];
          index := index + 2;
          src := src + 3;
        end;
        CopyMemory(dst, @line, width * 4 * 2);
        dst := dst + FramebufferProperties.Pitch SHR 2;
        CopyMemory(dst, @line, width * 4 * 2);
        dst := dst + FramebufferProperties.Pitch SHR 2;
      end;
    end else
    begin
      for y := 0 to height-1 do
      begin
        src := pbyte(buffer + pitch * y);
        offset := (FramebufferProperties.PhysicalWidth - width) * 2;
        offset := offset + (FramebufferProperties.Pitch * ((FramebufferProperties.PhysicalHeight - height) DIV 2));
        dst := pint(BufferStart + (CurrentPage * PageSize) + FramebufferProperties.Pitch * y + offset);
        for x := 0 to width-1 do
        begin
          dst^ := ((src + 0)^) + ((src + 1)^ SHL 8) + ((src + 2)^ SHL 16) + (255 SHL 24);
          dst := dst + 1;
          src := src + 3;
        end;
      end;
    end;
  end;

  // flip the screen

  if (FramebufferProperties.Flags and FRAMEBUFFER_FLAG_CACHED) <> 0 then
  begin
    CleanDataCacheRange(PtrUInt(BufferStart) + (CurrentPage * PageSize), PageSize);
  end;

  FramebufferDeviceSetOffset(FramebufferDevice, 0, CurrentPage * FramebufferProperties.PhysicalHeight, True);

  if (FramebufferProperties.Flags and FRAMEBUFFER_FLAG_SYNC) <> 0 then
  begin
    FramebufferDeviceWaitSync(FramebufferDevice); // eliminating this call gets rid of stuttering, but... surely we should honour it?
  end;

end;

function Pal_getTicks() : ULONG; export; cdecl;
begin
  Result := GetTickCount();
end;

function Pal_init() : integer; export; cdecl;
begin
  Result := 1;
end;

procedure Pal_createFrameBuffer(width, height, bpp : integer); export; cdecl;
begin
  GfxWidth := width;
  GfxHeight := height;
  fillchar(BufferStart, PageSize, 0);
end;

procedure Pal_getKey(var value : integer; var down : integer); export; cdecl;
var
 Count:LongWord;
 Data:TKeyboardData;
begin
  down := 0;
  Count := 1;

  if KeyboardReadEx(@Data, SizeOf(TKeyboardData), KEYBOARD_FLAG_NONE, Count) = ERROR_SUCCESS then
  begin
    if (Data.KeyCode = KEY_CODE_F12) then SystemRestart(0);

    value := ScancodeToPCE(Data.ScanCode);
    if (Data.Modifiers AND KEYBOARD_KEYUP) = 0 then
      down := 1;
  end;
end;

function Pal_peekKey() : integer; export; cdecl;
begin
  Result := 0;

  if (KeyboardPeek() = ERROR_SUCCESS) then
    Result := 1;
end;

function Pal_peekMouse() : integer; export; cdecl;
var
  data : TMouseData;
  count : LongWord;
begin
  Result := 0;
  if (MousePeek() = ERROR_SUCCESS) then
    Result := 1;
end;

procedure Pal_getMouse(var x : integer; var y : integer; var down : integer); export; cdecl;
var
  data : TMouseData;
  count : LongWord;
begin
  MouseRead(@data, sizeof(data), count);

  if (data.Buttons AND MOUSE_ABSOLUTE_X) <> 0 then
  begin
    if (previousMouseX = -1) then
      x := 0
    else
      x := data.OffsetX - previousMouseX;

    previousMouseX := data.OffsetX;
  end else
  begin
    x := data.OffsetX;
  end;

  if (data.Buttons AND MOUSE_ABSOLUTE_Y) <> 0 then
  begin
    if (previousMouseY = -1) then
      y := 0
    else
      y := data.OffsetY - previousMouseY;

    previousMouseY := data.OffsetY;
  end else
  begin
    y := data.OffsetY;
  end;

  if (data.Buttons AND MOUSE_LEFT_BUTTON) <> 0 then
    down := 1
  else
    down := 0;
end;

procedure Pal_setScreenSize(width : integer; height : integer); export; cdecl; // limits drawing within fullscreen
begin
  GfxWidth := width;
  GfxHeight := height;
end;

///////////////////////////////

procedure showLog;
var
  fs : TFileStream;
  sl : TStringList;
  count : integer;
  WindowHandle : TWindowHandle; //debug
begin
  FramebufferDeviceRelease(FramebufferDevice);
  FramebufferDevice := FramebufferDeviceGetDefault;
  FramebufferDeviceGetProperties(FramebufferDevice, @FramebufferProperties);
  FramebufferDeviceRelease(FramebufferDevice);
  Sleep(RUNDELAY);
  FramebufferProperties.Depth := C_BPP;
  FramebufferProperties.VirtualWidth:= FramebufferProperties.PhysicalWidth;
  FramebufferProperties.VirtualHeight := FramebufferProperties.PhysicalHeight*2;

  FRAMEBUFFER_CONSOLE_AUTOCREATE := True;
  FramebufferDeviceAllocate(FramebufferDevice, @FramebufferProperties);
  Sleep(RUNDELAY);

  WindowHandle := ConsoleWindowCreate(ConsoleDeviceGetDefault, CONSOLE_POSITION_FULLSCREEN, True);

  ConsoleWindowWriteLn(WindowHandle, 'Showing error log:');
  ConsoleWindowWriteLn(WindowHandle, '');

  sl:=TStringList.Create;
  try
    fs := TFileStream.Create('c:\pce_ibmpc.log', fmOpenRead);

    {And use LoadFromStream to read it}
    sl.LoadFromStream(fs);

    {Iterate the strings and print them to the screen}
    for Count:=0 to sl.Count - 1 do
    begin
      ConsoleWindowWriteLn(WindowHandle,sl.Strings[Count]);
    end;
    fs.Free;
  finally
    sl.Free;
  end;

  readln;
end;

begin
  ThreadSetCPU(ThreadGetCurrent, CPU_ID_3);

  Sleep(RUNDELAY);

// This breaks framebuffer ininitialisation into two parts because it's suggested that
// you sleep between allocating the buffer and getting its properties, but seeing as
// we need to wait for the disk to be available, we can effectively cut out a wait
// and use one alone the way.
FramebufferDevice := FramebufferDeviceGetDefault;
FramebufferDeviceGetProperties(FramebufferDevice, @FramebufferProperties);
FramebufferDeviceRelease(FramebufferDevice);
Sleep(RUNDELAY);
FramebufferProperties.Depth := C_BPP;
FramebufferProperties.VirtualWidth:= FramebufferProperties.PhysicalWidth;
FramebufferProperties.VirtualHeight := FramebufferProperties.PhysicalHeight*2;

FRAMEBUFFER_CONSOLE_AUTOCREATE := False;
FramebufferDeviceAllocate(FramebufferDevice, @FramebufferProperties);

  while not DirectoryExists('C:\') do
  begin
    Sleep(100);
  end;

FramebufferDeviceGetProperties(FramebufferDevice, @FramebufferProperties);
BufferStart := Pointer(FramebufferProperties.Address);
PageSize := FramebufferProperties.Pitch * FramebufferProperties.PhysicalHeight;
CurrentPage := 0;

  previousMouseX := -1;
  previousMouseY := -1;
  try
    DeleteFile('c:\pce_ibmpc.log');
    IBMPC_LOOP('c:\ibmpc.cfg', nil, nil, nil, 'vga');
    //IBMPC_LOOP('c:\ibmpc_8086.cfg', nil, nil, nil, 'vga');
    //IBMPC_LOOP('c:\ibmpc_8086_dynamic.cfg', nil, nil, nil, 'vga');
    //IBMPC_LOOP('c:\ibmpc_186.cfg', nil, nil, nil, 'vga');
    //IBMPC_LOOP('c:\ibmpc_286.cfg', nil, nil, nil, 'vga');
    showLog(); // will only be called in event of error
  except
    on E:exception do
    begin
      ConsoleWriteLn('Exception thrown: ' + E.Message);
    end;
  end;
end.

