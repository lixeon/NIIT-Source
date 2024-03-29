/*
 * Copyright (c) 2000-2001 Sun Microsystems, Inc. All Rights Reserved.
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
 * SUBSYSTEM: Debugger
 * FILE:      debugger.h
 * OVERVIEW:  Header file for Java level debugger code that implements
 *            a subset of the JPDA (Java-Platform Debug Architecture).
 *            For details on the implementation, refer to the KDWP
 *            Specification.
 * AUTHOR:    Bill Pittore, Sun Labs
 *=======================================================================*/
 
#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

/*                         
 * All the constants defined by
 * com.sun.tools.jdi.JDWP and all its nested classes. Since the names are
 * very long, the macros below are provided for convenience.
 */
#define JDWP_COMMAND_SET(name) JDWP_ ## name 
#define JDWP_COMMAND(set, name) JDWP_ ## set ## _ ## name
#define JDWP_REQUEST_MODIFIER(name) \
           JDWP_EventRequest_Set_Out_modifiers_Modifier_ ## name
#define JDWP_EVENT(name) \
           JDWP_EventKind_ ## name 
#define JDWP_ERROR(name) JDWP_Error_ ## name
#define JDWP_HIGHEST_COMMAND_SET 17

#define JDWP_EventKind_MIDLET_DEATH 100
#define JDWP_EventKind_METHOD_EXIT 41
#define JDWP_EventKind_EXCEPTION_CATCH 30
#define JDWP_EventKind_USER_DEFINED 5
#define JDWP_EventKind_METHOD_ENTRY 40
#define JDWP_EventKind_VM_DEATH 99
#define JDWP_EventKind_CLASS_UNLOAD 9
#define JDWP_EventKind_CLASS_PREPARE 8
#define JDWP_EventKind_SINGLE_STEP 1
#define JDWP_EventKind_FIELD_MODIFICATION 21
#define JDWP_EventKind_CLASS_LOAD 10
#define JDWP_EventKind_THREAD_START 6
#define JDWP_EventKind_FRAME_POP 3
#define JDWP_EventKind_VM_INIT 90
#define JDWP_EventKind_BREAKPOINT 2
#define JDWP_EventKind_THREAD_END 7
#define JDWP_EventKind_FIELD_ACCESS 20
#define JDWP_EventKind_EXCEPTION 4
#define JDWP_EventKind_VM_START JDWP_EventKind_VM_INIT
#define JDWP_EventKind_THREAD_DEATH JDWP_EventKind_THREAD_END

