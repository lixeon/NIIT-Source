/*
 * Copyright (c) 1998-2002 Sun Microsystems, Inc. All Rights Reserved.
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
 * SYSTEM:    KVM
 * SUBSYSTEM: Global definitions
 * FILE:      global.h
 * OVERVIEW:  Global system-wide definitions.
 * AUTHOR:    Antero Taivalsaari, Sun Labs, 1998
 *            Many others since then...
 *=======================================================================*/

/*=========================================================================
 * COMMENT:
 * This file contains declarations that do not belong to any
 * particular structure or subsystem of the VM. There are many
 * other additional global variable declarations in other files.
 *=======================================================================*/

/*=========================================================================
 * System include files
 *=======================================================================*/

/* The performs per-machine initialization */
#include <machine_md.h>

/*=========================================================================
 * Global compile-time constants and typedefs
 *=======================================================================*/

#undef  TRUE
#undef  FALSE

#define NIL   0

typedef enum {
    FALSE = 0,
    TRUE = 1
} bool_t;

/*=========================================================================
 * Global data type declarations
 *=======================================================================*/

/*=========================================================================
 * NOTE: These type declarations are not quite as portable as they
 * could be.  It might be useful to declare specific type names for
 * all Java-specific types (e.g., jint instead of normal int, jlong
 * instead of long64, and so forth).
 *=======================================================================*/

#define CELL  4        /* Size of a java word (= 4 bytes) */
#define log2CELL 2     /* Shift amount equivalent to dividing by CELL */
#define SHORTSIZE 2

typedef unsigned char     BYTE;
typedef unsigned long     cell;    /* Must be 32 bits long! */

/*=========================================================================
 * System-wide structure declarations
 *=======================================================================*/

typedef struct classStruct*         CLASS;
typedef struct instanceClassStruct* INSTANCE_CLASS;
typedef struct arrayClassStruct*    ARRAY_CLASS;

typedef struct objectStruct*        OBJECT;
typedef struct instanceStruct*      INSTANCE;
typedef struct arrayStruct*         ARRAY;
typedef struct stringInstanceStruct* STRING_INSTANCE;
typedef struct throwableInstanceStruct* THROWABLE_INSTANCE;
typedef struct internedStringInstanceStruct* INTERNED_STRING_INSTANCE;
typedef struct classInstanceStruct* CLASS_INSTANCE;

typedef struct byteArrayStruct*     BYTEARRAY;
typedef struct shortArrayStruct*    SHORTARRAY;
typedef struct pointerListStruct*   POINTERLIST;
typedef struct weakPointerListStruct* WEAKPOINTERLIST;

typedef struct fieldStruct*         FIELD;
typedef struct fieldTableStruct*    FIELDTABLE;
typedef struct methodStruct*        METHOD;
typedef struct methodTableStruct*   METHODTABLE;
typedef struct stackMapStruct*      STACKMAP;
typedef struct icacheStruct*        ICACHE;
typedef struct chunkStruct*         CHUNK;

typedef struct staticChunkStruct*   STATICCHUNK;
typedef struct threadQueue*         THREAD;
typedef struct javaThreadStruct*    JAVATHREAD;
typedef struct monitorStruct*       MONITOR;

typedef struct stackStruct*         STACK;

typedef struct frameStruct*            FRAME;
typedef struct exceptionHandlerStruct* HANDLER;
typedef struct exceptionHandlerTableStruct* HANDLERTABLE;
typedef struct filePointerStruct*      FILEPOINTER;
typedef union constantPoolEntryStruct* CONSTANTPOOL_ENTRY;
typedef struct constantPoolStruct*     CONSTANTPOOL;
typedef char* BYTES;

typedef FILEPOINTER        *FILEPOINTER_HANDLE;
typedef OBJECT             *OBJECT_HANDLE;
typedef INSTANCE           *INSTANCE_HANDLE;
typedef ARRAY              *ARRAY_HANDLE;
typedef POINTERLIST        *POINTERLIST_HANDLE;
typedef WEAKPOINTERLIST    *WEAKPOINTERLIST_HANDLE;
typedef JAVATHREAD         *JAVATHREAD_HANDLE;
typedef BYTES              *BYTES_HANDLE;
typedef METHOD             *METHOD_HANDLE;
typedef FRAME              *FRAME_HANDLE;
typedef const char*        *CONST_CHAR_HANDLE;
typedef unsigned char*     *UNSIGNED_CHAR_HANDLE;
typedef STRING_INSTANCE    *STRING_INSTANCE_HANDLE;
typedef THROWABLE_INSTANCE *THROWABLE_INSTANCE_HANDLE;
typedef THREAD             *THREAD_HANDLE;

#define ByteSizeToCellSize(n)   (((n) + (CELL - 1)) >> log2CELL)
#define StructSizeInCells(structName) ((sizeof(struct structName) + 3) >> 2)
#define UnionSizeInCells(structName) ((sizeof(union structName) + 3) >> 2)

/* Field and Method key types */
typedef unsigned short NameKey;
typedef unsigned short MethodTypeKey;
typedef unsigned short FieldTypeKey;

typedef union {
    struct {
        unsigned short nameKey;
        unsigned short typeKey; /* either MethodTypeKey or FieldTypeKey */
    } nt;
    unsigned long i;
} NameTypeKey;

/* Machines such as the Palm that use something other than FILE* for stdin
 * and stdout should redefine this in their machine_md.h
 */
#ifndef LOGFILEPTR
#define LOGFILEPTR FILE*
#endif

/*=========================================================================
 * Locally defined include files
 *=======================================================================*/

#include <messages.h>

#include <main.h>
#include <long.h>
#include <hashtable.h>

#include <class.h>
#include <garbage.h>
#include <interpret.h>

#if ENABLE_JAVA_DEBUGGER
#include <debuggerStreams.h>
#include <debugger.h>
#endif /* ENABLE_JAVA_DEBUGGER */

#include <thread.h>
#include <pool.h>
#include <fields.h>
#include <frame.h>
#include <loader.h>
#include <native.h>
#include <cache.h>

#include <runtime.h>
#include <profiling.h>
#include <verifier.h>
#include <log.h>
#include <property.h>

/* The network protocol implementations.  These */
/* are not part of the CLDC Specification. */
#include <commProtocol.h>
#include <datagramProtocol.h>
#include <socketProtocol.h>

/*=========================================================================
 * Miscellaneous global variables
 *=======================================================================*/

/* Shared string buffer that is used internally by the VM */
extern char str_buffer[];

/* Requested heap size when starting the VM from command line */
extern long RequestedHeapSize;

/*=========================================================================
 * Global execution modes
 *=======================================================================*/

/*  Flags for toggling certain global modes on and off */
extern bool_t JamEnabled;
extern bool_t JamRepeat;

/*=========================================================================
 * Macros for controlling global execution tracing modes
 *=======================================================================*/

/* The following isn't really intended to be unreadable, but it simplifies
 * the maintenance of the various execution tracing flags.
 * 
 * NOTE: Logically these operations belong to VmExtra directory,
 * since this code is not useful for those ports that do not support
 * command line operation.  However, since this code is intimately
 * tied with the tracing capabilities of the core KVM, we'll keep
 * these definitions in this file for the time being.
 *
 * The intent is that you can call
 *    FOR_EACH_TRACE_FLAG(Macro_Of_Two_Arguments)
 * where Macro_Of_Two_Arguments is a two-argument macro whose first argument
 * is the name of a variable, and the second argument is the string the user
 * enters to turn on that flag.
 */
#define FOR_EACH_TRACE_FLAG(Macro) \
     FOR_EACH_ORDINARY_TRACE_FLAG(Macro) \
     FOR_EACH_DEBUGGER_TRACE_FLAG(Macro) \
     FOR_EACH_JAM_TRACE_FLAG(Macro) 

/* The ordinary trace flags are those included in any debugging build */
#if INCLUDEDEBUGCODE
#   define FOR_EACH_ORDINARY_TRACE_FLAG(Macro)  \
         Macro(tracememoryallocation,         "-traceallocation") \
         Macro(tracegarbagecollection,        "-tracegc") \
         Macro(tracegarbagecollectionverbose, "-tracegcverbose") \
         Macro(traceclassloading,             "-traceclassloading") \
         Macro(traceclassloadingverbose,      "-traceclassloadingverbose") \
         Macro(traceverifier,                 "-traceverifier") \
         Macro(tracestackmaps,                "-tracestackmaps") \
         Macro(tracebytecodes,                "-tracebytecodes") \
         Macro(tracemethodcalls,              "-tracemethods") \
         Macro(tracemethodcallsverbose,       "-tracemethodsverbose") \
         Macro(tracestackchunks,              "-tracestackchunks") \
         Macro(traceframes,                   "-traceframes") \
         Macro(traceexceptions,               "-traceexceptions") \
         Macro(traceevents,                   "-traceevents") \
         Macro(tracemonitors,                 "-tracemonitors") \
         Macro(tracethreading,                "-tracethreading") \
         Macro(tracenetworking,               "-tracenetworking") 
#else 
#    define FOR_EACH_ORDINARY_TRACE_FLAG(Macro)
#endif

/* The debugger tracing flags are those included only if we support KWDP */
#if INCLUDEDEBUGCODE && ENABLE_JAVA_DEBUGGER
#  define FOR_EACH_DEBUGGER_TRACE_FLAG(Macro)  \
               Macro(tracedebugger, "-tracedebugger") 
#else 
#  define FOR_EACH_DEBUGGER_TRACE_FLAG(Macro)
#endif

/* The debugger tracing flags are those included only if we support KWDP */
#if INCLUDEDEBUGCODE && USE_JAM
#  define FOR_EACH_JAM_TRACE_FLAG(Macro)  \
               Macro(tracejam, "-tracejam") 
#else 
#  define FOR_EACH_JAM_TRACE_FLAG(Macro)
#endif

/* Declare each of the trace flags to be external.  Note that we define
 * a two-variable macro, then use that macro as an argument to
 * FOR_EACH_TRACE_FLAG 
 */

#define DECLARE_TRACE_VAR_EXTERNAL(varName, userName) \
    extern int varName;
FOR_EACH_TRACE_FLAG(DECLARE_TRACE_VAR_EXTERNAL)

/*=========================================================================
 * Most frequently called functions are "inlined" here
 *=======================================================================*/

/*=========================================================================
 * Quick operations for stack manipulation (for manipulating stack
 * frames and operands).
 *=======================================================================*/

#define topStack                        (*getSP())
#define secondStack                     (*(getSP()-1))
#define thirdStack                      (*(getSP()-2))
#define fourthStack                     (*(getSP()-3))

#define topStackAsType(_type_)          (*(_type_ *)(getSP()))
#define secondStackAsType(_type_)       (*(_type_ *)(getSP() - 1))
#define thirdStackAsType(_type_)        (*(_type_ *)(getSP() - 2))
#define fourthStackAsType(_type_)       (*(_type_ *)(getSP() - 3))

#define oneMore                         getSP()++
#define oneLess                         getSP()--
#define moreStack(n)                    getSP() += (n)
#define lessStack(n)                    getSP() -= (n)

#define popStack()                      (*getSP()--)
#define popStackAsType(_type_)          (*(_type_ *)(getSP()--))

#define pushStack(data)                 *++getSP() = (data)
#define pushStackAsType(_type_, data)   *(_type_ *)(++getSP()) = (data)

