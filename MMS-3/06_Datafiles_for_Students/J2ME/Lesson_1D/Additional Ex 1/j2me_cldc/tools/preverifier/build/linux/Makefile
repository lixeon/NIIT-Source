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
	-DUNIX -DLINUX \
	-DJAVAVERIFY \
	-DTRIMMED \
	-D$(ARCH) $(EXTRACFLAGS)

CC = gcc
LD = gcc

DEBUG_FLAG =  
LDFLAGS =

ifeq ($(DEBUG), true)
   DEBUG_FLAG = -g 
endif

all: preverify

preverify: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $(DEBUG_FLAG) $<

clean: 
	rm -f core *.o preverify
