# Makefile.inc

etcdir      = ${prefix}/etc/pce
datarootdir = ${prefix}/share
datadir     = ${datarootdir}/pce

CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
NASM    = 
IHEX    = 
LN_S    = ln -s
INSTALL = /usr/bin/install -c

CFLAGS         = -O2 -mabi=aapcs -marm -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=hard -fsigned-char -std=c99 -D__DYNAMIC_REENT__
CFLAGS_DEFAULT = $(CFLAGS) -Isrc

LDFLAGS         = 
LDFLAGS_DEFAULT = $(LDFLAGS)

EXEEXT := 

LIBS := 

ifneq "$(srcdir)" "."
CFLAGS_DEFAULT += -I$(srcdir)/src
endif

PCE_VERSION_MAJ := 0
PCE_VERSION_MIN := 2
PCE_VERSION_MIC := 2
PCE_VERSION_STR := 20180722-3f91fa12-mod

PCE_HAVE_NASM := 0
PCE_HAVE_IHEX := 0

PCE_ENABLE_X11 := 0
PCE_X11_CFLAGS := 
PCE_X11_LIBS   := 

PCE_ENABLE_SDL  := 1
PCE_ENABLE_SDL1 := 0
PCE_ENABLE_SDL2 := 1
PCE_SDL_CFLAGS  := 
PCE_SDL_LIBS    :=

PCE_HAVE_STDINT_H   := 1
PCE_HAVE_INTTYPES_H := 1

PCE_LARGE_FILE := 1

PCE_HOST_LINUX   := 1
PCE_HOST_WINDOWS := 0
PCE_HOST_SUNOS   := 0
PCE_HOST_NETBSD  := 0

PCE_HOST_IA32  := 1
PCE_HOST_PPC   := 0
PCE_HOST_SPARC := 0

ifneq "$(PCE_HAVE_STDINT_H)" "1"
CFLAGS_DEFAULT += -Isrc/lib/stdint
ifneq "$(srcdir)" "."
CFLAGS_DEFAULT += -I$(srcdir)/src/lib/stdint
endif
endif

ifeq "$(PCE_HOST_IA32)" "1"
CFLAGS_DEFAULT += -DPCE_HOST_IA32
endif

ifeq "$(PCE_HOST_PPC)" "1"
CFLAGS_DEFAULT += -DPCE_HOST_PPC
endif

ifeq "$(PCE_HOST_SPARC)" "1"
CFLAGS_DEFAULT += -DPCE_HOST_SPARC
endif

PCE_BUILD_ATARIST := 0
PCE_BUILD_CPM80   := 0
PCE_BUILD_IBMPC   := 1
PCE_BUILD_MACPLUS := 1
PCE_BUILD_RC759   := 0
PCE_BUILD_SIM405  := 0
PCE_BUILD_SIM6502 := 1
PCE_BUILD_SIMARM  := 0
PCE_BUILD_SIMS32  := 0

PCE_BUILD_IBMPC_ROM   := 0
PCE_BUILD_MACPLUS_ROM := 0

PCE_ENABLE_READLINE := 0
PCE_READLINE_LIBS   := 

PCE_ENABLE_TUN := 0

PCE_ENABLE_CHAR_POSIX  := 0
PCE_ENABLE_CHAR_PPP    := 0
PCE_ENABLE_CHAR_PTY    := 0
PCE_ENABLE_CHAR_SLIP   := 0
PCE_ENABLE_CHAR_TCP    := 0
PCE_ENABLE_CHAR_TIOS   := 0
PCE_ENABLE_CHAR_WINCOM := 0

PCE_ENABLE_SOUND_OSS := 0

distdir     := pce-$(PCE_VERSION_STR)

# ----------------------------------------------------------------------

DCL += Makefile Makefile.inc configure config.log config.status

DIST += AUTHORS COPYING ChangeLog INSTALL \
	Makefile.dep Makefile.in Makefile.inc.in \
	NEWS README TODO autogen.sh config.guess config.sub \
	configure configure.in gendep install-sh

DIST += doc/char-drivers.txt doc/keys.txt doc/messages.txt \
	doc/monitor.txt doc/pce-ibmpc.txt doc/pce-macplus.txt \
	doc/pfi-format.txt doc/pri-format.txt doc/psi-format.txt \
	doc/rc759-keymap.txt doc/sound-drivers.txt

# ----------------------------------------------------------------------

include $(srcdir)/Makefile.dep

