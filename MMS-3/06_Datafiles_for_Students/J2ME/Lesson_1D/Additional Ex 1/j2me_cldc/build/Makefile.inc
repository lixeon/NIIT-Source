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

# The PLATFORM defined in build/platform-name

UNAME=$(shell uname -a)

ARCH=i386
ifneq (,$(findstring sparc,$(UNAME)))
  export ARCH=sparc
endif

OTHER_FLAGS += -DBUILD_VERSION='"$(VERSION)"'
