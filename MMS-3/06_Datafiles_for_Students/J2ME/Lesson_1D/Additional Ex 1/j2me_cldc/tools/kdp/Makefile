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

all: tools

JAVA          = java
JAVAC         = javac
JAR           = jar
JAVAFILES =  $(shell find src -name "*.java"|grep -v SCCS)
 
CLASSFILES = $(subst src,classes,$(JAVAFILES:java=class))

ifeq ($(DEBUG),true)
DEBUGFLAG=
else
DEBUGFLAG=":none"
endif

# $< is dependency
# $@ is target
#
$(CLASSFILES): classes/%.class : src/%.java
	@echo $< >> .filelist

eraselists:
	@rm -f .filelist 

compilefiles:
	@if [ '!' -d classes ]; then rm -rf classes; mkdir classes; fi;
	@if [ -f .filelist ]; then \
		echo $(JAVAC) `cat .filelist`; \
		$(JAVAC) -g$(DEBUGFLAG) -d classes -classpath classes \
	              `cat .filelist`; \
		fi

tools: eraselists $(CLASSFILES) compilefiles

classes.zip: tools
	@rm -rf classes.zip
	@$(JAR) cfM0 classes.zip -C classes .

clean:
	rm -rf *.zip *.pdb
	rm -rf .filelist
	rm -rf classes
	rm -rf *~ */*~ */*/*~
	rm -rf *# */*# */*/*#