#define JDWP_VirtualMachine 1
#define JDWP_VirtualMachine_Version 1
#define JDWP_VirtualMachine_ClassesBySignature 2
#define JDWP_VirtualMachine_AllClasses 3
#define JDWP_VirtualMachine_AllThreads 4
#define JDWP_VirtualMachine_TopLevelThreadGroups 5
#define JDWP_VirtualMachine_Dispose 6
#define JDWP_VirtualMachine_IDSizes 7
#define JDWP_VirtualMachine_Suspend 8
#define JDWP_VirtualMachine_Resume 9
#define JDWP_VirtualMachine_Exit 10
#define JDWP_VirtualMachine_CreateString 11
#define JDWP_VirtualMachine_Capabilities 12
#define JDWP_VirtualMachine_ClassPaths 13
#define JDWP_VirtualMachine_DisposeObjects 14
#define JDWP_VirtualMachine_HoldEvents 15
#define JDWP_VirtualMachine_ReleaseEvents 16
#define JDWP_ReferenceType 2
#define JDWP_ReferenceType_Signature 1
#define JDWP_ReferenceType_ClassLoader 2
#define JDWP_ReferenceType_Modifiers 3
#define JDWP_ReferenceType_Fields 4
#define JDWP_ReferenceType_Methods 5
#define JDWP_ReferenceType_GetValues 6
#define JDWP_ReferenceType_SourceFile 7
#define JDWP_ReferenceType_NestedTypes 8
#define JDWP_ReferenceType_Status 9
#define JDWP_ReferenceType_Interfaces 10
#define JDWP_ReferenceType_ClassObject 11
#define JDWP_ClassType 3
#define JDWP_ClassType_Superclass 1
#define JDWP_ClassType_SetValues 2
#define JDWP_ClassType_InvokeMethod 3
#define JDWP_ClassType_NewInstance 4
#define JDWP_ArrayType 4
#define JDWP_ArrayType_NewInstance 1
#define JDWP_InterfaceType 5
#define JDWP_Method 6
#define JDWP_Method_LineTable 1
#define JDWP_Method_VariableTable 2
#define JDWP_Method_Bytecodes 3
#define JDWP_Field 8
#define JDWP_ObjectReference 9
#define JDWP_ObjectReference_ReferenceType 1
#define JDWP_ObjectReference_GetValues 2
#define JDWP_ObjectReference_SetValues 3
#define JDWP_ObjectReference_MonitorInfo 5
#define JDWP_ObjectReference_InvokeMethod 6
#define JDWP_ObjectReference_DisableCollection 7
#define JDWP_ObjectReference_EnableCollection 8
#define JDWP_ObjectReference_IsCollected 9
#define JDWP_StringReference 10
#define JDWP_StringReference_Value 1
#define JDWP_ThreadReference 11
#define JDWP_ThreadReference_Name 1
#define JDWP_ThreadReference_Suspend 2
#define JDWP_ThreadReference_Resume 3
#define JDWP_ThreadReference_Status 4
#define JDWP_ThreadReference_ThreadGroup 5
#define JDWP_ThreadReference_Frames 6
#define JDWP_ThreadReference_FrameCount 7
#define JDWP_ThreadReference_OwnedMonitors 8
#define JDWP_ThreadReference_CurrentContendedMonitor 9
#define JDWP_ThreadReference_Stop 10
#define JDWP_ThreadReference_Interrupt 11
#define JDWP_ThreadReference_SuspendCount 12
#define JDWP_ThreadGroupReference 12
#define JDWP_ThreadGroupReference_Name 1
#define JDWP_ThreadGroupReference_Parent 2
#define JDWP_ThreadGroupReference_Children 3
#define JDWP_ArrayReference 13
#define JDWP_ArrayReference_Length 1
#define JDWP_ArrayReference_GetValues 2
#define JDWP_ArrayReference_SetValues 3
#define JDWP_ClassLoaderReference 14
#define JDWP_ClassLoaderReference_VisibleClasses 1
#define JDWP_EventRequest 15
#define JDWP_EventRequest_Set 1
#define JDWP_EventRequest_Set_Out_modifiers_Modifier_Count 1
#define JDWP_EventRequest_Set_Out_modifiers_Modifier_Conditional 2
#define JDWP_EventRequest_Set_Out_modifiers_Modifier_ThreadOnly 3
#define JDWP_EventRequest_Set_Out_modifiers_Modifier_ClassOnly 4
#define JDWP_EventRequest_Set_Out_modifiers_Modifier_ClassMatch 5
#define JDWP_EventRequest_Set_Out_modifiers_Modifier_ClassExclude 6
#define JDWP_EventRequest_Set_Out_modifiers_Modifier_LocationOnly 7
#define JDWP_EventRequest_Set_Out_modifiers_Modifier_ExceptionOnly 8
#define JDWP_EventRequest_Set_Out_modifiers_Modifier_FieldOnly 9
#define JDWP_EventRequest_Set_Out_modifiers_Modifier_Step 10
#define JDWP_EventRequest_Clear 2
#define JDWP_EventRequest_ClearAllBreakpoints 3
#define JDWP_StackFrame 16
#define JDWP_StackFrame_GetValues 1
#define JDWP_StackFrame_SetValues 2
#define JDWP_StackFrame_ThisObject 3
#define JDWP_ClassObjectReference 17
#define JDWP_ClassObjectReference_ReflectedType 1
#define JDWP_Event 64
#define JDWP_Event_Composite 100
#define JDWP_Event_Composite_Event_events_Events_VMStart JDWP_EventKind_VM_START
#define JDWP_Event_Composite_Event_events_Events_SingleStep JDWP_EventKind_SINGLE_STEP
#define JDWP_Event_Composite_Event_events_Events_Breakpoint JDWP_EventKind_BREAKPOINT
#define JDWP_Event_Composite_Event_events_Events_MethodEntry JDWP_EventKind_METHOD_ENTRY
#define JDWP_Event_Composite_Event_events_Events_MethodExit JDWP_EventKind_METHOD_EXIT
#define JDWP_Event_Composite_Event_events_Events_Exception JDWP_EventKind_EXCEPTION
#define JDWP_Event_Composite_Event_events_Events_ThreadStart JDWP_EventKind_THREAD_START
#define JDWP_Event_Composite_Event_events_Events_ThreadDeath JDWP_EventKind_THREAD_DEATH
#define JDWP_Event_Composite_Event_events_Events_ClassPrepare JDWP_EventKind_CLASS_PREPARE
#define JDWP_Event_Composite_Event_events_Events_ClassUnload JDWP_EventKind_CLASS_UNLOAD
#define JDWP_Event_Composite_Event_events_Events_FieldAccess JDWP_EventKind_FIELD_ACCESS
#define JDWP_Event_Composite_Event_events_Events_FieldModification JDWP_EventKind_FIELD_MODIFICATION
#define JDWP_Event_Composite_Event_events_Events_VMDeath JDWP_EventKind_VM_DEATH
#define JDWP_Error_INVALID_TAG 500
#define JDWP_Error_ALREADY_INVOKING 502
#define JDWP_Error_INVALID_INDEX 503
#define JDWP_Error_INVALID_LENGTH 504
#define JDWP_Error_INVALID_STRING 506
#define JDWP_Error_INVALID_CLASS_LOADER 507
#define JDWP_Error_INVALID_ARRAY 508
#define JDWP_Error_TRANSPORT_LOAD 509
#define JDWP_Error_TRANSPORT_INIT 510
#define JDWP_Error_NATIVE_METHOD 511
#define JDWP_Error_INVALID_COUNT 512
#define JDWP_Error_VM_DEAD 112
#define JDWP_Error_INVALID_MONITOR 50
#define JDWP_Error_OUT_OF_MEMORY 110
#define JDWP_Error_INVALID_SLOT 35
#define JDWP_Error_INVALID_CLASS_FORMAT 60
#define JDWP_Error_INVALID_THREAD 10
#define JDWP_Error_INTERRUPT 52
#define JDWP_Error_NOT_MONITOR_OWNER 51
#define JDWP_Error_CIRCULAR_CLASS_DEFINITION 61
#define JDWP_Error_ACCESS_DENIED 111
#define JDWP_Error_INVALID_FIELDID 25
#define JDWP_Error_TYPE_MISMATCH 34
#define JDWP_Error_OPAQUE_FRAME 32
#define JDWP_Error_CLASS_NOT_PREPARED 22
#define JDWP_Error_FAILS_VERIFICATION 62
#define JDWP_Error_INVALID_METHODID 23
#define JDWP_Error_INVALID_CLASS 21
#define JDWP_Error_INVALID_OBJECT 20
#define JDWP_Error_ADD_METHOD_NOT_IMPLEMENTED 63
#define JDWP_Error_NULL_POINTER 100
#define JDWP_Error_DUPLICATE 40
#define JDWP_Error_INVALID_FRAMEID 30
#define JDWP_Error_UNATTACHED_THREAD 115
#define JDWP_Error_THREAD_NOT_SUSPENDED 13
#define JDWP_Error_INVALID_LOCATION 24
#define JDWP_Error_INVALID_TYPESTATE 65
#define JDWP_Error_THREAD_SUSPENDED 14
#define JDWP_Error_ABSENT_INFORMATION 101
#define JDWP_Error_INVALID_THREAD_GROUP 11
#define JDWP_Error_INTERNAL 113
#define JDWP_Error_NONE 0
#define JDWP_Error_INVALID_PRIORITY 12
#define JDWP_Error_ILLEGAL_ARGUMENT 103
#define JDWP_Error_SCHEMA_CHANGE_NOT_IMPLEMENTED 64
#define JDWP_Error_INVALID_EVENT_TYPE 102
#define JDWP_Error_NOT_CURRENT_FRAME 33
#define JDWP_Error_NOT_IMPLEMENTED 99
#define JDWP_Error_NO_MORE_FRAMES 31
#define JDWP_Error_NOT_FOUND 41
#define JDWP_ThreadStatus_UNKNOWN -1
#define JDWP_ThreadStatus_ZOMBIE 0
#define JDWP_ThreadStatus_RUNNING 1
#define JDWP_ThreadStatus_WAIT 4
#define JDWP_ThreadStatus_SLEEPING 2
#define JDWP_ThreadStatus_MONITOR 3
#define JDWP_SuspendStatus_SUSPEND_STATUS_SUSPENDED 0x1
#define JDWP_ClassStatus_PREPARED 2
#define JDWP_ClassStatus_VERIFIED 1
#define JDWP_ClassStatus_INITIALIZED 4
#define JDWP_ClassStatus_ERROR 8
#define JDWP_TypeTag_CLASS 1
#define JDWP_TypeTag_INTERFACE 2
#define JDWP_TypeTag_ARRAY 3
#define JDWP_Tag_ARRAY 91
#define JDWP_Tag_BYTE 66
#define JDWP_Tag_CHAR 67
#define JDWP_Tag_OBJECT 76
#define JDWP_Tag_FLOAT 70
#define JDWP_Tag_DOUBLE 68
#define JDWP_Tag_INT 73
#define JDWP_Tag_LONG 74
#define JDWP_Tag_SHORT 83
#define JDWP_Tag_VOID 86
#define JDWP_Tag_BOOLEAN 90
#define JDWP_Tag_STRING 115
#define JDWP_Tag_THREAD 116
#define JDWP_Tag_THREAD_GROUP 103
#define JDWP_Tag_CLASS_LOADER 108
#define JDWP_Tag_CLASS_OBJECT 99
#define JDWP_StepDepth_INTO 0
#define JDWP_StepDepth_OVER 1
#define JDWP_StepDepth_OUT 2
#define JDWP_StepSize_MIN 0
#define JDWP_StepSize_LINE 1
#define JDWP_SuspendPolicy_NONE 0
#define JDWP_SuspendPolicy_EVENT_THREAD 1
#define JDWP_SuspendPolicy_ALL 2
#define JDWP_InvokeOptions_INVOKE_SINGLE_THREADED 0x01
#define JDWP_InvokeOptions_INVOKE_NONVIRTUAL 0x02

