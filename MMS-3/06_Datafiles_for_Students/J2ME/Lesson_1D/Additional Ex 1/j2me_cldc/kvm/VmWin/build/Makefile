#
# Copyright 1995-2002 by Sun Microsystems, Inc.,
# 901 San Antonio Road, Palo Alto, California, 94303, U.S.A.
# All rights reserved.
#
# This software is the confidential and proprietary information
# of Sun Microsystems, Inc. ("Confidential Information").  You
# shall not disclose such Confidential Information and shall use
# it only in accordance with the terms of the license agreement
# you entered into with Sun.
# Use is subject to license terms.
#

#
# Makefile for GNU make, adapted from the solaris version
# @(#)GNUmakefile	12:23:08 09/27/02 @(#)Makefile	1.32
#

TOP=../../..
include $(TOP)/build/Makefile.inc

ifeq ($(DEBUG), true)
   g = _g
else
   g =
endif

j =

ifeq ($(DEBUG_COLLECTOR), true)
COLLECTOR = collectorDebug.c
else
COLLECTOR = collector.c
endif

ifeq ($(ASM_LOOP), true)
OTHER_FLAGS += -DALTERNATIVE_FAST_INTERPRETER=1
endif

SRCFILES  = cache.c class.c fields.c frame.c garbage.c $(COLLECTOR)   \
            global.c interpret.c execute.c loader.c main.c native.c   \
            property.c thread.c nativeCore.c loaderFile.c hashtable.c \
            verifier.c log.c jar.c inflate.c  stackmap.c profiling.c  \
	    pool.c events.c resource.c StartJVM.c                     \
	    nativeFunctionTableWin.c runtime_md.c runtime2_md.c       \
	    datagramProtocol.c datagramProtocol_md.c                  \
	    socketProtocol.c socketProtocol_md.c                      \
	    commProtocol.c commProtocol_md.c async.c verifierUtil.c


ifeq ($(DEBUG), true)
   SRCFILES += debugger.c debuggerSocketIO.c debuggerOutputStream.c debuggerInputStream.c
   OTHER_FLAGS += -DENABLE_JAVA_DEBUGGER=1
else
   OTHER_FLAGS += -DENABLE_JAVA_DEBUGGER=0
endif

OTHER_FLAGS +=-DPADTABLE=1

ifeq ($(USE_JAM), true)
   OTHER_FLAGS += -DUSE_JAM=1
   SRCFILES += jam.c jamParse.c jamHttp.c jamStorage.c
   j = _j
else

endif

ifeq ($(USE_KNI), false)
  OTHER_FLAGS += -DUSE_KNI=0
else
  OTHER_FLAGS += -DUSE_KNI=1
  SRCFILES += kni.c
endif

ifeq ($(ROMIZING), false) 
   ROMFLAGS = -DROMIZING=0
else
   SRCFILES += ROMjavaWin.c
endif

ifeq ($(EXCESSIVEGC), true)
   OTHER_FLAGS += -DEXCESSIVE_GARBAGE_COLLECTION=1
else
   OTHER_FLAGS += -DEXCESSIVE_GARBAGE_COLLECTION=0
endif

all: kvm$(j)$(g).exe


OBJFILES = $(patsubst %.c,obj$(j)$(g)/%.o,$(SRCFILES))

vpath %.c  $(TOP)/kvm/VmCommon/src/ $(TOP)/kvm/VmWin/src/ \
	   $(TOP)/kvm/VmExtra/src/ $(TOP)/tools/jcc/ \
	   $(TOP)/jam/src/

ifeq ($(GCC), true)
   CC = gcc
   LD = gcc
   LIBS= -luser32 -lkernel32 -lmsvcrt -lwsock32
   CPPFLAGS = -DWIN32 -DGCC -I$(TOP)/kvm/VmCommon/h -I$(TOP)/kvm/VmWin/h \
	      -I$(TOP)/kvm/VmExtra/h -I$(TOP)/jam/h -I$(TOP)/kvm/VmCommon/src
   CFLAGS = -mno-cygwin -mcpu=i386 -Wall $(CPPFLAGS) $(ROMFLAGS) $(OTHER_FLAGS)
   DEBUG_FLAG = -g
   OPTIMIZE_FLAG = -O2
   OUTPUT = -o 
   LINKER_OUTPUT = -o 
   LDFLAGS = -mno-cygwin
else
   CC= cl
   LD= link
   LIBS= user32.lib kernel32.lib wsock32.lib
   CPPFLAGS = -DWIN32 -I$(TOP)/kvm/VmCommon/h -I$(TOP)/kvm/VmWin/h \
	      -I$(TOP)/kvm/VmExtra/h -I$(TOP)/jam/h /MT
   CFLAGS= -nologo $(CPPFLAGS) $(ROMFLAGS) $(OTHER_FLAGS)
   DEBUG_FLAG= -Zi -Od
   OPTIMIZE_FLAG= -Ox
   OUTPUT = -Fo
   LINKER_OUTPUT = -out:
   LDFLAGS = 
ifeq ($(DEBUG), true)
   LDFLAGS += -debug
endif

endif


DEBUG_FLAG += -DINCLUDEDEBUGCODE=1 

$(TOP)/tools/jcc/ROMjavaWin.c $(TOP)/tools/jcc/nativeFunctionTableWin.c: jccWin

.PHONY:  jccWin
jccWin: 
	@(cd $(TOP)/tools/jcc; $(MAKE) windows)


kvm$(j)$(g).exe: obj$j$g/  $(OBJFILES)
	@echo "Linking ... $@"
	@$(LD) $(LDFLAGS) $(OBJFILES) $(LINKER_OUTPUT)$@ $(LIBS)

clean: 
	rm -rf kvm* obj*
	
$(OBJFILES): $(TOP)/kvm/VmWin/h/machine_md.h

obj$(j)$g/execute.o : execute.c bytecodes.c 

obj$(j)/%.o: %.c
		@echo "... $@"
		@$(CC) $(CFLAGS) $(OPTIMIZE_FLAG) -c $(OUTPUT)$@ $<

obj$(j)_g/%.o: %.c
		@echo "... $@"
		@$(CC) $(CFLAGS) $(DEBUG_FLAG) -c $(OUTPUT)$@ $<

obj$j$g/:
		mkdir -p $@

ifeq ($(GCC), true)

%.i: %.c .FORCE
	echo $(CC)
	@$(CC) $(CFLAGS) $(OPTIMIZE_FLAG) -E $(OUTPUT)$@ $<

else 

%.i: %.c .FORCE
	echo $(CC)
	@$(CC) $(CFLAGS) $(OPTIMIZE_FLAG) -E $< > $@

endif

%.s: %.c .FORCE
	@echo "... $@"
	@$(CC) $(CFLAGS) $(OPTIMIZE_FLAG) -S $(OUTPUT)$@ $<

.FORCE:
