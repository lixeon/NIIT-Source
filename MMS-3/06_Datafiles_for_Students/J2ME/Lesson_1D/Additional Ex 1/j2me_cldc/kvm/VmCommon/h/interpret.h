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
 * SYSTEM:    KVM
 * SUBSYSTEM: Bytecode interpreter
 * FILE:      interpret.h
 * OVERVIEW:  This file defines the general routines used by the
 *            Java bytecode interpreter.
 * AUTHOR:    Antero Taivalsaari, Sun Labs
 *            Major reorganization by Nik Shaylor 9/5/2000
 * NOTE:      In KVM 1.0.2, the interpreter was restructured for
 *            better performance, but without sacrificing portability.
 *            The high-level interpreter loop is now defined in file
 *            execute.c.  Actual bytecode definitions are in file
 *            bytecodes.c.  Various high-level compilation flags
 *            for the interpreter have been documented in main.h 
 *            and in the KVM Porting Guide.
 *=======================================================================*/

/*=========================================================================
 * Include files
 *=======================================================================*/

/*=========================================================================
 * Virtual machine global registers
 *=======================================================================*/

/*=========================================================================
 * These variables form the heart of the virtual machine. UP holds
 * a pointer to the currently executing VM thread, IP holds the current
 * instruction pointer (next bytecode to be executed), FP holds a
 * pointer to the currently active stack frame, LP holds a pointer
 * to the local variables of the current stack frame and SP holds a
 * pointer to the top of the execution stack.
 *
 * Note that IP, FP, LP and SP are thread-specific, i.e., their
 * values are changed every time a thread switch takes place.
 *=========================================================================
 * GENERAL COMMENT:
 * The main reason why virtual machines written in high-level languages
 * such as C/C++ are inherently slower than machine-coded ones is that
 * C/C++ does not provide any mechanism to allocate global variables
 * in hardware registers.  KVM provides a compilation option 
 * (LOCALVMREGISTERS, see main.h) to accomplish this in a portable 
 * fashion.
 *=======================================================================*/

struct GlobalStateStruct { 
    BYTE*         gs_ip; /*  Instruction pointer (program counter) */
    cell*         gs_sp; /*  Execution stack pointer */
    cell*         gs_lp; /*  Local variable pointer */
    CONSTANTPOOL  gs_cp; /*  Constant pool pointer */
    FRAME         gs_fp; /*  Current frame pointer */
};

extern struct GlobalStateStruct GlobalState;

#define ip_global GlobalState.gs_ip
#define sp_global GlobalState.gs_sp
#define lp_global GlobalState.gs_lp
#define cp_global GlobalState.gs_cp
#define fp_global GlobalState.gs_fp


/* These get and set macros provide better control */
/* over the way VM registers are accessed. */

#define getIP() (ip_global)
#define getFP() (fp_global)
#define getSP() (sp_global)
#define getLP() (lp_global)
#define getCP() (cp_global)

#define setIP(x) (ip_global = (x))
#define setFP(x) (fp_global = (x))
#define setSP(x) (sp_global = (x))
#define setLP(x) (lp_global = (x))
#define setCP(x) (cp_global = (x))

/*=========================================================================
 * Bytecode declarations
 *=======================================================================*/

typedef enum {
        NOP         = 0x00,
        ACONST_NULL = 0x01,
        ICONST_M1   = 0x02,
        ICONST_0    = 0x03,
        ICONST_1    = 0x04,
        ICONST_2    = 0x05,
        ICONST_3    = 0x06,
        ICONST_4    = 0x07,

        ICONST_5    = 0x08,
        LCONST_0    = 0x09,
        LCONST_1    = 0x0A,
        FCONST_0    = 0x0B,
        FCONST_1    = 0x0C,
        FCONST_2    = 0x0D,
        DCONST_0    = 0x0E,
        DCONST_1    = 0x0F,

        BIPUSH      = 0x10,
        SIPUSH      = 0x11,
        LDC         = 0x12,
        LDC_W       = 0x13,
        LDC2_W      = 0x14,
        ILOAD       = 0x15,
        LLOAD       = 0x16,
        FLOAD       = 0x17,

        DLOAD       = 0x18,
        ALOAD       = 0x19,
        ILOAD_0     = 0x1A,
        ILOAD_1     = 0x1B,
        ILOAD_2     = 0x1C,
        ILOAD_3     = 0x1D,
        LLOAD_0     = 0x1E,
        LLOAD_1     = 0x1F,

        LLOAD_2     = 0x20,
        LLOAD_3     = 0x21,
        FLOAD_0     = 0x22,
        FLOAD_1     = 0x23,
        FLOAD_2     = 0x24,
        FLOAD_3     = 0x25,
        DLOAD_0     = 0x26,
        DLOAD_1     = 0x27,

        DLOAD_2     = 0x28,
        DLOAD_3     = 0x29,
        ALOAD_0     = 0x2A,
        ALOAD_1     = 0x2B,
        ALOAD_2     = 0x2C,
        ALOAD_3     = 0x2D,
        IALOAD      = 0x2E,
        LALOAD      = 0x2F,

        FALOAD      = 0x30,
        DALOAD      = 0x31,
        AALOAD      = 0x32,
        BALOAD      = 0x33,
        CALOAD      = 0x34,
        SALOAD      = 0x35,
        ISTORE      = 0x36,
        LSTORE      = 0x37,

        FSTORE      = 0x38,
        DSTORE      = 0x39,
        ASTORE      = 0x3A,
        ISTORE_0    = 0x3B,
        ISTORE_1    = 0x3C,
        ISTORE_2    = 0x3D,
        ISTORE_3    = 0x3E,
        LSTORE_0    = 0x3F,

        LSTORE_1    = 0x40,
        LSTORE_2    = 0x41,
        LSTORE_3    = 0x42,
        FSTORE_0    = 0x43,
        FSTORE_1    = 0x44,
        FSTORE_2    = 0x45,
        FSTORE_3    = 0x46,
        DSTORE_0    = 0x47,

        DSTORE_1    = 0x48,
        DSTORE_2    = 0x49,
        DSTORE_3    = 0x4A,
        ASTORE_0    = 0x4B,
        ASTORE_1    = 0x4C,
        ASTORE_2    = 0x4D,
        ASTORE_3    = 0x4E,
        IASTORE     = 0x4F,

        LASTORE     = 0x50,
        FASTORE     = 0x51,
        DASTORE     = 0x52,
        AASTORE     = 0x53,
        BASTORE     = 0x54,
        CASTORE     = 0x55,
        SASTORE     = 0x56,
        POP         = 0x57,

        POP2        = 0x58,
        DUP         = 0x59,
        DUP_X1      = 0x5A,
        DUP_X2      = 0x5B,
        DUP2        = 0x5C,
        DUP2_X1     = 0x5D,
        DUP2_X2     = 0x5E,
        SWAP        = 0x5F,

        IADD        = 0x60,
        LADD        = 0x61,
        FADD        = 0x62,
        DADD        = 0x63,
        ISUB        = 0x64,
        LSUB        = 0x65,
        FSUB        = 0x66,
        DSUB        = 0x67,

        IMUL        = 0x68,
        LMUL        = 0x69,
        FMUL        = 0x6A,
        DMUL        = 0x6B,
        IDIV        = 0x6C,
        LDIV        = 0x6D,
        FDIV        = 0x6E,
        DDIV        = 0x6F,

        IREM        = 0x70,
        LREM        = 0x71,
        FREM        = 0x72,
        DREM        = 0x73,
        INEG        = 0x74,
        LNEG        = 0x75,
        FNEG        = 0x76,
        DNEG        = 0x77,

        ISHL        = 0x78,
        LSHL        = 0x79,
        ISHR        = 0x7A,
        LSHR        = 0x7B,
        IUSHR       = 0x7C,
        LUSHR       = 0x7D,
        IAND        = 0x7E,
        LAND        = 0x7F,

        IOR         = 0x80,
        LOR         = 0x81,
        IXOR        = 0x82,
        LXOR        = 0x83,
        IINC        = 0x84,
        I2L         = 0x85,
        I2F         = 0x86,
        I2D         = 0x87,

        L2I         = 0x88,
        L2F         = 0x89,
        L2D         = 0x8A,
        F2I         = 0x8B,
        F2L         = 0x8C,
        F2D         = 0x8D,
        D2I         = 0x8E,
        D2L         = 0x8F,

        D2F         = 0x90,
        I2B         = 0x91,
        I2C         = 0x92,
        I2S         = 0x93,
        LCMP        = 0x94,
        FCMPL       = 0x95,
        FCMPG       = 0x96,
        DCMPL       = 0x97,

        DCMPG       = 0x98,
        IFEQ        = 0x99,
        IFNE        = 0x9A,
        IFLT        = 0x9B,
        IFGE        = 0x9C,
        IFGT        = 0x9D,
        IFLE        = 0x9E,
        IF_ICMPEQ   = 0x9F,

        IF_ICMPNE   = 0xA0,
        IF_ICMPLT   = 0xA1,
        IF_ICMPGE   = 0xA2,
        IF_ICMPGT   = 0xA3,
        IF_ICMPLE   = 0xA4,
        IF_ACMPEQ   = 0xA5,
        IF_ACMPNE   = 0xA6,
        GOTO        = 0xA7,

        JSR          = 0xA8,
        RET          = 0xA9,
        TABLESWITCH  = 0xAA,
        LOOKUPSWITCH = 0xAB,
        IRETURN      = 0xAC,
        LRETURN      = 0xAD,
        FRETURN      = 0xAE,
        DRETURN      = 0xAF,

        ARETURN       = 0xB0,
        RETURN        = 0xB1,
        GETSTATIC     = 0xB2,
        PUTSTATIC     = 0xB3,
        GETFIELD      = 0xB4,
        PUTFIELD      = 0xB5,
        INVOKEVIRTUAL = 0xB6,
        INVOKESPECIAL = 0xB7,

        INVOKESTATIC    = 0xB8,
        INVOKEINTERFACE = 0xB9,
        UNUSED_BA       = 0xBA,
        NEW             = 0xBB,
        NEWARRAY        = 0xBC,
        ANEWARRAY       = 0xBD,
        ARRAYLENGTH     = 0xBE,
        ATHROW          = 0xBF,

        CHECKCAST       = 0xC0,
        INSTANCEOF      = 0xC1,
        MONITORENTER    = 0xC2,
        MONITOREXIT     = 0xC3,
        WIDE            = 0xC4,
        MULTIANEWARRAY  = 0xC5,
        IFNULL          = 0xC6,
        IFNONNULL       = 0xC7,

        GOTO_W          = 0xC8,
        JSR_W           = 0xC9,
        BREAKPOINT      = 0xCA,

/*=========================================================================
 * Fast bytecodes (used internally by the system
 *                 only if FASTBYTECODES flag is on)
 *=======================================================================*/

        GETFIELD_FAST         = 0xCB,
        GETFIELDP_FAST        = 0xCC,
        GETFIELD2_FAST        = 0xCD,
        PUTFIELD_FAST         = 0xCE,
        PUTFIELD2_FAST        = 0xCF,
        GETSTATIC_FAST        = 0xD0,
        GETSTATICP_FAST       = 0xD1,
        GETSTATIC2_FAST       = 0xD2,
        PUTSTATIC_FAST        = 0xD3,
        PUTSTATIC2_FAST       = 0xD4,
        UNUSED_D5             = 0xD5,
        INVOKEVIRTUAL_FAST    = 0xD6,
        INVOKESPECIAL_FAST    = 0xD7,

        INVOKESTATIC_FAST     = 0xD8,
        INVOKEINTERFACE_FAST  = 0xD9,
        NEW_FAST              = 0xDA,
        ANEWARRAY_FAST        = 0xDB,
        MULTIANEWARRAY_FAST   = 0xDC,
        CHECKCAST_FAST        = 0xDD,
        INSTANCEOF_FAST       = 0xDE,

        CUSTOMCODE            = 0xDF,

        LASTBYTECODE          = 0xDF
} ByteCode ;

#define BYTE_CODE_NAMES {              \
    "NOP",              /*  0x00 */  \
    "ACONST_NULL",      /*  0x01 */  \
    "ICONST_M1",        /*  0x02 */  \
    "ICONST_0",         /*  0x03 */  \
    "ICONST_1",         /*  0x04 */  \
    "ICONST_2",         /*  0x05 */  \
    "ICONST_3",         /*  0x06 */  \
    "ICONST_4",         /*  0x07 */  \
    "ICONST_5",         /*  0x08 */  \
    "LCONST_0",         /*  0x09 */  \
    "LCONST_1",         /*  0x0A */  \
    "FCONST_0",         /*  0x0B */  \
    "FCONST_1",         /*  0x0C */  \
    "FCONST_2",         /*  0x0D */  \
    "DCONST_0",         /*  0x0E */  \
    "DCONST_1",         /*  0x0F */  \
    "BIPUSH",           /*  0x10 */  \
    "SIPUSH",           /*  0x11 */  \
    "LDC",              /*  0x12 */  \
    "LDC_W",            /*  0x13 */  \
    "LDC2_W",           /*  0x14 */  \
    "ILOAD",            /*  0x15 */  \
    "LLOAD",            /*  0x16 */  \
    "FLOAD",            /*  0x17 */  \
    "DLOAD",            /*  0x18 */  \
    "ALOAD",            /*  0x19 */  \
    "ILOAD_0",          /*  0x1A */  \
    "ILOAD_1",          /*  0x1B */  \
    "ILOAD_2",          /*  0x1C */  \
    "ILOAD_3",          /*  0x1D */  \
    "LLOAD_0",          /*  0x1E */  \
    "LLOAD_1",          /*  0x1F */  \
    "LLOAD_2",          /*  0x20 */  \
    "LLOAD_3",          /*  0x21 */  \
    "FLOAD_0",          /*  0x22 */  \
    "FLOAD_1",          /*  0x23 */  \
    "FLOAD_2",          /*  0x24 */  \
    "FLOAD_3",          /*  0x25 */  \
    "DLOAD_0",          /*  0x26 */  \
    "DLOAD_1",          /*  0x27 */  \
    "DLOAD_2",          /*  0x28 */  \
    "DLOAD_3",          /*  0x29 */  \
    "ALOAD_0",          /*  0x2A */  \
    "ALOAD_1",          /*  0x2B */  \
    "ALOAD_2",          /*  0x2C */  \
    "ALOAD_3",          /*  0x2D */  \
    "IALOAD",           /*  0x2E */  \
    "LALOAD",           /*  0x2F */  \
    "FALOAD",           /*  0x30 */  \
    "DALOAD",           /*  0x31 */  \
    "AALOAD",           /*  0x32 */  \
    "BALOAD",           /*  0x33 */  \
    "CALOAD",           /*  0x34 */  \
    "SALOAD",           /*  0x35 */  \
    "ISTORE",           /*  0x36 */  \
    "LSTORE",           /*  0x37 */  \
    "FSTORE",           /*  0x38 */  \
    "DSTORE",           /*  0x39 */  \
    "ASTORE",           /*  0x3A */  \
    "ISTORE_0",         /*  0x3B */  \
    "ISTORE_1",         /*  0x3C */  \
    "ISTORE_2",         /*  0x3D */  \
    "ISTORE_3",         /*  0x3E */  \
    "LSTORE_0",         /*  0x3F */  \
    "LSTORE_1",         /*  0x40 */  \
    "LSTORE_2",         /*  0x41 */  \
    "LSTORE_3",         /*  0x42 */  \
    "FSTORE_0",         /*  0x43 */  \
    "FSTORE_1",         /*  0x44 */  \
    "FSTORE_2",         /*  0x45 */  \
    "FSTORE_3",         /*  0x46 */  \
    "DSTORE_0",         /*  0x47 */  \
    "DSTORE_1",         /*  0x48 */  \
    "DSTORE_2",         /*  0x49 */  \
    "DSTORE_3",         /*  0x4A */  \
    "ASTORE_0",         /*  0x4B */  \
    "ASTORE_1",         /*  0x4C */  \
    "ASTORE_2",         /*  0x4D */  \
    "ASTORE_3",         /*  0x4E */  \
    "IASTORE",          /*  0x4F */  \
    "LASTORE",          /*  0x50 */  \
    "FASTORE",          /*  0x51 */  \
    "DASTORE",          /*  0x52 */  \
    "AASTORE",          /*  0x53 */  \
    "BASTORE",          /*  0x54 */  \
    "CASTORE",          /*  0x55 */  \
    "SASTORE",          /*  0x56 */  \
    "POP",              /*  0x57 */  \
    "POP2",             /*  0x58 */  \
    "DUP",              /*  0x59 */  \
    "DUP_X1",           /*  0x5A */  \
    "DUP_X2",           /*  0x5B */  \
    "DUP2",             /*  0x5C */  \
    "DUP2_X1",          /*  0x5D */  \
    "DUP2_X2",          /*  0x5E */  \
    "SWAP",             /*  0x5F */  \
    "IADD",             /*  0x60 */  \
    "LADD",             /*  0x61 */  \
    "FADD",             /*  0x62 */  \
    "DADD",             /*  0x63 */  \
    "ISUB",             /*  0x64 */  \
    "LSUB",             /*  0x65 */  \
    "FSUB",             /*  0x66 */  \
    "DSUB",             /*  0x67 */  \
    "IMUL",             /*  0x68 */  \
    "LMUL",             /*  0x69 */  \
    "FMUL",             /*  0x6A */  \
    "DMUL",             /*  0x6B */  \
    "IDIV",             /*  0x6C */  \
    "LDIV",             /*  0x6D */  \
    "FDIV",             /*  0x6E */  \
    "DDIV",             /*  0x6F */  \
    "IREM",             /*  0x70 */  \
    "LREM",             /*  0x71 */  \
    "FREM",             /*  0x72 */  \
    "DREM",             /*  0x73 */  \
    "INEG",             /*  0x74 */  \
    "LNEG",             /*  0x75 */  \
    "FNEG",             /*  0x76 */  \
    "DNEG",             /*  0x77 */  \
    "ISHL",             /*  0x78 */  \
    "LSHL",             /*  0x79 */  \
    "ISHR",             /*  0x7A */  \
    "LSHR",             /*  0x7B */  \
    "IUSHR",            /*  0x7C */  \
    "LUSHR",            /*  0x7D */  \
    "IAND",             /*  0x7E */  \
    "LAND",             /*  0x7F */  \
    "IOR",              /*  0x80 */  \
    "LOR",              /*  0x81 */  \
    "IXOR",             /*  0x82 */  \
    "LXOR",             /*  0x83 */  \
    "IINC",             /*  0x84 */  \
    "I2L",              /*  0x85 */  \
    "I2F",              /*  0x86 */  \
    "I2D",              /*  0x87 */  \
    "L2I",              /*  0x88 */  \
    "L2F",              /*  0x89 */  \
    "L2D",              /*  0x8A */  \
    "F2I",              /*  0x8B */  \
    "F2L",              /*  0x8C */  \
    "F2D",              /*  0x8D */  \
    "D2I",              /*  0x8E */  \
    "D2L",              /*  0x8F */  \
    "D2F",              /*  0x90 */  \
    "I2B",              /*  0x91 */  \
    "I2C",              /*  0x92 */  \
    "I2S",              /*  0x93 */  \
    "LCMP",             /*  0x94 */  \
    "FCMPL",            /*  0x95 */  \
    "FCMPG",            /*  0x96 */  \
    "DCMPL",            /*  0x97 */  \
    "DCMPG",            /*  0x98 */  \
    "IFEQ",             /*  0x99 */  \
    "IFNE",             /*  0x9A */  \
    "IFLT",             /*  0x9B */  \
    "IFGE",             /*  0x9C */  \
    "IFGT",             /*  0x9D */  \
    "IFLE",             /*  0x9E */  \
    "IF_ICMPEQ",        /*  0x9F */  \
    "IF_ICMPNE",        /*  0xA0 */  \
    "IF_ICMPLT",        /*  0xA1 */  \
    "IF_ICMPGE",        /*  0xA2 */  \
    "IF_ICMPGT",        /*  0xA3 */  \
    "IF_ICMPLE",        /*  0xA4 */  \
    "IF_ACMPEQ",        /*  0xA5 */  \
    "IF_ACMPNE",        /*  0xA6 */  \
    "GOTO",             /*  0xA7 */  \
    "JSR",              /*  0xA8 */  \
    "RET",              /*  0xA9 */  \
    "TABLESWITCH",      /*  0xAA */  \
    "LOOKUPSWITCH",     /*  0xAB */  \
    "IRETURN",          /*  0xAC */  \
    "LRETURN",          /*  0xAD */  \
    "FRETURN",          /*  0xAE */  \
    "DRETURN",          /*  0xAF */  \
    "ARETURN",          /*  0xB0 */  \
    "RETURN",           /*  0xB1 */  \
    "GETSTATIC",        /*  0xB2 */  \
    "PUTSTATIC",        /*  0xB3 */  \
    "GETFIELD",         /*  0xB4 */  \
    "PUTFIELD",         /*  0xB5 */  \
    "INVOKEVIRTUAL",    /*  0xB6 */  \
    "INVOKESPECIAL",    /*  0xB7 */  \
    "INVOKESTATIC",     /*  0xB8 */  \
    "INVOKEINTERFACE",  /*  0xB9 */  \
    "UNUSED_BA",        /*  0xBA */  \
    "NEW",              /*  0xBB */  \
    "NEWARRAY",         /*  0xBC */  \
    "ANEWARRAY",        /*  0xBD */  \
    "ARRAYLENGTH",      /*  0xBE */  \
    "ATHROW",           /*  0xBF */  \
    "CHECKCAST",        /*  0xC0 */  \
    "INSTANCEOF",       /*  0xC1 */  \
    "MONITORENTER",     /*  0xC2 */  \
    "MONITOREXIT",      /*  0xC3 */  \
    "WIDE",             /*  0xC4 */  \
    "MULTIANEWARRAY",   /*  0xC5 */  \
    "IFNULL",           /*  0xC6 */  \
    "IFNONNULL",        /*  0xC7 */  \
    "GOTO_W",           /*  0xC8 */  \
    "JSR_W",            /*  0xC9 */  \
    "BREAKPOINT",       /*  0xCA */  \
    /*  Fast bytecodes: */           \
    "GETFIELD_FAST",        /*  0xCB */  \
    "GETFIELDP_FAST",       /*  0xCC */  \
    "GETFIELD2_FAST",       /*  0xCD */  \
    "PUTFIELD_FAST",        /*  0xCE */  \
    "PUTFIELD2_FAST",       /*  0xCF */  \
    "GETSTATIC_FAST",       /*  0xD0 */  \
    "GETSTATICP_FAST",      /*  0xD1 */  \
    "GETSTATIC2_FAST",      /*  0xD2 */  \
    "PUTSTATIC_FAST",       /*  0xD3 */  \
    "PUTSTATIC2_FAST",      /*  0xD4 */  \
    "UNUSED_D5",            /*  0xD5 */  \
    "INVOKEVIRTUAL_FAST",   /*  0xD6 */  \
    "INVOKESPECIAL_FAST",   /*  0xD7 */  \
    "INVOKESTATIC_FAST",    /*  0xD8 */  \
    "INVOKEINTERFACE_FAST", /*  0xD9 */  \
    "NEW_FAST",             /*  0xDA */  \
    "ANEWARRAY_FAST",       /*  0xDB */  \
    "MULTIANEWARRAY_FAST",  /*  0xDC */  \
    "CHECKCAST_FAST",       /*  0xDD */  \
    "INSTANCEOF_FAST",      /*  0xDE */  \
    "CUSTOMCODE"            /*  0xDF */ } 

