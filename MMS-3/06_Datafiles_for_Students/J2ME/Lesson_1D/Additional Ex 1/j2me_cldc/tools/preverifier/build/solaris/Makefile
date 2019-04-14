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
	check_class.o \
	main.o \
	utf.o \
	check_code.o \
        convert_md.o \
	util.o \
	jar.o \
        jar_support.o \
	classloader.o \
	file.o \
	classresolver.o \
	stubs.o \
	inlinejsr.o \
	sys_support.o

CFLAGS = \
	-I$(SRC_DIR) \
	-DUNIX -DSOLARIS2 \
	-DJAVAVERIFY \
	-DTRIMMED \
	-D$(ARCH) $(EXTRACFLAGS)

ifeq ($(GCC), true)
   CC = gcc
   LD = gcc
   CFLAGS +=  -Wall $(CPPFLAGS) $(OTHER_FLAGS)
   DEBUG_FLAG =  
   LDFLAGS =
   
ifeq ($(DEBUG), true)
   DEBUG_FLAG = -g 
endif   

   OPTIMIZE_FLAG = -O2

else 

   CC = cc
   LD = cc
   CFLAGS += -Xa $(CPPFLAGS) $(OTHER_FLAGS)
   DEBUG_FLAG =
   LDFLAGS =

ifeq ($(DEBUG), true)
   DEBUG_FLAG = -g -xsb 
endif   

   OPTIMIZE_FLAG = -xO2
endif


all: preverify

preverify: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: $(SRC_DIR)/%.c
	$(CC) $(EXTRACFLAGS) $(CFLAGS) $(XWINFLAGS) $(OPTIMIZE_FLAG) $(DEBUG_FLAG) -c -o $@ $<

clean: 
	rm -rf core *.o preverify SunWS_cache .sb
