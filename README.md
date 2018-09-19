[![Baremetal PCE](https://img.youtube.com/vi/x4OXQm42dyo/0.jpg)](https://www.youtube.com/watch?v=x4OXQm42dyo "Bare metal PCE")

# Bare metal PC emulator for Raspberry Pi

To quote the PCE [homepage](http://www.hampa.ch/pce/about.html): 
PCE is a collection of microcomputer emulators. At the moment it contains three emulators: 
* PCE/atarist is an Atari ST emulator. It emulates the various ST and Mega ST models.
* PCE/ibmpc is an IBM PC 5150 emulator. It emulates an IBM PC 5150 or an IBM PC/XT 5160.
* PCE/macplus is a classic Macintosh emulator. It emulates a Macintosh 128K, Macintosh 512k, Macintosh 512ke, Macintosh Plus, Macintosh SE or a Macintosh Classic.
* PCE/rc759 is a Regnecentralen RC759 Piccoline emulator.

And off-by-one errors.

The SDL codepath was hastily ported to the [Ultibo](https://ultibo.org/) bare metal environment for Raspberry Pi.
This allows for quick booting, at the expense of other luxuries.

This is by no means a complete port, suffering from several severe issues such as -but not limited to- flickering, stuttering, slowdown, etc.

This git repo was mirrored from the official PCE git repo on 19 September, 2018.

## Running

* Format an SD card to be FAT32
* Copy the Raspberry Pi [firmware files](https://github.com/raspberrypi/firmware/tree/master/boot) onto it
* Download and extract the zip file from this release page onto the SD card, overwriting `kernel7.img`
* Put your PCE config file in the root of the SD card specifically named as `pce_ibmpc.cfg`
* Put your hard disk image in the root of the SD card

No work has been done to make it easy to setup your PC installation on the Raspberry Pi itself.
You will find it infinitely easier to set everything up on your desktop, then simply copy the hard disk image over to the SD card.

## Compiling

To build the library
* Install the arm-none-eabi toolchain from [here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
* Run `make` as usual
* This should result in several .a lib files, the ones you'll need are `libdos.a` and `libibmpc.a`

To build the bare metal kernel
* Copy the .a files to the Ultibo project folder
* Compile the project from inside Ultibo Lazarus, or by using one of the batch files
*Note*: you will need to edit the path to your Ultibo installation in the batch files.

---

PCE - PC Emulator
==============================================================================

PCE is a collection of microcomputer emulators.

For further information check out these files:

doc/pce-ibmpc.txt
	PCE/ibmpc, an IBM PC 5150 emulator

doc/pce-macplus.txt
	PCE/macplus, a Macintosh Plus emulator

doc/keys.txt
	Key combinations to control the emulator

doc/monitor.txt
	The monitor interface

doc/messages.txt
	Messages to control the emulator


Install
==============================================================================

To install do:

./configure
make
make install

The most important options for configure are:

--prefix=path
Set the installation prefix. The default is /usr/local.

--with-x11[=path]
Build the X11 based terminal.

--with-sdl
Build the SDL based terminal.


have fun
Hampa Hug <hampa@hampa.ch>