/*=========================================================================
 * Callback function for CUSTOMCODE bytecode
 *=======================================================================*/

typedef void (*CustomCodeCallbackFunction)(FRAME_HANDLE exceptionFrame);

/*=========================================================================
 * Bytecode interpreter debugging operations
 *=======================================================================*/

#if INCLUDEDEBUGCODE
void printRegisterStatus(void);
void printVMstatus(void);
#else
#  define printRegisterStatus()
#  define printVMstatus()
#endif

#if ENABLEPROFILING
void InstructionProfile(void);
void printVMsummary(void);
#else
#  define printVMsummary()
#endif

#if __GNUC__
void fatalSlotError(CONSTANTPOOL, int cpIndex) __attribute__ ((noreturn));
void fatalIcacheMethodError(ICACHE icacheEntry) __attribute__ ((noreturn));
#else
void fatalSlotError(CONSTANTPOOL constantPool, int cpIndex);
void fatalIcacheMethodError(ICACHE icacheEntry);
#endif



#if INCLUDEDEBUGCODE
void TraceMethodEntry(METHOD method, const char *what);
void TraceMethodExit(METHOD method);
#else
#define TraceMethodEntry(method, what) /**/
#define TraceMethodExit(method) /**/
#endif

#if INCLUDEDEBUGCODE
void InstructionTrace(BYTE *traceip);
#endif