#define KVM_CMDSET  128
#define KVM_HANDSHAKE_COMMAND 1
#define KVM_GETSTEPPINGINFO_COMMAND 2
#define KVM_STEPPING_EVENT_COMMAND 3

/*
 *  Misc control structures used by the debugger code
 */

#define SYS_OK          0
#define SYS_ERR        -1

/*-------------------------------*/

struct location_mod {
    unsigned long classID;
    unsigned long methodIndex;
    unsigned long       offset;
    unsigned long       dupCurrentLineOffs;
    unsigned long       postDupLineOffs;
    ByteCode     opcode;
    BYTE         tag;
};

struct exception_mod {
    unsigned long classID;
    bool_t   sig_caught;
    bool_t   sig_uncaught;
};

struct singleStep_mod {
    unsigned long threadID;
    long          size, depth;
    unsigned long       startingOffset;
    FRAME        fp;
    struct       location_mod target;
};

struct classMatch {
    char *className;
};

struct Modifiers {
    struct Modifiers *next;
    struct Modifiers *freeNext;
    BYTE kind;
    union {
        unsigned long          threadID;
        struct location_mod   loc;
        struct exception_mod  exp;
        struct singleStep_mod step;
        struct classMatch     classMatch;
        unsigned long          classID;
    } u;
    bool_t inUse;
};

struct CE_Modifiers {
    struct CE_Modifiers *next;
    struct CE_Modifiers *nextEvent;
    BYTE eventKind;
    unsigned long threadID;
    struct location_mod  loc;
    struct exception_mod exp;
    struct singleStep_mod step;
    bool_t inUse;
};

struct VmEvent {
    struct VmEvent *next; /* next event */
    struct VmEvent *sendNext; /* next event being sent */

    BYTE kind;
    BYTE suspendPolicy;

    bool_t active;
    long    numModifiers;
    struct Modifiers *mods;
    bool_t count_active;
    long    count;
    bool_t inUse;
};

#define SIZEOF_MODIFIERS   StructSizeInCells(Modifiers)
#define SIZEOF_CEMODIFIERS StructSizeInCells(Modifiers)
#define SIZEOF_VMEVENT     StructSizeInCells(VmEvent)

#define OBJECT_ROOT_SIZE 16

typedef struct VmEvent VMEvent, *VMEventPtr;
typedef struct Modifiers *EVENTMODIFIER;
typedef struct CE_Modifiers *CEModPtr;

#define MIN(x, y)          ((x) < (y) ? (x) : (y))

#define MAX_INPUT_PACKET_SIZE 300

#define DEBUG_MAX_CLASSNAME 256
#define DEBUG_MAX_SIG 64

/* Must match what's in the debug agent */
#define ONLY_THREADGROUP_ID  0xFFFFFFE0

#define Dbg_EventKind_NONE                      0x00000
#define Dbg_EventKind_ANY                       0xFFFFF
#define Dbg_EventKind_MIDLET_DEATH              0x40000
#define Dbg_EventKind_METHOD_EXIT               0x02000
#define Dbg_EventKind_EXCEPTION_CATCH           0x08000
#define Dbg_EventKind_USER_DEFINED              0x00010
#define Dbg_EventKind_METHOD_ENTRY              0x01000
#define Dbg_EventKind_VM_DEATH                  0x20000
#define Dbg_EventKind_CLASS_UNLOAD              0x00100
#define Dbg_EventKind_CLASS_PREPARE             0x00080
#define Dbg_EventKind_SINGLE_STEP               0x00001
#define Dbg_EventKind_FIELD_MODIFICATION        0x04000
#define Dbg_EventKind_CLASS_LOAD                0x00200
#define Dbg_EventKind_THREAD_START              0x00040
#define Dbg_EventKind_FRAME_POP                 0x00004
#define Dbg_EventKind_VM_INIT                   0x10000
#define Dbg_EventKind_BREAKPOINT                0x00002
#define Dbg_EventKind_THREAD_END                0x00020
#define Dbg_EventKind_FIELD_ACCESS              0x00800
#define Dbg_EventKind_EXCEPTION                 0x00008
#define Dbg_EventKind_VM_START                  Dbg_EventKind_VM_INIT
#define Dbg_EventKind_THREAD_DEATH              Dbg_EventKind_THREAD_END

extern long debuggerNotifyList;
#define NOTIFY_WANTED(x)       ( ( debuggerNotifyList & (x) ) == (x) )
#define checkNOTIFY_WANTED(x)  if ( !NOTIFY_WANTED(x) ) return;

