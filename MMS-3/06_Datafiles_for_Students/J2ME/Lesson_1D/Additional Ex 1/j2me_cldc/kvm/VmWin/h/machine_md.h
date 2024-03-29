/*
 * Copyright (c) 1998-2001 Sun Microsystems, Inc. All Rights Reserved.
 *
 * This software is the confidential and proprietary information of Sun
 * Microsystems, Inc. ("Confidential Information").  You shall not
 * disclose such Confidential Information and shall use it only in
 * accordance with the terms of the license agreement you entered into
 * with Sun.
 *
 * SUN MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
 * SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT. SUN SHALL NOT BE LIABLE FOR ANY DAMAGES
 * SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
 * THIS SOFTWARE OR ITS DERIVATIVES.
 *
 * Use is subject to license terms.
 */

/*=========================================================================
 * KVM
 *=========================================================================
 * SYSTEM:    KVM
 * FILE:      machine_md.h (for Windows)
 * OVERVIEW:  This file is included in every compilation.  It contains
 *            definitions that are specific to the Windows port of KVM.
 * AUTHOR:    Frank Yellin
 *            Ioi Lam
 *            Richard Berlin
 * NOTE:      This file overrides many of the default compilation
 *            flags and macros defined in VmCommon/h/main.h.
 *=======================================================================*/

/*=========================================================================
 * Platform definition
 *=======================================================================*/

#define WINDOWS 1

/*=========================================================================
 * Include files
 *=======================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef CONST

/*=========================================================================
 * Platform-specific datatype definitions
 *=======================================================================*/

typedef long             Long;
typedef char             Boolean;

#ifdef GCC
typedef long long          long64;  /* 64-bit signed integer type */
typedef unsigned long long ulong64; /* 64-bit unsigned integer type */
typedef long long          jlong;   /* Added for KNI */
#else
typedef __int64          long64;    /* 64-bit signed integer type */
typedef unsigned __int64 ulong64;   /* 64-bit unsigned integer type */
typedef __int64          jlong;     /* Added for KNI */
#endif

#ifndef Calendar_md
unsigned long *Calendar_md();
#endif

#define PATH_SEPARATOR    ';'

/*=========================================================================
 * Compilation flags and macros that override values defined in main.h
 *=======================================================================*/
#if ALTERNATIVE_FAST_INTERPRETER
#define ENABLE_JAVA_DEBUGGER 0
#define IPISLOCAL 1
#define SPISLOCAL 1
#define LPISLOCAL 1
#define FPISLOCAL 1
#define CPISLOCAL 1
#endif

/* This is a little-endian target platform */
#define LITTLE_ENDIAN 1

/* Make the VM run a little faster (can afford the extra space) */
#define ENABLEFASTBYTECODES 1

/* Use asynchronous native functions on Windows */
#define ASYNCHRONOUS_NATIVE_FUNCTIONS 1

#define SLEEP_UNTIL(wakeupTime)                            \
    {  long delta = (long)(wakeupTime - CurrentTime_md()); \
           Sleep(delta);                                   \
    }

/*=========================================================================
 * Platform-specific macros and function prototypes
 *=======================================================================*/

#define InitializeVM()
#define FinalizeVM()

#define freeHeap(x) free(x)
#define RandomNumber_md() rand()

void InitializeWindowSystem();
void FinalizeWindowSystem(void);

ulong64 sysTimeMillis(void);

/*=========================================================================
 * The following are used in several different places, and its worthwhile
 * to define them just once
 *=======================================================================*/

void* allocateVirtualMemory_md(long size);
void  freeVirtualMemory_md(void *address, long size);
enum { PVM_NoAccess, PVM_ReadOnly, PVM_ReadWrite };
void  protectVirtualMemory_md(void *address, long size, int protection);
int  getpagesize();

/*=========================================================================
 * Graphics system operations
 *=======================================================================*/

void winDrawRectangle(int x, int y, int width, int height, int mode);
void winFillRectangle(int x, int y, int width, int height,
                      int cornerDiam, int mode);
void winDrawLine(int x1, int y1, int x2, int y2, int mode);

int  winCharWidth(int c);
void winDrawChar(int c, int x, int y, int mode);
void winRefreshScreen(int x, int y, int width, int height);
void winSetClip(int x, int y, int width, int height);
void winResetClip();
void winCopyRegion(int x, int y, int width, int height, int dstX, int dstY, int mode);
void winCopyRectangle(int srcWin, int dstWin, int x, int y,
                  int width, int height, int dstX, int dstY, int mode);
void winDrawMonoBitmap(int x, int y, int width, int height, int rowBytes, char *bitmap);

/*
 * Things to make general native code build under Windows
 */

#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <direct.h>

#ifndef __GNUC__
/*
 * The following is already defined in GCC from the Cygwin32 distribution
 */
#define open    _open
#define close   _close
#define read    _read
#define write   _write
#define access  _access
#define umask   _umask
#define fstat   _fstat
#define stat    _stat
#define mkdir   _mkdir

#define S_IFMT   _S_IFMT
#define S_IFREG  _S_IFREG
#define S_IFCHR  _S_IFCHR
#define S_IFFIFO _S_IFIFO
#define S_IFDIR  _S_IFDIR
#define S_IFBLK  _S_IFBLK

#define O_BINARY _O_BINARY

#endif /* __GNUC__ */

#ifndef S_ISREG
#define S_ISREG(mode)   ( ((mode) & S_IFMT) == S_IFREG )
#define S_ISCHR(mode)   ( ((mode) & S_IFMT) == S_IFCHR )
#define S_ISFIFO(mode)  ( ((mode) & S_IFMT) == S_ISFIFO )
#define S_ISDIR(mode)   ( ((mode) & S_IFMT) == S_IFDIR )
#define S_ISBLK(mode)   ( ((mode) & S_IFMT) == S_IFBLK )
#endif


/*
 * Thread processing routines
 */

void releaseAsyncThread(void);
void processAcyncThread(void);
void Yield_md(void);

/*=========================================================================
 * FUNCTION:      The stub of GetAndStoreNextKVMEvent
 * TYPE:          event handler
 * OVERVIEW:      Wait for an external event to occur, and store
 *                the event to the KVM event queue.
 *                On a real device, the implementation of this
 *                function should try to conserve battery if 
 *                the routine is called with the 'forever'
 *                flag turned on, or with a 'waitUntil' parameter
 *                larger than zero.
 * INTERFACE
 *   parameters:  forever: a boolean flag that tells whether the
 *                KVM has anything to do or not.  When the KVM calls
 *                this routine with the forever flag turned on, the
 *                VM has no threads to run and this function can call
 *                machine-specific battery conservation/hibernation
 *                facilities.  If the forever flag is off, the function
 *                should try to return immediately, or at least after
 *                'waitUntil' milliseconds, since there are other threads
 *                that need to continue running.
 *                waitUntil: the amount of milliseconds this routine
 *                should wait in case the 'forever' flag is off.
 *   returns:     nothing directly, but the event should be stored
 *                in the event queue of the KVM (see events.c and
 *                the porting guide for details).
 *=======================================================================*/

#define GetAndStoreNextKVMEvent(x,y)
#define _NOT_IMPLEMENTED_GetAndStoreNextKVMEvent