#if ENABLEPROFILING && ENABLEFASTBYTECODES
#define IncrInlineCacheHitCounter()  { InlineCacheHitCounter++;  }
#define IncrInlineCacheMissCounter() { InlineCacheMissCounter++; }
#else
#define IncrInlineCacheHitCounter() /**/
#define IncrInlineCacheMissCounter() /**/
#endif

/*=========================================================================
 * Macros for data alignment
 *=======================================================================*/

#if NEED_LONG_ALIGNMENT

#define GET_LONG(_addr) ( ((tdub).x[0] = ((long*)(_addr))[0]), \
                          ((tdub).x[1] = ((long*)(_addr))[1]), \
                          (tdub).l )

#define GET_ULONG(_addr) ( ((tdub).x[0] = ((long*)(_addr))[0]), \
                           ((tdub).x[1] = ((long*)(_addr))[1]), \
                           (tdub).ul )

#define SET_LONG( _addr, _v) ( (tdub).l = (_v),                   \
                               ((long*)(_addr))[0] = (tdub).x[0], \
                               ((long*)(_addr))[1] = (tdub).x[1] )


#define SET_ULONG( _addr, _v) ( (tdub).ul = (_v),                  \
                                ((long*)(_addr))[0] = (tdub).x[0], \
                                ((long*)(_addr))[1] = (tdub).x[1] )

