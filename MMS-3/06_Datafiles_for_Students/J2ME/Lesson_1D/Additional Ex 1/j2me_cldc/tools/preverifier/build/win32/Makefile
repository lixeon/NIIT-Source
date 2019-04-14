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

TOP=../../../..
include $(TOP)/build/Makefile.inc

SRC_DIR = ../../src

OBJS = \
	check_class.obj \
	main.obj \
	utf.obj \
	check_code.obj \
        convert_md.obj \
	util.obj \
        jar.obj \
        jar_support.obj \
	classloader.obj \
	file.obj \
	classresolver.obj \
	stubs.obj \
	inlinejsr.obj \
	sys_support.obj

OTHER_FLAGS += \
	-I$(SRC_DIR) \
	-DWIN32 \
	-DJAVAVERIFY \
	-DTRIMMED \
	-D$(ARCH) \
    $(EXTRACFLAGS)

ifeq ($(GCC), true)
   CC = gcc
   LD = gcc
   CFLAGS = -DWIN32 -DGCC -mno-cygwin -mcpu=i386 -Wall $(OTHER_FLAGS)
   LIBS= -luser32 -lkernel32 -lmsvcrt -lwsock32
   CPPFLAGS += 
   DEBUG_FLAG =
   LDFLAGS = -mno-cygwin

ifeq ($(DEBUG), true)
   DEBUG_FLAG = -g 
endif

   OPTIMIZE_FLAG = -O2
   OUTPUT = -o 
   LINKER_OUTPUT = -o 

else
   CC= cl
   LD= link
   CFLAGS= -DWIN32 -nologo $(OTHER_FLAGS)
   LIBS= user32.lib kernel32.lib wsock32.lib
   CPPFLAGS += 
   DEBUG_FLAG =
   LDFLAGS = 

ifeq ($(DEBUG), true)
   DEBUG_FLAG= -Zi -Od 
   LDFLAGS += -debug
endif

   OPTIMIZE_FLAG= -Os -O1
   OUTPUT = -Fo
   LINKER_OUTPUT = -out:
endif


all: preverify.exe

preverify.exe: $(OBJS)
	@$(LD) $(LDFLAGS) $(OBJS) $(LINKER_OUTPUT)$@ $(LIBS)

%.obj: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) $(DEBUG_FLAG) -c $(OUTPUT)$@ $<

clean: 
	rm -f core *.obj preverify.exe *.ilk *.pdb