extern bool_t suspend;       /* Suspend all threads on VM startup. */
extern bool_t vmDebugReady;
extern bool_t debuggerActive;
extern short debuggerPort;

typedef struct Debugger_ID_Hash_Entry {
    struct Debugger_ID_Hash_Entry *next;
    unsigned long key;
    unsigned long rootIndex;
} *ID_HASH_ENTRY;

#define DEBUGGER_HASH_SIZE 32
#define SIZEOF_ID_HASH_ENTRY    StructSizeInCells(Debugger_ID_Hash_Entry)

extern unsigned long uniqueDebuggerID;
#define DEBUGGER_ID_MASK 0x3FFFF
#define DEBUGGER_ID_SHIFT 14
#define DEBUGGER_INDEX_SHIFT 1
#define DEBUGGER_INDEX_MASK 0x1FFF

#define NEXT_UNIQUE_DEBUGGER_ID() ((uniqueDebuggerID++ & DEBUGGER_ID_MASK) << DEBUGGER_ID_SHIFT)

#define GET_OBJECT_DEBUGGERID(x) (inCurrentHeap(x) ? (getObjectID(x)) : (unsigned long)x)

/* Classes are always in permanent heap, so we can simplify */
#define GET_CLASS_DEBUGGERID(x) ((unsigned long)x)
#define GET_DEBUGGERID_CLASS(x) ((CLASS)x)

#define DEFAULT_DEBUGGER_PORT 2800

bool_t InitDebugger(void);
void CloseDebugger();
bool_t GetDebugger(int timeout);
void ProcessDebugCmds(int timeout);
void processBreakCommands(void);
ByteCode getBreakpointOpcode( VMEventPtr *, struct Modifiers ** );
void handleBreakpoint( THREAD thread );
int handleSingleStep(THREAD thread, THREAD *);
CEModPtr GetCEModifier();
void FreeCEModifier(CEModPtr);

ByteCode getVerifierBreakpointOpcode(METHOD, unsigned short);

int getJDWPClassStatus( INSTANCE_CLASS clazz );
bool_t verifyThread( THREAD thread );

void checkDebugEvent(THREAD);
void insertDebugEvent(CEModPtr);

void replaceEventOpcode(ByteCode);
void clearAllBreakpoints(void);
void sendAllClassPrepares(void);
void setEvent_VMInit( void );
void setEvent_VMDeath( void );
void setEvent_SingleStep(CEModPtr, THREAD);
void setEvent_Breakpoint(VMEventPtr ep, CEModPtr );
void setEvent_MethodEntry( void );
void setEvent_MethodExit( void );
void setEvent_Exception(THROWABLE_INSTANCE_HANDLE, FRAME, BYTE *, METHOD, unsigned long, CEModPtr );
void setEvent_ExceptionCatch();
void setEvent_ThreadStart( CEModPtr );
void setEvent_ThreadDeath( CEModPtr );
void setEvent_ClassPrepare( CEModPtr );
void setEvent_ClassLoad( CEModPtr );
void setEvent_ClassUnload( CEModPtr );
void setEvent_FieldAccess( void );
void setEvent_FieldModification( void );
void setEvent_FramePop( void );
void setEvent_MidletDeath(CEModPtr);

unsigned long getObjectID(OBJECT);
OBJECT getObjectPtr(unsigned long);

bool_t InitDebuggerIO(void);
void CloseDebuggerIO();
bool_t GetDebuggerChannel(int timeout);
bool_t dbgCharAvail(int timeout);
int dbgWrite(void *buf, int len);
int dbgAsyncWrite(void *buf, int len);
int dbgStdOutWrite(void *buf, int len);
int dbgRead(void *buf, int len, int flag);
int dbgReceivePacket(PACKET_INPUT_STREAM_HANDLE);
int dbgSendPacket(struct Packet *);
bool_t dbgInitialized(void);
void dbgFlush(void);

#endif /* __DEBUGGER_H__ */