#define COPY_LONG(_new, _old) ( ((long*)(_new))[0] = ((long*)(_old))[0], \
                                ((long*)(_new))[1] = ((long*)(_old))[1] )

#else

#define GET_LONG(_addr) (*(long64*)(_addr))
#define GET_ULONG(_addr) (*(ulong64*)(_addr))
#define SET_LONG(_addr, _v) (*(long64*)(_addr) = (_v))
#define SET_ULONG(_addr, _v) (*(ulong64*)(_addr) = (_v))
#define COPY_LONG(_new, _old) (*(long64*)(_new) = *(long64*)(_old))

#endif /* NEED_LONG_ALIGNMENT */


/* If double's must be aligned on doubleword boundaries then define this */
#if NEED_DOUBLE_ALIGNMENT

#define GET_DOUBLE(_addr) ( ((tdub).x[0] = ((long*)(_addr))[0]), \
                            ((tdub).x[1] = ((long*)(_addr))[1]), \
                            (tdub).d )
#define SET_DOUBLE( _addr, _v) ( (tdub).d = (_v),                   \
                                 ((long*)(_addr))[0] = (tdub).x[0], \
                                 ((long*)(_addr))[1] = (tdub).x[1] )
#define COPY_DOUBLE(_new, _old) ( ((long*)(_new))[0] = ((long*)(_old))[0], \
                                  ((long*)(_new))[1] = ((long*)(_old))[1] )

