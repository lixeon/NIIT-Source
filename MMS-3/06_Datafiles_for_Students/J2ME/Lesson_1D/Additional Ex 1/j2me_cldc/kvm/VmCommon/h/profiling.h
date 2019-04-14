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
 * SUBSYSTEM: Profiling
 * FILE:      profiling.h
 * OVERVIEW:  The variables and operations defined in this file 
 *            execution allow the of the execution of the virtual
 *            machine to be profiled at runtime.
 * AUTHOR:    Antero Taivalsaari, Frank Yellin
 * NOTE:      The code in this file was originally in
 *            interpret.h, and moved to a separate file later.
 *=======================================================================*/

/*=========================================================================
 * Profiling variables
 *=======================================================================*/

#if ENABLEPROFILING

extern int InstructionCounter;       /* Number of bytecodes executed */
extern int ThreadSwitchCounter;      /* Number of thread switches */

extern int DynamicObjectCounter;     /* Number of dynamic objects allocated */
extern int DynamicAllocationCounter; /* Bytes of dynamic memory allocated */
extern int DynamicDeallocationCounter; /* Bytes of dynamic memory deallocated */
extern int GarbageCollectionCounter; /* Number of garbage collections done */
extern int TotalGCDeferrals;         /* Total number of GC objects deferred */
extern int MaximumGCDeferrals;       /* Maximum number of GC objects deferred */
extern int GarbageCollectionRescans; /* Number of extra scans of GC heap */

#if ENABLEFASTBYTECODES
extern int InlineCacheHitCounter;    /* Number of inline cache hits */
extern int InlineCacheMissCounter;   /* Number of inline cache misses */
extern int MaxStackCounter;          /* Maximum amount of stack space needed */
#endif

#if USESTATIC
extern int StaticObjectCounter;      /* Number of static objects allocated */
extern int StaticAllocationCounter;  /* Bytes of static memory allocated */
#endif

/*=========================================================================
 * Profiling operations
 *=======================================================================*/

void InitializeProfiling(void);
void printProfileInfo(void);

#else 

#define InitializeProfiling()
#define printProfileInfo()

#endif /* ENABLEPROFILING */