include $(srcdir)/src/Makefile.inc
include $(srcdir)/src/cpu/Makefile.inc
include $(srcdir)/src/cpu/arm/Makefile.inc
include $(srcdir)/src/cpu/e6502/Makefile.inc
include $(srcdir)/src/cpu/e68000/Makefile.inc
include $(srcdir)/src/cpu/e8080/Makefile.inc
include $(srcdir)/src/cpu/e8086/Makefile.inc
include $(srcdir)/src/cpu/ppc405/Makefile.inc
include $(srcdir)/src/cpu/sparc32/Makefile.inc
include $(srcdir)/src/chipset/80186/Makefile.inc
include $(srcdir)/src/chipset/82xx/Makefile.inc
include $(srcdir)/src/chipset/clock/Makefile.inc
include $(srcdir)/src/chipset/ppc405/Makefile.inc
include $(srcdir)/src/chipset/Makefile.inc
include $(srcdir)/src/devices/clock/Makefile.inc
include $(srcdir)/src/devices/video/Makefile.inc
include $(srcdir)/src/devices/Makefile.inc
include $(srcdir)/src/libini/Makefile.inc
include $(srcdir)/src/lib/Makefile.inc
include $(srcdir)/src/drivers/Makefile.inc
include $(srcdir)/src/drivers/block/Makefile.inc
include $(srcdir)/src/drivers/char/Makefile.inc
include $(srcdir)/src/drivers/pfi/Makefile.inc
include $(srcdir)/src/drivers/pri/Makefile.inc
include $(srcdir)/src/drivers/psi/Makefile.inc
include $(srcdir)/src/drivers/sound/Makefile.inc
include $(srcdir)/src/drivers/video/Makefile.inc
include $(srcdir)/src/arch/Makefile.inc
include $(srcdir)/src/arch/atarist/Makefile.inc
include $(srcdir)/src/arch/cpm80/Makefile.inc
include $(srcdir)/src/arch/dos/Makefile.inc
include $(srcdir)/src/arch/ibmpc/Makefile.inc
include $(srcdir)/src/arch/ibmpc/bios/Makefile.inc
include $(srcdir)/src/arch/ibmpc/pceutils/Makefile.inc
include $(srcdir)/src/arch/macplus/Makefile.inc
include $(srcdir)/src/arch/macplus/pcex/Makefile.inc
include $(srcdir)/src/arch/rc759/Makefile.inc
include $(srcdir)/src/arch/sim405/Makefile.inc
include $(srcdir)/src/arch/sim6502/Makefile.inc
include $(srcdir)/src/arch/simarm/Makefile.inc
include $(srcdir)/src/arch/sims32/Makefile.inc
include $(srcdir)/src/utils/Makefile.inc
include $(srcdir)/src/utils/aym/Makefile.inc
include $(srcdir)/src/utils/pce-img/Makefile.inc
include $(srcdir)/src/utils/pfi/Makefile.inc
include $(srcdir)/src/utils/pri/Makefile.inc
include $(srcdir)/src/utils/psi/Makefile.inc

TARGETS += $(BIN) $(ETC) $(SHARE)

# ----------------------------------------------------------------------

%: %.sh
	$(QP)echo "  SED    $@"
	$(QR)rm -f $@
	$(QR)sed -e "s/%PCE_VERSION_STR/$(PCE_VERSION_STR)/g" < $< > $@
	$(QR)chmod a+x $@

install-extra:
ifneq "$(SHARE_IBMPC)" ""
	$(QP)echo "  MKDIR  $(DESTDIR)$(datadir)/ibmpc"
	$(QR)$(INSTALL) -d -m 755 $(DESTDIR)$(datadir)/ibmpc
	$(QR)for f in $(SHARE_IBMPC) ; do \
		dst=$(DESTDIR)$(datadir)/ibmpc/`basename "$$f"` ; \
		if test x$(V) != x1 ; then echo "  CP     $$dst" ; fi ; \
		$(INSTALL) -m 644 "$$f" "$$dst" ; \
	done
endif
ifneq "$(SHARE_MACPLUS)" ""
	$(QP)echo "  MKDIR  $(DESTDIR)$(datadir)/macplus"
	$(QR)$(INSTALL) -d -m 755 $(DESTDIR)$(datadir)/macplus
	$(QR)for f in $(SHARE_MACPLUS) ; do \
		dst=$(DESTDIR)$(datadir)/macplus/`basename "$$f"` ; \
		if test x$(V) != x1 ; then echo "  CP     $$dst" ; fi ; \
		$(INSTALL) -m 644 "$$f" "$$dst" ; \
	done
endif
ifneq "$(SHARE_PCEUTILS)" ""
	$(QR)$(INSTALL) -d -m 755 $(DESTDIR)$(datadir)/ibmpc/pceutils
	$(QR)for f in $(SHARE_PCEUTILS) ; do \
		dst=$(DESTDIR)$(datadir)/ibmpc/pceutils/`basename "$$f"` ; \
		test x$(V) != x1 && echo "  CP     $$dst" ; \
		$(INSTALL) -m 644 "$$f" "$$dst" ; \
	done
endif


dist-extra:
ifneq "$(SHARE_PCEUTILS)" ""
	$(QR)for f in $(PCEUTILS_BIN) ; do \
		test -f "$$f" || continue ; \
		dst=contrib/pceutils/`basename "$$f"` ; \
		test x$(V) != x1 && echo "  CP     $$dst" ; \
		mkdir -p "$(distdir)/contrib/pceutils" ; \
		cp -p "$$f" "$(distdir)/$$dst" ; \
	done
endif


dist-version:
	$(QP)echo "  GEN    version"
	$(QR)echo "pce  $(PCE_VERSION_MAJ) $(PCE_VERSION_MIN) $(PCE_VERSION_MIC)  $(PCE_VERSION_STR)" \
		> "$(distdir)/version"

release:
	$(MAKE) PCE_VERSION_STR=$(PCE_VERSION_MAJ).$(PCE_VERSION_MIN).$(PCE_VERSION_MIC) dist