#else

#define GET_DOUBLE(_addr) (*(double*)(_addr))
#define SET_DOUBLE(_addr, _v) (*(double*)(_addr) = (_v))
#define COPY_DOUBLE(_new, _old) (*(double*)(_new) = *(double*)(_old))

#endif /* NEED_DOUBLE_ALIGNMENT */


/* Macros for handling endianness of the hardware architecture properly */
#if LITTLE_ENDIAN
#   define SET_LONG_FROM_HALVES(_addr, _hi, _lo) \
        (((long *)_addr)[0] = (_lo), ((long *)_addr)[1] = (_hi))
#elif BIG_ENDIAN
#   define SET_LONG_FROM_HALVES(_addr, _hi, _lo) \
        (((long *)_addr)[0] = (_hi), ((long *)_addr)[1] = (_lo))
#elif COMPILER_SUPPORTS_LONG
#  if NEED_LONG_ALIGNMENT
#     define SET_LONG_FROM_HALVES(_addr, _hi, _lo) \
             do { long64 _tmp = ((((long64)_hi)<<32) | ((unsigned long)_lo)); \
                      COPY_LONG(_addr, &_tmp); } while(0)
#  else
#     define SET_LONG_FROM_HALVES(_addr, _hi, _lo) \
          (*(long64 *)(_addr) = ((((long64)_hi)<<32) | ((unsigned long)_lo)))
