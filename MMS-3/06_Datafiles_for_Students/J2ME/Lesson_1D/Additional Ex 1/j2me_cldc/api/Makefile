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

TOP=..
include $(TOP)/build/Makefile.inc

ifeq ($(DEBUG),true)
DEBUGFLAG=
else
DEBUGFLAG=":none"
endif

JAVAC     = javac 
PREVERIFY = ../tools/preverifier/build/$(PLATFORM)/preverify
JAR = jar

all: tools

JAVAFILES =  $(shell find src -name "*.java"|grep -v SCCS|grep -v j2se)
 
CLASSFILES = $(subst src,classes,$(JAVAFILES:java=class))

# $< is dependency
# $@ is target
#
$(CLASSFILES): classes/%.class : src/%.java
	@echo $< >> .filelist

eraselists:
	@rm -f .filelist

compilefiles:
	@if [ '!' -d tmpclasses ]; then rm -rf tmpclasses; mkdir tmpclasses; fi;
	@if [ -f .filelist ]; then \
		echo $(JAVAC) -g$(DEBUGFLAG) -d tmpclasses -bootclasspath classes \
		      -classpath classes `cat .filelist`; \
		$(JAVAC) -g$(DEBUGFLAG) -d tmpclasses -bootclasspath classes \
		      -classpath classes `cat .filelist` || exit 1; \
		echo $(PREVERIFY) -d classes tmpclasses; \
		$(PREVERIFY) -d classes tmpclasses || exit 1; \
		fi

tools: eraselists $(CLASSFILES) compilefiles

classes.zip: tools
	@rm -rf classes.zip
	@$(JAR) cfM0 classes.zip -C classes .

clean:
	rm -rf *.zip
	rm -rf .filelist
	rm -rf classes
	rm -rf tmpclasses
	rm -rf *~ */*~ */*/*~
	rm -rf *# */*# */*/*#

