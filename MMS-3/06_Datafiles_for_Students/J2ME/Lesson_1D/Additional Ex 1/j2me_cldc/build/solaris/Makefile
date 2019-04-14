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

BUILDDIR=$(shell pwd)
TOP=../..
PLATFORM=solaris

UNAME=$(shell uname -a)
ifneq (,$(findstring SunOS,$(UNAME)))
  export PLATFORM=solaris
endif

SUBDIRS = \
  $(TOP)/tools/preverifier/build/solaris \
  $(TOP)/api \
  $(TOP)/samples \
  $(TOP)/samples/jam

ifeq ($(DEBUG), true) 
   SUBDIRS += $(TOP)/tools/kdp
endif
  
ifneq ($(ROMIZING), false) 
   SUBDIRS += $(TOP)/tools/jcc
endif

SUBDIRS += $(TOP)/kvm/VmUnix/build

all clean : FORCE
	@for i in $(SUBDIRS) ; do \
	    echo ">>>Recursively making "$$i" "$@"..."; \
	    cd $$i; $(MAKE) $@ \
	    || exit 1; cd $(BUILDDIR); \
	    echo "<<<Finished Recursively making "$$i" "$@"." ; \
	done

FORCE: 