#  endif
#else
#   error "You must define LITTLE_ENDIAN or BIG_ENDIAN"
#endif


/* Macros for popping and pushing 64-bit numbers from/to operand stack */
#define popDouble(_lval) (oneLess, COPY_DOUBLE(&_lval, getSP()), oneLess)
#define popLong(_lval) (oneLess, COPY_LONG(&_lval, getSP()), oneLess)

#define pushDouble(_lval) (oneMore, COPY_DOUBLE(getSP(), &_lval), oneMore)
#define pushLong(_lval) (oneMore, COPY_LONG(getSP(), &_lval), oneMore)

/*=========================================================================
 * The equivalent macros to the above for use when the stack being
 * manipulated is not for the currently executing thread. In all cases
 * the additional parameter is the THREAD pointer.
 *=======================================================================*/

#define popDoubleForThread( t, _lval) (oneLessForThread(t), COPY_DOUBLE(&_lval, spForThread(t)), oneLessForThread(t))
#define popLongForThread(   t, _lval) (oneLessForThread(t), COPY_LONG(  &_lval, spForThread(t)), oneLessForThread(t))
#define pushDoubleForThread(t, _lval) (oneMoreForThread(t), COPY_DOUBLE(spForThread(t), &_lval), oneMoreForThread(t))
#define pushLongForThread(  t, _lval) (oneMoreForThread(t), COPY_LONG(  spForThread(t), &_lval), oneMoreForThread(t))

/*=========================================================================
 * Bytecode interpreter operations
 *=======================================================================*/

void Interpret(void);
void SlowInterpret(ByteCode token);
void FastInterpret(void);

void InstructionProfile(void);

extern OBJECT thisObjectGCSafe;

typedef union Java8Str {
    long x[2];
#if IMPLEMENTS_FLOAT
    double d;
#endif
    long64 l;
        ulong64 ul;
    void *p;
} Java8;


#ifndef DOUBLE_REMAINDER
#define DOUBLE_REMAINDER(a,b) fmod(a,b)
#endif

/*=========================================================================
 * Scheduling macros.
 * These are here to isolate the scheduling code out of interpret.c
 * (now split out into a separate file)
 *=======================================================================*/

#include <events.h>