/*=========================================================================
 * The equivalent macros to the above for use when the stack being
 * manipulated is not for the currently executing thread. In all cases
 * the additional parameter is the THREAD pointer.
 *=======================================================================*/

#define topStackForThread(t)         (*((t)->spStore))
#define secondStackForThread(t)      (*((t)->spStore - 1))
#define thirdStackForThread(t)       (*((t)->spStore - 2))
#define fourthStackForThread(t)      (*((t)->spStore - 3))

#define popStackForThread(t)         (*((t)->spStore--))
#define pushStackForThread(t, data)  (*(++(t)->spStore) = (data))
#define popStackAsTypeForThread(t, _type_) \
                                     (*(_type_*)((t)->spStore--))
#define pushStackAsTypeForThread(t, _type_, data)    \
                                     (*(_type_ *)(++(t)->spStore) = (data))
#define moreStackForThread(t, n)     ((t)->spStore += (n))
#define lessStackForThread(t, n)     ((t)->spStore -= (n))
#define oneMoreForThread(t)          ((t)->spStore++)
#define oneLessForThread(t)          ((t)->spStore--)

#define spForThread(t)               ((t)->spStore)

/*=========================================================================
 * Error handling definitions
 *=======================================================================*/

#ifndef ERROR_TRY
#  include <setjmp.h>
#  define ERROR_TRY  \
    {  jmp_buf *__prev__ = topJumpBuffer;                       \
           int *__prev__value = topJumpBuffer_value;            \
           jmp_buf __jmp_buf__;                                 \
           int __value__;                                       \
       int __state__;                                           \
       topJumpBuffer = &__jmp_buf__;                            \
       topJumpBuffer_value = &__value__;                        \
       if ((__state__ = setjmp(__jmp_buf__)) == 0) {

#  define ERROR_CATCH(var)                                      \
        }                                                       \
        topJumpBuffer = __prev__;                               \
        topJumpBuffer_value = __prev__value;                    \
        if (__state__ != 0) {                                   \
            long var = __value__;

#  define ERROR_END_CATCH     } }

#  define ERROR_THROW(i)                                        \
        *topJumpBuffer_value = i;                               \
        longjmp(*topJumpBuffer, i)

extern jmp_buf *topJumpBuffer;
extern int     *topJumpBuffer_value;

#define NEED_GLOBAL_JUMPBUFFER 1
#endif /* ERROR_TRY */

#ifndef FATAL_ERROR_EXIT_CODE
#define FATAL_ERROR_EXIT_CODE 127
#endif

/*=========================================================================
 * Operations for handling memory fetches and stores
 *=======================================================================*/

/*=========================================================================
 * These macros define Java-specific memory read/write operations
 * for reading high-endian numbers.
 *=======================================================================*/

/* Get a 32-bit value from the given memory location */
#define getCell(addr) \
                  ((((long)(((unsigned char *)(addr))[0])) << 24) |  \
                   (((long)(((unsigned char *)(addr))[1])) << 16) |  \
                   (((long)(((unsigned char *)(addr))[2])) << 8)  |  \
                   (((long)(((unsigned char *)(addr))[3])) << 0)) 

#if BIG_ENDIAN
#define getAlignedCell(addr) (*(long *)(addr))
#else 
#define getAlignedCell(addr) getCell(addr)
#endif

/* Get an unsigned 16-bit value from the given memory location */
#define getUShort(addr) \
                  ((((unsigned short)(((unsigned char *)(addr))[0])) << 8)  | \
                   (((unsigned short)(((unsigned char *)(addr))[1])) << 0))

/* Get a 16-bit value from the given memory location */
#define getShort(addr) ((short)(getUShort(addr)))

/* Store a 16-bit value in the given memory location */ 
#define putShort(addr, value) \
              ((unsigned char *)(addr))[0] = (unsigned char)((value) >> 8); \
              ((unsigned char *)(addr))[1] = (unsigned char)((value) & 0xFF)


