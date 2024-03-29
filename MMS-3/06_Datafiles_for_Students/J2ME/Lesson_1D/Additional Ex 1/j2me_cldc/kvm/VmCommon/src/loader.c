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
 * SUBSYSTEM: Class loader
 * FILE:      loader.c
 * OVERVIEW:  Structures and operations needed for loading
 *            Java classfiles (class loader).
 * AUTHOR:    Antero Taivalsaari, Sun Labs
 *            Edited by Doug Simon 11/1998
 *            Extensive changes for spec-compliance by Sheng Liang
 *            Frank Yellin (lots of additional checks for JLS (Java
 *            Language Spec) compliance)
 *=======================================================================*/

/*=========================================================================
 * Include files
 *=======================================================================*/

#include <global.h>

/*=========================================================================
 * Macros and constants
 *=======================================================================*/

#define STRING_POOL(i)   (*((char **)(&StringPool->data[i])))

#define RECOGNIZED_CLASS_FLAGS (ACC_PUBLIC | \
                    ACC_FINAL | \
                    ACC_SUPER | \
                    ACC_INTERFACE | \
                    ACC_ABSTRACT)    
       
#define RECOGNIZED_FIELD_FLAGS (ACC_PUBLIC | \
                    ACC_PRIVATE | \
                    ACC_PROTECTED | \
                    ACC_STATIC | \
                    ACC_FINAL | \
                    ACC_VOLATILE | \
                    ACC_TRANSIENT)

#define RECOGNIZED_METHOD_FLAGS (ACC_PUBLIC | \
                     ACC_PRIVATE | \
                     ACC_PROTECTED | \
                     ACC_STATIC | \
                     ACC_FINAL | \
                     ACC_SYNCHRONIZED | \
                     ACC_NATIVE | \
                     ACC_ABSTRACT )

/*=========================================================================
 * Static functions (used only in this file)
 *=======================================================================*/

#if USESTATIC
static void moveClassFieldsToStatic(INSTANCE_CLASS CurrentClass);
#else
# define moveClassFieldsToStatic(CurrentClass)
#endif

static void ignoreAttributes(FILEPOINTER_HANDLE ClassFile, 
                             POINTERLIST_HANDLE StringPool);

static void NoClassDefFoundError(INSTANCE_CLASS thisClass);

/*=========================================================================
 * Class file verification operations (performed during class loading)
 *=======================================================================*/

/*=========================================================================
 * FUNCTION:      skipOverFieldName()
 * TYPE:          private class file load operation
 * OVERVIEW:      skip over a legal field name in a get a given string
 * INTERFACE:
 *   parameters:  string: a string in which the field name is skipped
 *                slash_okay: is '/' OK.
 *                length: length of the string
 *   returns:     what's remaining after skipping the field name
 *=======================================================================*/

static const char *
skipOverFieldName(const char *string, bool_t slash_okay, unsigned short length)
{
    const char *p;
    unsigned short ch;
    unsigned short last_ch = 0;
    /* last_ch == 0 implies we are looking at the first char. */
    for (p = string; p != string + length; last_ch = ch) {
        const char *old_p = p;
        ch = *p;
        if (ch < 128) {
            p++;
            /* quick check for ascii */
            if ((ch >= 'a' && ch <= 'z') ||
                (ch >= 'A' && ch <= 'Z') ||
                (last_ch && ch >= '0' && ch <= '9')) {
                continue;
            }
        } else {
            /* KVM simplification: we give up checking for those Unicode
               chars larger than 127. Otherwise we'll have to include a
               couple of large Unicode tables, bloating the footprint by
               8~10K. */
            const char *tmp_p = p;
            ch = utf2unicode((const char**)&tmp_p);
            p = tmp_p;
            continue;
        }

        if (slash_okay && ch == '/' && last_ch) {
            if (last_ch == '/') {
                return NULL;    /* Don't permit consecutive slashes */
            }
        } else if (ch == '_' || ch == '$') {
            /* continue */
        } else {
            return last_ch ? old_p : NULL;
        }
    }
    return last_ch ? p : NULL;
}

/*=========================================================================
 * FUNCTION:      skipOverFieldType()
 * TYPE:          private class file load operation
 * OVERVIEW:      skip over a legal field signature in a get a given string
 * INTERFACE:
 *   parameters:  string: a string in which the field signature is skipped
 *                slash_okay: is '/' OK.
 *                length: length of the string
 *   returns:     what's remaining after skipping the field signature
 *=======================================================================*/

static const char *
skipOverFieldType(const char *string, bool_t void_okay, unsigned short length)
{
    unsigned int depth = 0;
    for (;length > 0;) {
        switch (string[0]) {
        case 'V':
            if (!void_okay) return NULL;
            /* FALL THROUGH */
        case 'Z':
        case 'B':
        case 'C':
        case 'S':
        case 'I':
        case 'J':
#if IMPLEMENTS_FLOAT
        case 'F':
        case 'D':
#endif
            return string + 1;

        case 'L': {
            /* Skip over the class name, if one is there. */
            const char *p = skipOverFieldName((const char *) (string + 1), 
                                           TRUE, (unsigned short) (length - 1));
            /* The next character better be a semicolon. */
            if (p != NULL && p < string + length && p[0] == ';') { 
                return p + 1;
            } 
            return NULL;
        }
        
        case '[':
            /* The rest of what's there better be a legal signature.  */
            string++;
            length--;
            if (++depth == 256) { 
                return NULL;
            }
            void_okay = FALSE;
            break;

        default:
            return NULL;
        }
    }
    return NULL;
}

/*=========================================================================
 * FUNCTION:      getUTF8String()
 * TYPE:          private class file load operation
 * OVERVIEW:      get a UTF8 string from string pool, check index validity
 * INTERFACE:
 *   parameters:  String pool, index
 *   returns:     Pointer to UTF8 string
 *=======================================================================*/

static char *
getUTF8String(POINTERLIST_HANDLE StringPoolH, unsigned short index)
{
    POINTERLIST StringPool = unhand(StringPoolH);
    if (index >= StringPool->length ||
        STRING_POOL(index) == NULL) {
        fatalError(KVM_MSG_BAD_UTF8_INDEX);
    }
    return STRING_POOL(index);
}

/*=========================================================================
 * FUNCTION:      verifyUTF8String()
 * TYPE:          private class file load operation
 * OVERVIEW:      validate a UTF8 string
 * INTERFACE:
 *   parameters:  pointer to UTF8 string
 *   returns:     nothing
 *=======================================================================*/

static void
verifyUTF8String(char* bytes, unsigned short length)
{
    unsigned int i;
    for (i=0; i<length; i++) {
        unsigned int c = ((unsigned char *)bytes)[i];
        if (c == 0) /* no embedded zeros */
            goto failed;
        if (c < 128)
            continue;
        switch (c >> 4) {
        default:
            break;

        case 0x8: case 0x9: case 0xA: case 0xB: case 0xF:
            goto failed;
            
        case 0xC: case 0xD: 
            /* 110xxxxx  10xxxxxx */
            i++;
            if (i >= length)
                goto failed;
            if ((bytes[i] & 0xC0) == 0x80)
                break;
            goto failed;
            
        case 0xE:
            /* 1110xxxx 10xxxxxx 10xxxxxx */
            i += 2;
            if (i >= length)
                goto failed;
            if (((bytes[i-1] & 0xC0) == 0x80) &&
                ((bytes[i] & 0xC0) == 0x80))
                break;
            goto failed;
        } /* end of switch */
    }
    return;

 failed:
    fatalError(KVM_MSG_BAD_UTF8_STRING);
}

/*=========================================================================
 * FUNCTION:      verifyName()
 * TYPE:          private class file load operation
 * OVERVIEW:      validate a class, field, or method name
 * INTERFACE:
 *   parameters:  pointer to a name, name type
 *   returns:     nothing
 *=======================================================================*/

bool_t
verifyName(const char* name, enum verifyName_type type, bool_t abortOnError)
{
    bool_t result;
    unsigned short length = (unsigned short)strlen(name);

    if (length > 0) {
        if (name[0] == '<') {
            result = (type == LegalMethod) && 
                ((strcmp(name, "<init>") == 0) || 
                 (strcmp(name, "<clinit>") == 0));
        } else {
            const char *p;
            if (type == LegalClass && name[0] == '[') {
                p = skipOverFieldType(name, FALSE, length);
            } else {
                p = skipOverFieldName(name, 
                                      type == LegalClass,
                                      length);
            }
            result = (p != NULL) && (p - name == length);
        }
    } else {
        result = FALSE;
    }
    if (!result && abortOnError) { 
        fatalError(KVM_MSG_BAD_NAME);
    } 
    return result;
}

/*=========================================================================
 * FUNCTION:      verifyClassFlags()
 * TYPE:          private class file load operation
 * OVERVIEW:      validate class access flags
 * INTERFACE:
 *   parameters:  class access flags
 *   returns:     nothing
 *=======================================================================*/

static void 
verifyClassFlags(unsigned short flags)
{
    if (flags & ACC_INTERFACE) {
        if ((flags & ACC_ABSTRACT) == 0)
            goto failed;
        if (flags & ACC_FINAL)
            goto failed;
    } else {
        if ((flags & ACC_FINAL) && (flags & ACC_ABSTRACT))
            goto failed;
    }
    return;
    
 failed:
    fatalError(KVM_MSG_BAD_CLASS_ACCESS_FLAGS);
}

/*=========================================================================
 * FUNCTION:      verifyFieldFlags()
 * TYPE:          private class file load operation
 * OVERVIEW:      validate field access flags
 * INTERFACE:
 *   parameters:  field access flags, isInterface
 *   returns:     nothing
 *=======================================================================*/

static void 
verifyFieldFlags(unsigned short flags, unsigned short classFlags)
{
    if ((classFlags & ACC_INTERFACE) == 0) {
        /* Class or instance fields */
        int accessFlags = flags & (ACC_PUBLIC | ACC_PRIVATE | ACC_PROTECTED);

        /* Make sure that accessFlags has one of the four legal values, by
         * looking it up in a bit mask */
        if (( (1 << accessFlags) & ((1 << 0) + (1 << ACC_PUBLIC) 
             + (1 << ACC_PRIVATE) + (1 << ACC_PROTECTED))) == 0) {
            goto failed;
        }

        if ((flags & (ACC_FINAL | ACC_VOLATILE)) == (ACC_FINAL | ACC_VOLATILE)){
            /* A field can't be both final and volatile */
            goto failed;
        }
    } else { 
        /* interface fields */
        if (flags != (ACC_STATIC | ACC_FINAL | ACC_PUBLIC)) { 
            goto failed;
        }
    }
    return;
    
 failed:
    fatalError(KVM_MSG_BAD_FIELD_ACCESS_FLAGS);
}

/*=========================================================================
 * FUNCTION:      verifyFieldType()
 * TYPE:          private class file load operation
 * OVERVIEW:      validate field signature
 * INTERFACE:
 *   parameters:  field signature
 *   returns:     nothing
 *=======================================================================*/

static void 
verifyFieldType(const char* type)
{
    unsigned short length = (unsigned short)strlen(type);
    const char *p = skipOverFieldType(type, FALSE, length);

    if (p == NULL || p - type != length) {
        fatalError(KVM_MSG_BAD_FIELD_SIGNATURE);
    }
}

/*=========================================================================
 * FUNCTION:      verifyMethodFlags()
 * TYPE:          private class file load operation
 * OVERVIEW:      validate method access flags
 * INTERFACE:
 *   parameters:  method access flags, isInterface, methodName
 *   returns:     nothing
 *=======================================================================*/

static void 
verifyMethodFlags(unsigned short flags, unsigned short classFlags,
                  const char* name)
{
    /* These are all small bits.  The value is between 0 and 7. */
    int accessFlags = flags & (ACC_PUBLIC | ACC_PRIVATE | ACC_PROTECTED);

    /* Make sure that accessFlags has one of the four legal values, by
     * looking it up in a bit mask */

    if (( (1 << accessFlags) 
         & ((1 << 0) 
         | (1 << ACC_PUBLIC) 
         | (1 << ACC_PRIVATE) | (1 << ACC_PROTECTED))) == 0) {
        goto failed;
    }

    if ((classFlags & ACC_INTERFACE) == 0) {
        /* class or instance methods */
        if (flags & ACC_ABSTRACT) {
            if (flags & (ACC_FINAL | ACC_NATIVE | ACC_SYNCHRONIZED
                | ACC_PRIVATE | ACC_STATIC)) { 
                goto failed;
            }
        }
    } else { 
        /* interface methods */
        if ( (flags & (ACC_ABSTRACT | ACC_PUBLIC | ACC_STATIC)) 
              != (ACC_ABSTRACT | ACC_PUBLIC)) { 
            /* Note that <clinit> is special, and not handled by this
             * function.  It's not abstract, and static. */
            goto failed;            
        }
    }

    if (strcmp(name, "<init>") == 0) {
        if (flags & ~(ACC_PUBLIC | ACC_PROTECTED | ACC_PRIVATE))
            goto failed;
    } 
    return;
    
 failed:
    fatalError(KVM_MSG_BAD_METHOD_ACCESS_FLAGS);
}

/*=========================================================================
 * FUNCTION:      verifyMethodType()
 * TYPE:          private class file load operation
 * OVERVIEW:      validate method signature
 * INTERFACE:
 *   parameters:  method name, signature
 *   returns:     argument size
 *=======================================================================*/

static unsigned short
verifyMethodType(const char *name, const char* signature)
{
    unsigned short args_size = 0;
    const char *p = signature;
    unsigned short length = (unsigned short)strlen(signature);
    const char *next_p;

    /* The first character must be a '(' */
    if ((length > 0) && (*p++ == '(')) {
        length--;
        /* Skip over however many legal field signatures there are */
        while ((length > 0) &&
               (next_p = skipOverFieldType(p, FALSE, length))) {
            args_size++;
            if (p[0] == 'J' || p[0] == 'D')
                args_size++;
            length -= (next_p - p);
            p = next_p;
        }
        /* The first non-signature thing better be a ')' */
        if ((length > 0) && (*p++ == ')')) {
            length --;
            if (strlen(name) > 0 && name[0] == '<') {
                /* All internal methods must return void */
                if ((length == 1) && (p[0] == 'V'))
                    return args_size;
            } else {
                /* Now, we better just have a return value. */
                next_p =  skipOverFieldType(p, TRUE, length);
                if (next_p && (length == next_p - p))
                    return args_size;
            }
        }
    }
    fatalError(KVM_MSG_BAD_METHOD_SIGNATURE);
    return 0; /* never reached */
}

/*=========================================================================
 * FUNCTION:      verifyConstantPoolEntry()
 * TYPE:          private class file load operation
 * OVERVIEW:      validate constant pool index
 * INTERFACE:
 *   parameters:  constant pool, index, and expected tag
 *   returns:     nothing
 *=======================================================================*/

static void
verifyConstantPoolEntry(INSTANCE_CLASS CurrentClass,
                        unsigned short index,
                        unsigned char tag)
{
    CONSTANTPOOL ConstantPool = CurrentClass->constPool;
    unsigned short length = (unsigned short)CONSTANTPOOL_LENGTH(ConstantPool);
    unsigned char tag2 = CONSTANTPOOL_TAGS(ConstantPool)[index];
    if (index >= length || tag2 != tag) {
        fatalError(KVM_MSG_BAD_CONSTANT_INDEX);
    }
}

/*=========================================================================
 * Class loading operations
 *=======================================================================*/

/*=========================================================================
 * FUNCTION:      loadVersionInfo()
 * TYPE:          private class file load operation
 * OVERVIEW:      Load the first few bytes of a Java class file,
 *                checking the file type and version information.
 * INTERFACE:
 *   parameters:  classfile pointer
 *   returns:     <nothing>
 *=======================================================================*/

static void 
loadVersionInfo(FILEPOINTER_HANDLE ClassFileH)
{
    UNUSEDPARAMETER(CurrentClass)
    long magic;

#if INCLUDEDEBUGCODE
    if (traceclassloadingverbose) {
        fprintf(stdout, "Loading version information\n");
    }
#endif /* INCLUDEDEBUGCODE */

    magic = loadCell(ClassFileH);
    if (magic != 0xCAFEBABE) {
        /*  This should actually be an exception rather than a fatal error */
        /*  but it is unlikely that we would be able to continue execution */
        fatalError(KVM_MSG_NOT_A_JAVA_CLASSFILE);
    }

    skipBytes(ClassFileH, 4);    /* ignore minorVersion and majorVersion */
}

/*=========================================================================
 * FUNCTION:      loadConstantPool()
 * TYPE:          private class file load operation
 * OVERVIEW:      Load the constant pool part of a Java class file,
 *                building the runtime class constant pool structure
 *                during the loading process.
 * INTERFACE:
 *   parameters:  classfile pointer
 *   returns:     constant pool pointer
 *=======================================================================*/

static POINTERLIST
loadConstantPool(FILEPOINTER_HANDLE ClassFileH, INSTANCE_CLASS CurrentClass)
{
    unsigned short constantCount = loadShort(ClassFileH);
    
    CONSTANTPOOL ConstantPool;
    int cpIndex;
    POINTERLIST result;
    int lastNonUtfIndex = -1;

#if INCLUDEDEBUGCODE
    if (traceclassloadingverbose) { 
        fprintf(stdout, "Loading constant pool\n");
    }
#endif /* INCLUDEDEBUGCODE */

    START_TEMPORARY_ROOTS
        DECLARE_TEMPORARY_ROOT(CONSTANTPOOL_ENTRY, RawPool,
            (CONSTANTPOOL_ENTRY)mallocBytes(constantCount * (1 + sizeof(*RawPool))));
        DECLARE_TEMPORARY_ROOT(POINTERLIST, StringPool, 
            (POINTERLIST)callocObject(SIZEOF_POINTERLIST(constantCount), GCT_POINTERLIST));
        StringPool->length = constantCount;
#define RAW_POOL(i)      (RawPool[i])
#define CP_ENTRY(i)      (ConstantPool->entries[i])

        /*  Read the constant pool entries from the class file */
        for (cpIndex = 1; cpIndex < constantCount; cpIndex++) { 
            unsigned char tag = loadByte(ClassFileH);
            unsigned char *Tags = (unsigned char *)(&RawPool[constantCount]);
            Tags[cpIndex] = tag;

            switch (tag) {
            case CONSTANT_String: 
            case CONSTANT_Class: { 
                /* A single 16-bit entry that points to a UTF string */
                unsigned short nameIndex = loadShort(ClassFileH);
                RAW_POOL(cpIndex).integer = nameIndex;
                break;
            }

            case CONSTANT_Fieldref: 
            case CONSTANT_Methodref: 
            case CONSTANT_InterfaceMethodref: { 
                /* Two 16-bit entries */
                unsigned short classIndex = loadShort(ClassFileH);
                unsigned short nameTypeIndex = loadShort(ClassFileH);
                RAW_POOL(cpIndex).method.classIndex = classIndex;
                RAW_POOL(cpIndex).method.nameTypeIndex = nameTypeIndex;
                break;
            }

            case CONSTANT_Float: 
#if !IMPLEMENTS_FLOAT
                fatalError(KVM_MSG_FLOATING_POINT_NOT_SUPPORTED);
#endif
            case CONSTANT_Integer: 
            { 
                /* A signal 32-bit value */
                long value = loadCell(ClassFileH);
                RAW_POOL(cpIndex).integer = value;
                break;
            }

            case CONSTANT_Double:
#if !IMPLEMENTS_FLOAT
                fatalError(KVM_MSG_FLOATING_POINT_NOT_SUPPORTED);
#endif
            case CONSTANT_Long: 
                /* A 64-bit value */
                RAW_POOL(cpIndex).integer = loadCell(ClassFileH);
                cpIndex++;
                if (cpIndex >= constantCount) {
                    fatalError(KVM_MSG_BAD_64BIT_CONSTANT);
                }
                RAW_POOL(cpIndex).integer = loadCell(ClassFileH);
                break;
 
            case CONSTANT_NameAndType: { 
                /* Like Fieldref, etc */
                unsigned short nameIndex = loadShort(ClassFileH);
                unsigned short typeIndex = loadShort(ClassFileH);
                /* In the second pass, below, these will be replaced with the
                 * actual nameKey and typeKey.  Currently, they are indices
                 * to items in the constant pool that may not yet have been
                 * loaded */
                RAW_POOL(cpIndex).nameTypeKey.nt.nameKey = nameIndex;
                RAW_POOL(cpIndex).nameTypeKey.nt.typeKey = typeIndex;
                break;
            }

            case CONSTANT_Utf8: {
                unsigned short length = loadShort(ClassFileH);
                /* This allocation may invalidate ClassFile */
                char *string = mallocBytes(length + 1);
                STRING_POOL(cpIndex) =  string;
                loadBytes(ClassFileH, string, length);
                string[length] = '\0';

                verifyUTF8String(string, length);
            }
            break;
            
            default: 
                fatalError(KVM_MSG_INVALID_CONSTANT_POOL_ENTRY);
                break;
            }
            
            if (tag != CONSTANT_Utf8) { 
                lastNonUtfIndex = cpIndex;
            }
        }

        { 
            int numberOfEntries = lastNonUtfIndex + 1;
            int tableSize = 
                numberOfEntries + ((numberOfEntries + (CELL - 1)) >> log2CELL);
#if USESTATIC
            IS_TEMPORARY_ROOT(ConstantPool, 
                (CONSTANTPOOL)callocObject(tableSize, GCT_NOPOINTERS));
#else 
            ConstantPool = (CONSTANTPOOL)callocPermanentObject(tableSize);
#endif
            CONSTANTPOOL_LENGTH(ConstantPool) = numberOfEntries;
            CurrentClass->constPool = ConstantPool;
        }

        /* Now create the constant pool entries */
        for (cpIndex = 1; cpIndex < constantCount; cpIndex++) { 
            /* These can move between iterations of the loop */
            unsigned char *Tags = (unsigned char *)(&RawPool[constantCount]);
            unsigned char *CPTags = CONSTANTPOOL_TAGS(ConstantPool);

            unsigned char tag = Tags[cpIndex];
            if (cpIndex <= lastNonUtfIndex) {
                CPTags[cpIndex] = tag;
            }

            switch (tag) {

            case CONSTANT_Class: {
                unsigned short nameIndex = 
                    (unsigned short)RAW_POOL(cpIndex).integer;
                START_TEMPORARY_ROOTS
                    DECLARE_TEMPORARY_ROOT(const char *, name, 
                        getUTF8String(&StringPool, nameIndex));
                    verifyName(name, LegalClass, TRUE);
                    CP_ENTRY(cpIndex).clazz = 
                        getRawClassX(&name, 0, strlen(name));
                END_TEMPORARY_ROOTS
                break;
            }

            case CONSTANT_String: { 
                unsigned short nameIndex = 
                    (unsigned short)RAW_POOL(cpIndex).integer;
                char *name = getUTF8String(&StringPool, nameIndex);
                INTERNED_STRING_INSTANCE string = 
                    internString(name, strlen(name));
                CP_ENTRY(cpIndex).String = string;
                break;
            }
            case CONSTANT_Fieldref: 
            case CONSTANT_Methodref: 
            case CONSTANT_InterfaceMethodref: { 
                /* Two 16-bit entries */
                unsigned short classIndex = RAW_POOL(cpIndex).method.classIndex;
                unsigned short nameTypeIndex =RAW_POOL(cpIndex).method.nameTypeIndex;
                if (classIndex >= constantCount ||
                    Tags[classIndex] != CONSTANT_Class ||
                    nameTypeIndex >= constantCount ||
                    Tags[nameTypeIndex] != CONSTANT_NameAndType) {
                    fatalError(KVM_MSG_BAD_FIELD_OR_METHOD_REFERENCE);
                } else {
                    unsigned short typeIndex = 
                        RAW_POOL(nameTypeIndex).nameTypeKey.nt.typeKey;
                    char* type = getUTF8String(&StringPool, typeIndex);
                    if (tag == CONSTANT_Fieldref && type[0] == '(') {
                        fatalError(KVM_MSG_BAD_NAME_OR_TYPE_REFERENCE);
                    }
                    CP_ENTRY(cpIndex) = RAW_POOL(cpIndex);
                }
                break;
            }

            case CONSTANT_Double:
#if !IMPLEMENTS_FLOAT
                fatalError(KVM_MSG_FLOATING_POINT_NOT_SUPPORTED);
#endif
            case CONSTANT_Long: 
                CP_ENTRY(cpIndex).integer = RAW_POOL(cpIndex).integer;
                cpIndex++; 
                CPTags[cpIndex] = 0;
                CP_ENTRY(cpIndex).integer = RAW_POOL(cpIndex).integer;
                break;

            case CONSTANT_Float: 
#if !IMPLEMENTS_FLOAT
                fatalError(KVM_MSG_FLOATING_POINT_NOT_SUPPORTED);
#endif
            case CONSTANT_Integer: 
                CP_ENTRY(cpIndex).integer = RAW_POOL(cpIndex).integer;
                break;

            case CONSTANT_NameAndType: { 
                unsigned short nameIndex = RAW_POOL(cpIndex).nameTypeKey.nt.nameKey;
                unsigned short typeIndex = RAW_POOL(cpIndex).nameTypeKey.nt.typeKey;
                START_TEMPORARY_ROOTS
                    DECLARE_TEMPORARY_ROOT(const char *, name, 
                        getUTF8String(&StringPool, nameIndex));
                    DECLARE_TEMPORARY_ROOT(const char *, type, 
                        getUTF8String(&StringPool, typeIndex));
                    NameKey nameKey;
                    unsigned short typeKey;
                    if (type[0] == '(') {
                        verifyName(name, LegalMethod, TRUE);
                        verifyMethodType(name, type);
                        typeKey = change_MethodSignature_to_Key(&type, 0, 
                                                                strlen(type)); 
                    } else {
                        verifyName(name, LegalField, TRUE);
                        verifyFieldType(type);
                        typeKey = change_FieldSignature_to_Key(&type, 0, 
                                                               strlen(type));
                    }
                    nameKey = change_Name_to_Key(&name, 0, strlen(name));
                    CP_ENTRY(cpIndex).nameTypeKey.nt.nameKey = nameKey;
                    CP_ENTRY(cpIndex).nameTypeKey.nt.typeKey = typeKey;
                 END_TEMPORARY_ROOTS
                break;
            }

            case CONSTANT_Utf8:
                /* We don't need these after loading time.  So why bother */
                if (cpIndex <= lastNonUtfIndex) {
                    CP_ENTRY(cpIndex).integer = 0;
                    CPTags[cpIndex] = 0;
                }
                break;

            default: 
                fatalVMError(KVM_MSG_INVALID_CONSTANT_POOL_ENTRY);
                break;
            }
        }
        result = StringPool;
    END_TEMPORARY_ROOTS
    return result;
}
        
/*=========================================================================
 * FUNCTION:      loadClassInfo()
 * TYPE:          private class file load operation
 * OVERVIEW:      Load the access flag, thisClass and superClass
 *                parts of a Java class file.
 * INTERFACE:
 *   parameters:  classfile pointer, current constant pool pointer
 *   returns:     Pointer to class runtime structure
 *=======================================================================*/

static void
loadClassInfo(FILEPOINTER_HANDLE ClassFileH, INSTANCE_CLASS CurrentClass)
{
    INSTANCE_CLASS thisClass;
    INSTANCE_CLASS superClass;
    CONSTANTPOOL ConstantPool = CurrentClass->constPool;

    unsigned short accessFlags = loadShort(ClassFileH) & RECOGNIZED_CLASS_FLAGS;

    verifyClassFlags(accessFlags);
    
    {
        unsigned short thisClassIndex = loadShort(ClassFileH);
        verifyConstantPoolEntry(CurrentClass, thisClassIndex, CONSTANT_Class);
        thisClass = (INSTANCE_CLASS)CP_ENTRY(thisClassIndex).clazz;

        if (CurrentClass != thisClass) {
            NoClassDefFoundError(CurrentClass);
        }
    }
    {
        unsigned short superClassIndex = loadShort(ClassFileH);
        if (superClassIndex == 0) {
            superClass = NULL;
        } else {
            verifyConstantPoolEntry(CurrentClass, superClassIndex, 
                                    CONSTANT_Class);
            superClass = (INSTANCE_CLASS)CP_ENTRY(superClassIndex).clazz;
        }
    }

#if INCLUDEDEBUGCODE
    if (traceclassloadingverbose) { 
        fprintf(stdout, "Loading class info\n");
    }
#endif /* INCLUDEDEBUGCODE */

    if ((superClass == NULL && (ROMIZING || CurrentClass != JavaLangObject))
       || (superClass != JavaLangObject && (accessFlags & ACC_INTERFACE))
    ) {
        fatalError(KVM_MSG_BAD_SUPERCLASS);
    }
    CurrentClass->superClass = superClass;
    CurrentClass->clazz.accessFlags = accessFlags;

#if INCLUDEDEBUGCODE
    if (traceclassloadingverbose) 
        fprintf(stdout, "Class info loaded ok\n");
#endif /* INCLUDEDEBUGCODE */

}

/*=========================================================================
 * FUNCTION:      loadInterfaces()
 * TYPE:          private class file load operation
 * OVERVIEW:      Load the interface part of a Java class file.
 * INTERFACE:
 *   parameters:  classfile pointer, current class pointer
 *   returns:     <nothing>
 *=======================================================================*/

static void 
loadInterfaces(FILEPOINTER_HANDLE ClassFileH, INSTANCE_CLASS CurrentClass)
{
    unsigned short interfaceCount = loadShort(ClassFileH);
    long byteSize = (interfaceCount+1) * sizeof(unsigned short);
    unsigned int ifIndex;

#if INCLUDEDEBUGCODE
    if (traceclassloadingverbose) { 
        fprintf(stdout, "Loading interfaces\n");
    }
#endif /* INCLUDEDEBUGCODE */

    if (interfaceCount == 0) { 
        return;
    }

    if (USESTATIC) { 
        CurrentClass->ifaceTable = (unsigned short *)mallocBytes(byteSize);
    } else {
        long cellSize = ByteSizeToCellSize(byteSize);
        CurrentClass->ifaceTable =
            (unsigned short*)callocPermanentObject(cellSize);
    }

    /*  Store length in the first slot */
    CurrentClass->ifaceTable[0] = interfaceCount;

    for (ifIndex = 1; ifIndex <= interfaceCount; ifIndex++) {
        unsigned short cpIndex = loadShort(ClassFileH);
        verifyConstantPoolEntry(CurrentClass, cpIndex, CONSTANT_Class);
        CurrentClass->ifaceTable[ifIndex] = cpIndex;
    }

#if INCLUDEDEBUGCODE
    if (traceclassloadingverbose) {
        fprintf(stdout, "Interfaces loaded ok\n");
    }
#endif /* INCLUDEDEBUGCODE */

}

/*=========================================================================
 * FUNCTION:      loadStaticFieldAttributes()
 * TYPE:          private class file load operation
 * OVERVIEW:      Load the "ConstantValue" attribute of a static field,
 *                ignoring all the other possible field attributes.
 * INTERFACE:
 *   parameters:  classfile pointer, constant pool pointer, 
 *                field attribute count, pointer to the runtime field struct
 *   returns:     <nothing>
 * 
 * COMMENTS: this function sets the u.offset field of the field to the
 *           constant pool entry that should contain its initial value, or
 *           to 0 if there is no initial value.
 *=======================================================================*/

static void 
loadStaticFieldAttributes(FILEPOINTER_HANDLE ClassFileH, 
                          INSTANCE_CLASS CurrentClass, 
                          FIELD thisField, 
                          POINTERLIST_HANDLE StringPoolH)
{
    int cpIndex = 0;
    unsigned short attrCount = loadShort(ClassFileH);
    unsigned short attrIndex;
    
    /*  See if the field has any attributes in the class file */
    for (attrIndex = 0; attrIndex < attrCount; attrIndex++) {
        unsigned short attrNameIndex = loadShort(ClassFileH);
        unsigned long  attrLength    = loadCell(ClassFileH);
        const char*    attrName     = getUTF8String(StringPoolH, attrNameIndex);
        
        /*  Check if the attribute represents a constant value index */
        if (!strcmp(attrName, "ConstantValue")) {
            if (attrLength != 2) {
                fatalError(KVM_MSG_BAD_CONSTANTVALUE_LENGTH);
            }
            if (cpIndex != 0) { 
                fatalError(KVM_MSG_DUPLICATE_CONSTANTVALUE_ATTRIBUTE);
            }
            /*  Read index to a constant in constant pool */
            cpIndex = loadShort(ClassFileH);
            if (cpIndex == 0) { 
                fatalError(KVM_MSG_BAD_CONSTANT_INDEX);
            }
        } else {
            /*  Unrecognized attribute; read the bytes to /dev/null */
            skipBytes(ClassFileH, attrLength);
        }
    }
    thisField->u.offset = cpIndex;
}

/*=========================================================================
 * FUNCTION:      loadFields()
 * TYPE:          private class file load operation
 * OVERVIEW:      Load the fields (static & instance variables) defined 
 *                in a Java class file.
 * INTERFACE:
 *   parameters:  classfile pointer, current class pointer
 *   returns:     <nothing>
 *=======================================================================*/

static void 
loadFields(FILEPOINTER_HANDLE ClassFileH, INSTANCE_CLASS CurrentClass, 
           POINTERLIST_HANDLE StringPoolH)
{
    unsigned short fieldCount  = loadShort(ClassFileH);
    int fieldTableSize = SIZEOF_FIELDTABLE(fieldCount);
    FIELDTABLE fieldTable;
    unsigned int index;

    int staticPtrCount = 0;
    int staticNonPtrCount = 0;
    
#if INCLUDEDEBUGCODE
    if (traceclassloadingverbose) { 
        fprintf(stdout, "Loading fields\n");
    } 
#endif /* INCLUDEDEBUGCODE */

    if (fieldCount == 0) { 
        return;
    }

    /*  Create a new field table */
#if USESTATIC
    fieldTable = (FIELDTABLE)callocObject(fieldTableSize, GCT_NOPOINTERS);
#else
    fieldTable = (FIELDTABLE)callocPermanentObject(fieldTableSize);
#endif

    /*  Store the size of the table at the beginning of the table */
    fieldTable->length = fieldCount;
    CurrentClass->fieldTable = fieldTable;

    for (index = 0; index < fieldCount; index++) { 
        unsigned short accessFlags = 
            loadShort(ClassFileH) & RECOGNIZED_FIELD_FLAGS;
        unsigned short nameIndex   = loadShort(ClassFileH);
        unsigned short typeIndex   = loadShort(ClassFileH);
        bool_t isStatic = (accessFlags & ACC_STATIC) != 0;
        FIELD thisField;

        START_TEMPORARY_ROOTS
            DECLARE_TEMPORARY_ROOT(const char *, fieldName, 
                                   getUTF8String(StringPoolH, nameIndex));
            DECLARE_TEMPORARY_ROOT(const char *, signature, 
                                   getUTF8String(StringPoolH, typeIndex));
            NameTypeKey result;

            verifyFieldFlags(accessFlags, CurrentClass->clazz.accessFlags);
            verifyName(fieldName, LegalField, TRUE);
            verifyFieldType(signature);

            result.nt.nameKey = 
                change_Name_to_Key(&fieldName, 0, strlen(fieldName));
            result.nt.typeKey = 
                change_FieldSignature_to_Key(&signature, 0, strlen(signature));
    
            ASSERTING_NO_ALLOCATION
                thisField = &CurrentClass->fieldTable->fields[index];

#if INCLUDEDEBUGCODE
            if (traceclassloadingverbose) { 
                fprintf(stdout, "Field '%s' loaded\n", fieldName);
            }
#endif /* INCLUDEDEBUGCODE */

                /* Check if the field is double length, or is a pointer type.
                 * If so set the appropriate bit in the word */
                switch (signature[0]) { 
                   case 'D': case 'J':   accessFlags |= ACC_DOUBLE;   break;
                     case 'L': case '[':   accessFlags |= ACC_POINTER;  break;
                }

                /*  Store the corresponding information in the structure */
                thisField->nameTypeKey = result;
                thisField->ofClass     = CurrentClass;
                thisField->accessFlags = accessFlags;

                if (isStatic) { 
                    loadStaticFieldAttributes(ClassFileH, CurrentClass,
                                              thisField, StringPoolH);
                    if (accessFlags & ACC_POINTER) { 
                        staticPtrCount++;
                    } else {
                        staticNonPtrCount += (accessFlags & ACC_DOUBLE) ? 2 : 1;
                    } 
                } else { 
                    ignoreAttributes(ClassFileH, StringPoolH);
                }
            END_ASSERTING_NO_ALLOCATION
        END_TEMPORARY_ROOTS
    }

    /* We know go back and look at each of the static fields again. */
    if (staticPtrCount > 0 || staticNonPtrCount > 0) { 
        /* We put all the statics into a POINTERLIST.  We specifically make
         * a POINTERLIST in which the real length is longer than the value
         * put into the length field.  The garbage collector only will look
         * at the first "length" fields.
         * So all the pointer statics go before all the non-pointer statics.
         *
         * As a hack, loadStaticFieldAttributes() has put into the offset
         * field the constant pool entry containing the static's initial
         * value.  The static field should be initialized appropriately
         * once space for it has been allocated 
         */

        /* Allocate space for all the pointers and non pointers.  
         */
        int staticsSize = SIZEOF_POINTERLIST(staticNonPtrCount+staticPtrCount);
        POINTERLIST statics = (POINTERLIST)callocPermanentObject(staticsSize);
        /* All the non-pointers go after all the pointers */
        void **nextPtrField = (void **)statics->data;
        void **nextNonPtrField = nextPtrField + staticPtrCount;

        /* Set the length field so that the GC only looks at the pointers */
        statics->length = staticPtrCount;
        CurrentClass->staticFields = statics;

        ASSERTING_NO_ALLOCATION
            CONSTANTPOOL ConstantPool = CurrentClass->constPool;
            if (USESTATIC) { 
                /* Otherwise, this is in permanent memory and won't move */
                fieldTable = CurrentClass->fieldTable;
            } 

            FOR_EACH_FIELD(thisField, fieldTable) 
                long accessFlags = thisField->accessFlags;
                unsigned short cpIndex;
                if (!(accessFlags & ACC_STATIC)) { 
                    continue;
                }
                cpIndex = (unsigned short)(thisField->u.offset);
                if (thisField->accessFlags & ACC_POINTER) { 
                    /* The only possible initialization is for a string */
                    thisField->u.staticAddress = nextPtrField;
                    if (cpIndex != 0) { 
                        verifyConstantPoolEntry(CurrentClass, cpIndex,
                                                CONSTANT_String);
                        *(INTERNED_STRING_INSTANCE *)nextPtrField = 
                            CP_ENTRY(cpIndex).String;
                    }
                    nextPtrField++;
                } else {
                    thisField->u.staticAddress = nextNonPtrField;
                    if (cpIndex != 0) { 
                        unsigned char tag;
                        switch(thisField->nameTypeKey.nt.typeKey) { 
                            case 'B': case 'C': case 'Z': case 'S': case 'I':
                                tag = CONSTANT_Integer;
                                break;
                            case 'F':
#if !IMPLEMENTS_FLOAT
                                fatalError(KVM_MSG_FLOATING_POINT_NOT_SUPPORTED);
#endif
                                tag = CONSTANT_Float;
                                break;
                            case 'D':
#if !IMPLEMENTS_FLOAT
                                fatalError(KVM_MSG_FLOATING_POINT_NOT_SUPPORTED);
#endif
                                tag = CONSTANT_Double;
                                break;
                            case 'J':
                                tag = CONSTANT_Long;
                                break;
                            default:
                                fatalError(KVM_MSG_BAD_SIGNATURE);
                        }
                        verifyConstantPoolEntry(CurrentClass, cpIndex, tag);
                        if (accessFlags & ACC_DOUBLE) {                 
                            /* Initialize a double or long */
                            CONSTANTPOOL_ENTRY thisEntry = &CP_ENTRY(cpIndex);
                            unsigned long hiBytes, loBytes;
                            hiBytes = (unsigned long)(thisEntry[0].integer);
                            loBytes = (unsigned long)(thisEntry[1].integer);
                            SET_LONG_FROM_HALVES(nextNonPtrField, 
                                                 hiBytes, loBytes);
                        } else { 
                            *(cell *)nextNonPtrField = 
                               CP_ENTRY(cpIndex).integer;
                        }                       
                    }
                    nextNonPtrField += (accessFlags & ACC_DOUBLE) ? 2 : 1;
                }
            END_FOR_EACH_FIELD
        END_ASSERTING_NO_ALLOCATION
    }

    if (fieldCount >= 2) { 
        if (USESTATIC) { 
            /* Otherwise, this is in permanent memory and won't move */
            fieldTable = CurrentClass->fieldTable;
        }
        ASSERTING_NO_ALLOCATION
            /* Check to see if there are two fields with the same name/type */
            FIELD firstField = &fieldTable->fields[0];
            FIELD lastField = firstField + (fieldCount - 1);
            FIELD outer, inner;
            for (outer = firstField; outer < lastField; outer++) { 
                for (inner = outer + 1; inner <= lastField; inner++) { 
                    if (outer->nameTypeKey.i == inner->nameTypeKey.i) { 
                        fatalError(KVM_MSG_DUPLICATE_FIELD_FOUND);
                    }
                }
            }
        END_ASSERTING_NO_ALLOCATION
    }

#if INCLUDEDEBUGCODE
    if (traceclassloadingverbose)
        fprintf(stdout, "Fields loaded ok\n");
#endif /* INCLUDEDEBUGCODE */

}

/*=========================================================================
 * FUNCTION:      loadExceptionHandlers()
 * TYPE:          private class file load operation
 * OVERVIEW:      Load the exception handling information associated
 *                with each method in a class file. 
 * INTERFACE:
 *   parameters:  constant pool, classfile pointer, method pointer
 *   returns:     number of characters read from the class file
 *=======================================================================*/

static int 
loadExceptionHandlers(FILEPOINTER_HANDLE ClassFileH, METHOD_HANDLE thisMethodH)
{
    unsigned short numberOfHandlers = loadShort(ClassFileH);
    if (numberOfHandlers > 0) {
        HANDLERTABLE handlerTable;
        METHOD thisMethod;
        int tableSize = SIZEOF_HANDLERTABLE(numberOfHandlers);
#if USESTATIC
        handlerTable = (HANDLERTABLE)callocObject(tableSize, GCT_NOPOINTERS);
#else
        handlerTable = (HANDLERTABLE)callocPermanentObject(tableSize);
#endif
        handlerTable->length = numberOfHandlers;
        thisMethod = unhand(thisMethodH);
        thisMethod->u.java.handlers = handlerTable;

        ASSERTING_NO_ALLOCATION
            FOR_EACH_HANDLER(thisHandler, handlerTable) 
                unsigned short startPC   = loadShort(ClassFileH);
                unsigned short endPC     = loadShort(ClassFileH);
                unsigned short handlerPC = loadShort(ClassFileH);
                unsigned short exception = loadShort(ClassFileH);

                if (startPC >= thisMethod->u.java.codeLength ||
                    endPC > thisMethod->u.java.codeLength || 
                    startPC >= endPC ||
                    handlerPC >= thisMethod->u.java.codeLength) {
                    fatalError(KVM_MSG_BAD_EXCEPTION_HANDLER_FOUND);
                }
                if (exception != 0) {
                    verifyConstantPoolEntry(thisMethod->ofClass,
                                            exception, CONSTANT_Class);
                }
                thisHandler->startPC   = startPC;
                thisHandler->endPC     = endPC;
                thisHandler->handlerPC = handlerPC;
                thisHandler->exception = exception;
            END_FOR_EACH_HANDLER
        END_ASSERTING_NO_ALLOCATION
    } else {
        /*  Method has no associated exception handlers */
        unhand(thisMethodH)->u.java.handlers = NULL;
    }
    return (numberOfHandlers * 8 + 2);
}

/*=========================================================================
 * FUNCTION:      loadStackMaps()
 * TYPE:          private class file load operation
 * OVERVIEW:      Load the stack maps associated
 *                with each method in a class file. 
 * INTERFACE:
 *   parameters:  classfile pointer, method pointer, constant pool
 *   returns:     number of characters read from the class file
 *=======================================================================*/

static long
loadStackMaps(FILEPOINTER_HANDLE ClassFileH, METHOD_HANDLE thisMethodH)
{
    long bytesRead;
    INSTANCE_CLASS CurrentClass = unhand(thisMethodH)->ofClass;
    START_TEMPORARY_ROOTS
        unsigned short nStackMaps = loadShort(ClassFileH);
        DECLARE_TEMPORARY_ROOT(POINTERLIST, stackMaps, 
            (POINTERLIST)callocObject(SIZEOF_POINTERLIST(2*nStackMaps), 
                                      GCT_POINTERLIST));
        METHOD thisMethod = unhand(thisMethodH); /* Very volatile */
        unsigned tempSize = (thisMethod->u.java.maxStack + thisMethod->frameSize + 2);
        DECLARE_TEMPORARY_ROOT(unsigned short*, stackMap, 
           (unsigned short *)mallocBytes(sizeof(unsigned short) * tempSize));
        unsigned short stackMapIndex;

        stackMaps->length = nStackMaps;
        unhand(thisMethodH)->u.java.stackMaps.verifierMap = stackMaps;
        bytesRead = 2;

        for (stackMapIndex = 0; stackMapIndex < nStackMaps; stackMapIndex++) {
            unsigned short i, index;
            /* Any allocation happens at the end, so we have to dereference this
             * at least once through each loop. 
             */
            thisMethod = unhand(thisMethodH);
            /* Read in the offset */
            stackMaps->data[stackMapIndex + nStackMaps].cell = 
                loadShort(ClassFileH);
            bytesRead += 2;
            for (index = 0, i = 0 ; i < 2; i++) {
                unsigned short j;
                unsigned short size = loadShort(ClassFileH);
                unsigned short size_delta = 0;
                unsigned short size_index = index++;
                unsigned short maxSize = (i == 0 ? thisMethod->frameSize 
                                                 : thisMethod->u.java.maxStack);
                bytesRead += 2;
                for (j = 0; j < size; j++) {
                    unsigned char stackType = loadByte(ClassFileH);
                    bytesRead += 1;

                    /* We are reading the j-th element of the stack map.
                     * This corresponds to the value in the j + size_delta'th
                     * local register or stack location 
                     */
                    if (j + size_delta >= maxSize) { 
                        fatalError(KVM_MSG_BAD_STACKMAP);
                    } else if (stackType == ITEM_NewObject) {
                        unsigned short instr = loadShort(ClassFileH);
                        bytesRead += 2;
                        if (instr >= thisMethod->u.java.codeLength) {
                            fatalError(KVM_MSG_BAD_NEWOBJECT);
                        }
                        stackMap[index++] = ENCODE_NEWOBJECT(instr);
                    } else if (stackType < ITEM_Object) {
                        stackMap[index++] = stackType;
                        if (stackType == ITEM_Long || stackType == ITEM_Double){
                            if (j + size_delta + 1 >= maxSize) { 
                                fatalError(KVM_MSG_BAD_STACKMAP);
                            }
                            stackMap[index++] = (stackType == ITEM_Long) 
                                                    ? ITEM_Long_2 
                                                    : ITEM_Double_2;
                            size_delta++;
                        }
                    } else if (stackType == ITEM_Object) {
                        unsigned short classIndex = loadShort(ClassFileH);
                        CONSTANTPOOL ConstantPool = CurrentClass->constPool;
                        CLASS clazz;
                        bytesRead += 2;
                        verifyConstantPoolEntry(CurrentClass, 
                                                classIndex, CONSTANT_Class);
                        clazz = CP_ENTRY(classIndex).clazz;
                        stackMap[index++] = clazz->key;
                    } else {
                        fatalError(KVM_MSG_BAD_STACKMAP);
                    }
                }
                stackMap[size_index] = size + size_delta;
            }
    
            /* We suspect that there will be a lot of duplication, so it's worth
             * it to check and see if we already have this identical string */
            for (i = 0; ; i++) {
                if (i == stackMapIndex) { 
                    /* We've reached the end, and no duplicate found */
                    char *temp = mallocBytes(index * sizeof(unsigned short));
                    memcpy(temp, stackMap, index * sizeof(short));
                    stackMaps->data[stackMapIndex].cellp = (cell*)temp;
                    break;
                } else if (memcmp(stackMap, stackMaps->data[i].cellp, 
                                  index * sizeof(short)) == 0) { 
                    /* We have found a duplicate */
                    stackMaps->data[stackMapIndex] = stackMaps->data[i];
                    break;
                } 
            }
        }
    END_TEMPORARY_ROOTS
    return bytesRead;
}

/*=========================================================================
 * FUNCTION:      loadMethodAttributes()
 * TYPE:          private class file load operation
 * OVERVIEW:      Load the "Code" and "Exceptions" attributes
 *                of a method in a class file, ignoring all the 
 *                other possible method attributes.
 * INTERFACE:
 *   parameters:  classfile pointer, constant pool pointer, 
 *                method attribute count, pointer to the runtime method struct
 *   returns:     <nothing>
 *=======================================================================*/

static unsigned int 
loadCodeAttribute(FILEPOINTER_HANDLE ClassFileH, 
                  METHOD_HANDLE thisMethodH, 
                  POINTERLIST_HANDLE StringPoolH)
{ 
    unsigned int actualAttrLength;
    unsigned int codeLength;
    int nCodeAttrs;
    int codeAttrIndex;
    BYTE *code;
    bool_t needStackMap = TRUE;


    METHOD thisMethod = unhand(thisMethodH);
    /*  Create a code object and store it in the method */
    thisMethod->u.java.maxStack    = loadShort(ClassFileH); /*  max stack */
    thisMethod->frameSize   = loadShort(ClassFileH); /*  frame size */
    codeLength              = loadCell(ClassFileH);  /*  Code length */
    if (codeLength >= 0x7FFF) {
        fatalError(KVM_MSG_METHOD_LONGER_THAN_32KB);
    }
    if (thisMethod->u.java.maxStack + thisMethod->frameSize > 
               MAXIMUM_STACK_AND_LOCALS) { 
        fatalError(KVM_MSG_TOO_MANY_LOCALS_AND_STACK);
    }
            
    /*  Allocate memory for storing the bytecode array */
    if (USESTATIC && !ENABLEFASTBYTECODES) { 
        code = (BYTE *)mallocBytes(codeLength);
    } else { 
        code = (BYTE *)callocPermanentObject(ByteSizeToCellSize(codeLength));
    } 
    thisMethod = unhand(thisMethodH); 
    thisMethod->u.java.code = code;

    thisMethod->u.java.codeLength = codeLength;
    loadBytes(ClassFileH, (char *)thisMethod->u.java.code, codeLength);
    actualAttrLength = 2 + 2 + 4 + codeLength;

        /*  Load exception handlers associated with the method */
    actualAttrLength += loadExceptionHandlers(ClassFileH, thisMethodH);
    
    nCodeAttrs = loadShort(ClassFileH);
    actualAttrLength += 2;
    for (codeAttrIndex = 0; codeAttrIndex < nCodeAttrs; codeAttrIndex++) {
        unsigned short codeAttrNameIndex = loadShort(ClassFileH);
        unsigned int   codeAttrLength    = loadCell(ClassFileH);
        char* codeAttrName = getUTF8String(StringPoolH, codeAttrNameIndex);
        /*  Check if the attribute contains stack maps */
        if (!strcmp(codeAttrName, "StackMap")) {
            unsigned int stackMapAttrSize;
            if (!needStackMap) { 
                fatalError(KVM_MSG_DUPLICATE_STACKMAP_ATTRIBUTE);
            } 
            needStackMap = FALSE;
            stackMapAttrSize = loadStackMaps(ClassFileH, thisMethodH);
            if (stackMapAttrSize != codeAttrLength) { 
                fatalError(KVM_MSG_BAD_ATTRIBUTE_SIZE);
            }
        } else {
            skipBytes(ClassFileH, codeAttrLength);
        }
        actualAttrLength += 6 + codeAttrLength;
    }
    return actualAttrLength;
}

static void 
loadMethodAttributes(FILEPOINTER_HANDLE ClassFileH, 
                     METHOD_HANDLE thisMethodH, 
                     POINTERLIST_HANDLE StringPoolH)
{
    unsigned short attrCount = loadShort(ClassFileH);
    METHOD thisMethod = unhand(thisMethodH);
    int attrIndex;
    bool_t needCode = !(thisMethod->accessFlags & (ACC_NATIVE | ACC_ABSTRACT));
    bool_t needExceptionTable = TRUE;  /* always optional */

    /*  See if the field has any attributes in the class file */
    for (attrIndex = 0; attrIndex < attrCount; attrIndex++) {
        unsigned short attrNameIndex = loadShort(ClassFileH);
        unsigned int   attrLength    = loadCell(ClassFileH);
        char*          attrName      = getUTF8String(StringPoolH, attrNameIndex);
        /*  Check if the attribute contains source code */
        if (strcmp(attrName, "Code") == 0) { 
            unsigned int actualLength;
            if (!needCode) { 
                fatalError(KVM_MSG_DUPLICATE_CODE_ATTRIBUTE);
            } 
            actualLength = loadCodeAttribute(ClassFileH, thisMethodH, StringPoolH);
            if (actualLength != attrLength) { 
                fatalError(KVM_MSG_BAD_CODE_ATTRIBUTE_LENGTH);
            }
            needCode = FALSE;
        } else if (!strcmp(attrName, "Exceptions")) { 
            /* Do minimal checking of this attribute. */
            unsigned int exceptionCount;
            unsigned int i;

            if (!needExceptionTable) { 
                fatalError(KVM_MSG_DUPLICATE_EXCEPTION_TABLE);
            } 
            needExceptionTable = FALSE;
            exceptionCount = loadShort(ClassFileH);
            if (2 * exceptionCount + 2 != attrLength) { 
                fatalError(KVM_MSG_BAD_EXCEPTION_ATTRIBUTE);
            } 
            for (i = 0; i < exceptionCount; i++) { 
                unsigned short exception = loadShort(ClassFileH);
                if (exception != 0) {
                    verifyConstantPoolEntry(unhand(thisMethodH)->ofClass,
                                            exception, CONSTANT_Class);
                }
            }
        } else {
            /*  Unrecognized attribute; skip */
            skipBytes(ClassFileH, attrLength);
        }
    }
    if (needCode) { 
        fatalError(KVM_MSG_MISSING_CODE_ATTRIBUTE);
    }
}

/*=========================================================================
 * FUNCTION:      loadMethods()
 * TYPE:          private class file load operation
 * OVERVIEW:      Load the methods defined in a Java class file.
 * INTERFACE:
 *   parameters:  classfile pointer, current class pointer
 *   returns:     <nothing>
 *=======================================================================*/

static void
loadOneMethod(FILEPOINTER_HANDLE ClassFileH, INSTANCE_CLASS CurrentClass, 
              METHOD_HANDLE thisMethodH, POINTERLIST_HANDLE StringPoolH) 
{
    METHOD thisMethod;
    unsigned short accessFlags = 
        loadShort(ClassFileH) & RECOGNIZED_METHOD_FLAGS;
    unsigned short nameIndex   = loadShort(ClassFileH);
    unsigned short typeIndex   = loadShort(ClassFileH);
    START_TEMPORARY_ROOTS
        DECLARE_TEMPORARY_ROOT(const char *, methodName, 
                               getUTF8String(StringPoolH, nameIndex));
        DECLARE_TEMPORARY_ROOT(const char *, signature, 
                               getUTF8String(StringPoolH, typeIndex));
        NameTypeKey result;

        if (strcmp(methodName, "<clinit>") == 0) {
            accessFlags = ACC_STATIC;
        } else {
            verifyMethodFlags(accessFlags, 
                              CurrentClass->clazz.accessFlags, 
                              methodName);
        }
        verifyName(methodName, LegalMethod, TRUE);
    
        result.nt.nameKey = 
            change_Name_to_Key(&methodName, 0, strlen(methodName));
        result.nt.typeKey = 
            change_MethodSignature_to_Key(&signature, 0, strlen(signature));

        ASSERTING_NO_ALLOCATION
            thisMethod = unhand(thisMethodH);
            thisMethod->nameTypeKey = result;
            thisMethod->argCount = verifyMethodType(methodName, signature);

            /*  If this is a virtual method, increment argument counter */
            if (!(accessFlags & ACC_STATIC)) { 
                thisMethod->argCount++;
            }

            if (thisMethod->argCount > 255) {
                fatalError(KVM_MSG_TOO_MANY_METHOD_ARGUMENTS);
            }

#if INCLUDEDEBUGCODE
            if (traceclassloadingverbose) { 
                fprintf(stdout, "Method '%s' loaded\n", methodName);
            }
#endif /* INCLUDEDEBUGCODE */

            /* Check if the field is double length, or is a pointer type.  If so
             * set the appropriate bit in the word */
            switch (strchr(signature, ')')[1]) { 
                case 'D': case 'J':   accessFlags |= ACC_DOUBLE;   break;
                case 'L': case '[':   accessFlags |= ACC_POINTER;  break;
                case 'V':   accessFlags |= (ACC_POINTER | ACC_DOUBLE);  break;
            }

            thisMethod->accessFlags = accessFlags;
            thisMethod->ofClass     = CurrentClass;
            /* These values will be initialized later */
            thisMethod->frameSize   = 0;
            thisMethod->u.java.maxStack    = 0;
            thisMethod->u.java.handlers = NIL;
        END_ASSERTING_NO_ALLOCATION

        loadMethodAttributes(ClassFileH, thisMethodH, StringPoolH);

        /* Garbage collection may have happened */
        thisMethod = unhand(thisMethodH);
        if (!(thisMethod->accessFlags & (ACC_NATIVE | ACC_ABSTRACT))) {
            if ( thisMethod->frameSize < thisMethod->argCount) {
                fatalError(KVM_MSG_BAD_FRAME_SIZE);
            }
        }

        if (accessFlags & ACC_NATIVE) {
            /*  Store native function pointer in the code field */
            thisMethod->u.native.info = NULL;
            thisMethod->u.native.code = 
                getNativeFunction(CurrentClass, methodName, signature);
        }
    END_TEMPORARY_ROOTS

}            

static void 
loadMethods(FILEPOINTER_HANDLE ClassFileH, INSTANCE_CLASS CurrentClass, 
            POINTERLIST_HANDLE StringPoolH)
{
    unsigned short methodCount = loadShort(ClassFileH);
    if (methodCount == 0) { 
        return;
    } 
    START_TEMPORARY_ROOTS
        int tableSize = SIZEOF_METHODTABLE(methodCount);
        unsigned int index;
#if USESTATIC
        DECLARE_TEMPORARY_ROOT(METHODTABLE, methodTable, 
            (METHODTABLE)callocObject(tableSize, GCT_METHODTABLE));
#else
        METHODTABLE methodTable = (METHODTABLE)callocPermanentObject(tableSize);
#endif
        methodTable->length = methodCount;
        CurrentClass->methodTable = methodTable;

#if INCLUDEDEBUGCODE
        if (traceclassloadingverbose) { 
            fprintf(stdout, "Loading methods\n");
        }
#endif /* INCLUDEDEBUGCODE */

        for (index = 0; index < methodCount; index++) { 
#if USESTATIC
            START_TEMPORARY_ROOTS
                DECLARE_TEMPORARY_METHOD_ROOT(thisMethod, methodTable, index);
                loadOneMethod(ClassFileH, CurrentClass, &thisMethod, StringPoolH);
            END_TEMPORARY_ROOTS
#else 
            METHOD thisMethod = &methodTable->methods[index];
            loadOneMethod(ClassFileH, CurrentClass, &thisMethod, StringPoolH);
#endif
        }
    END_TEMPORARY_ROOTS

    if (methodCount >= 2) { 
        /* Check to see if there are two methods with the same name/type */
        METHODTABLE methodTable = CurrentClass->methodTable;
        METHOD firstMethod = &methodTable->methods[0];
        METHOD lastMethod = firstMethod + (methodCount - 1);
        METHOD outer, inner;
        for (outer = firstMethod; outer < lastMethod; outer++) { 
            for (inner = outer + 1; inner <= lastMethod; inner++) { 
                if (outer->nameTypeKey.i == inner->nameTypeKey.i) { 
                    fatalError(KVM_MSG_DUPLICATE_METHOD_FOUND);
                }
            }
        }
    }

#if INCLUDEDEBUGCODE
    if (traceclassloadingverbose) {
        fprintf(stdout, "Methods loaded ok\n");
    }
#endif /* INCLUDEDEBUGCODE */

}

/*=========================================================================
 * FUNCTION:      ignoreAttributes()
 * TYPE:          private class file load operation
 * OVERVIEW:      Load the possible extra attributes (e.g., the 
 *                "SourceFile" attribute) supplied in a Java class file.
 *                The current implementation ignores all these attributes.
 * INTERFACE:
 *   parameters:  classfile pointer, current class pointer
 *   returns:     <nothing>
 *=======================================================================*/

static void 
ignoreAttributes(FILEPOINTER_HANDLE ClassFileH, POINTERLIST_HANDLE StringPoolH)
{
    unsigned short attrCount = loadShort(ClassFileH);
    int attrIndex;
   
#if INCLUDEDEBUGCODE
    if (traceclassloadingverbose) { 
        fprintf(stdout, "Loading extra attributes\n");
    }
#endif /* INCLUDEDEBUGCODE */


    for (attrIndex = 0; attrIndex < attrCount; attrIndex++) {
        unsigned short attrNameIndex = loadShort(ClassFileH);
        unsigned int   attrLength = loadCell(ClassFileH);

        /* This verifies that the attribute index is legitimate */
        (void)getUTF8String(StringPoolH, attrNameIndex);

        skipBytes(ClassFileH, attrLength);
    }

#if INCLUDEDEBUGCODE
    if (traceclassloadingverbose) 
        fprintf(stdout, "Extra attributes loaded\n");
#endif /* INCLUDEDEBUGCODE */

}

/*=========================================================================
 * FUNCTION:      loadClassfile()
 * TYPE:          constructor (kind of)
 * OVERVIEW:      Load the given Java class file in the system.
 * INTERFACE:
 *   parameters:  class structure.  Only the name field is filled in.  All
 *                other fields must be NULL.
 *   returns:     pointer to runtime class structure or NIL if not found
 *=======================================================================*/

static void
loadClassfileHelper(INSTANCE_CLASS CurrentClass, bool_t fatalErrorIfFail)
{
    POINTERLIST StringPool;

    if (CurrentClass->status != CLASS_RAW) { 
        return;
    } 
#if ROMIZING
    {
        UString uPackageName = CurrentClass->clazz.packageName;
        if (uPackageName != NULL) { 
            char *name = UStringInfo(uPackageName);
            if (IS_RESTRICTED_PACKAGE_NAME(name)) { 
                if (fatalErrorIfFail) {
                    fatalError(KVM_MSG_CREATING_CLASS_IN_SYSTEM_PACKAGE);
                }
                CurrentClass->status = CLASS_ERROR;
                return;
            }
        }
    }
#endif

    CurrentClass->status = CLASS_LOADING;

    START_TEMPORARY_ROOTS
        DECLARE_TEMPORARY_ROOT(FILEPOINTER, ClassFile, openClassfile(CurrentClass));
        if (ClassFile) {
            int ch;

#if INCLUDEDEBUGCODE
            if (traceclassloading || traceclassloadingverbose) {
                Log->loadClass(getClassName((CLASS)CurrentClass));
            }
#endif
            /*  Load version info and magic value */
            loadVersionInfo(&ClassFile);

            /*  Load and create constant pool */
            IS_TEMPORARY_ROOT(StringPool, 
                              loadConstantPool(&ClassFile, CurrentClass));

            /*  Load class identification information */
            loadClassInfo(&ClassFile, CurrentClass);

            /*  Load interface pointers */
            loadInterfaces(&ClassFile, CurrentClass);

            /*  Load field information */
            loadFields(&ClassFile, CurrentClass, &StringPool);

            /*  Load method information */
            loadMethods(&ClassFile, CurrentClass, &StringPool);

            /*  Load the possible extra attributes (e.g., debug info) */
            ignoreAttributes(&ClassFile, &StringPool);

            ch = loadByteNoEOFCheck(&ClassFile);
            if (ch != -1) { 
                fatalError(KVM_MSG_CLASSFILE_SIZE_DOES_NOT_MATCH);
            }

            /*  Ensure that EOF has been reached successfully and close file */
            closeClassfile(&ClassFile);

            /*  Make the class an instance of class 'java.lang.Class' */
            CurrentClass->clazz.ofClass =  JavaLangClass;

#if INCLUDEDEBUGCODE
            if (traceclassloading || traceclassloadingverbose) {
                fprintf(stdout, "Creating class: '%s'\n", 
                    getClassName((CLASS)CurrentClass));
            }
#endif
            /*  Raise the class status to 'LOADED' */
            CurrentClass->status = CLASS_LOADED;

#if ENABLE_JAVA_DEBUGGER
            if (vmDebugReady) {
                CEModPtr cep = GetCEModifier();
                cep->loc.classID = GET_CLASS_DEBUGGERID(&CurrentClass->clazz);
                cep->threadID = getObjectID((OBJECT)CurrentThread->javaThread);
                cep->eventKind = JDWP_EventKind_CLASS_LOAD;
                insertDebugEvent(cep);
            }
#endif

#if INCLUDEDEBUGCODE
            if (traceclassloadingverbose) {
                fprintf(stdout, "Class loaded ok\n");
            }
#endif /* INCLUDEDEBUGCODE */

        } else { 
            CurrentClass->status = CLASS_ERROR;
            if (fatalErrorIfFail) { 
                sprintf(str_buffer, KVM_MSG_CANNOT_LOAD_CLASS_1PARAM, 
                    getClassName((CLASS)CurrentClass));
                fatalError(str_buffer);
            }
        }
    END_TEMPORARY_ROOTS
}

static INSTANCE_CLASS
findSuperMostUnlinked(INSTANCE_CLASS CurrentClass)
{
    INSTANCE_CLASS result = NULL;
    while (CurrentClass) {
        if (CurrentClass->status < CLASS_LINKED) {
            result = CurrentClass;
        } else {
            break;
        }
        CurrentClass = CurrentClass->superClass;
    }
    return result;
}

struct loadingBacktraceStruct { 
    INSTANCE_CLASS clazz;
    struct loadingBacktraceStruct *prev;
};

static void
loadClassfileInternal(INSTANCE_CLASS CurrentClass, bool_t fatalErrorIfFail,
                      struct loadingBacktraceStruct *backtrace);
      
void
loadClassfile(INSTANCE_CLASS CurrentClass, bool_t fatalErrorIfFail)
{
    loadClassfileInternal(CurrentClass, fatalErrorIfFail, NULL);
}

static void
loadClassfileInternal(INSTANCE_CLASS CurrentClass, bool_t fatalErrorIfFail,
                      struct loadingBacktraceStruct *backtrace)
{
    INSTANCE_CLASS clazz = CurrentClass;

    while (clazz && (clazz->status == CLASS_RAW)) {
        loadClassfileHelper(clazz, fatalErrorIfFail);
        if (clazz->status == CLASS_ERROR) {
            /* If it's the current class that fails, then the loader
             * handles it.  But if a superclass fails, then this
             * is serious.
             */
            if (clazz != CurrentClass) { 
                NoClassDefFoundError(clazz);
            }
            return;
        }
        clazz = clazz->superClass;
    }

    for (clazz = CurrentClass->superClass; clazz != NULL; 
        clazz = clazz->superClass) { 
        if (clazz == CurrentClass) { 
            fatalError(KVM_MSG_CLASS_CIRCULARITY_ERROR);
            /* If we get rid of the fatal error, we have to mark
             * everything in the loop as status  CLASS_ERROR */
        }
    }

    while ((clazz = findSuperMostUnlinked(CurrentClass)) != NULL) {
        INSTANCE_CLASS superClass = clazz->superClass;

        /* Compute instance size and instance field offset */
        if (superClass != NULL) {
            if (superClass->clazz.accessFlags & ACC_FINAL) {
                fatalError(KVM_MSG_CLASS_EXTENDS_FINAL_CLASS);
            }
            if (superClass->clazz.accessFlags & ACC_INTERFACE) { 
                fatalError(KVM_MSG_CLASS_EXTENDS_INTERFACE);
            }
            verifyClassAccess((CLASS)superClass, clazz);

            /* Make the instance size of the new class "inherit" */
            /* the instance size of the superclass */
            clazz->instSize = superClass->instSize;
        } else {
            /*  Treat 'java.lang.Object' specially (it has no superclass) */
            clazz->instSize = 0;
        }

        if (clazz->ifaceTable) { 
            struct loadingBacktraceStruct newBacktrace;
            unsigned int tableLength = clazz->ifaceTable[0];
            unsigned int i;
            newBacktrace.prev = backtrace;
            newBacktrace.clazz = clazz;
            for (i = 1; i <= tableLength; i++) {
                int cpIndex = clazz->ifaceTable[i];
                struct loadingBacktraceStruct *tmp;
                CLASS intf = clazz->constPool->entries[cpIndex].clazz;
                if (IS_ARRAY_CLASS(intf)) { 
                    fatalError(KVM_MSG_CLASS_IMPLEMENTS_ARRAY_CLASS);
                } else if (intf == (CLASS)clazz) { 
                    fatalError(KVM_MSG_CLASS_IMPLEMENTS_ITSELF);
                } 
                for (tmp = &newBacktrace; tmp != NULL; tmp = tmp->prev) { 
                    if (tmp->clazz == (INSTANCE_CLASS)intf) { 
                        fatalError(KVM_MSG_INTERFACE_CIRCULARITY_ERROR);
                    }
                }
                loadClassfileInternal((INSTANCE_CLASS)intf, TRUE, &newBacktrace);
                if ((intf->accessFlags & ACC_INTERFACE) == 0) { 
                   fatalError(KVM_MSG_CLASS_IMPLEMENTS_NON_INTERFACE);
                }
                verifyClassAccess(intf, clazz);
            }
        }

        FOR_EACH_FIELD(thisField, clazz->fieldTable) 
            unsigned short accessFlags = (unsigned short)thisField->accessFlags;

            if ((accessFlags & ACC_STATIC) == 0) {
                thisField->u.offset = clazz->instSize;
                clazz->instSize += (accessFlags & ACC_DOUBLE) ? 2 : 1;
            }
        END_FOR_EACH_FIELD

        /* Move parts of the class to static memory */
 
        /* **DANGER** 
         * While moveClassFieldsToStatic is running, things are in a
         * very strange state as far as GC is concerned.  It is important
         * that this function do no allocation, and that we get rid
         * of the temporary roots registered above before any other
         * allocation is done.
         */
        moveClassFieldsToStatic(clazz);

        clazz->status = CLASS_LINKED;

#if ENABLE_JAVA_DEBUGGER
        if (vmDebugReady) {
            CEModPtr cep = GetCEModifier();
            cep->loc.classID = GET_CLASS_DEBUGGERID(&clazz->clazz);
            cep->threadID = getObjectID((OBJECT)CurrentThread->javaThread);
            cep->eventKind = JDWP_EventKind_CLASS_PREPARE;
            insertDebugEvent(cep);
        }
#endif
    }
}

void
loadArrayClass(ARRAY_CLASS clazz) 
{ 
    INSTANCE_CLASS base;
    if (clazz->flags & ARRAY_FLAG_BASE_NOT_LOADED) { 
        CLASS cb = (CLASS)clazz;
        do { 
            cb = ((ARRAY_CLASS)cb)->u.elemClass; 
        } while (IS_ARRAY_CLASS(cb));
        base = (INSTANCE_CLASS)cb;
        loadClassfile(base, TRUE);
        /* Run down the chain again, resetting the flags appropriately */
        cb = (CLASS)clazz;
        do { 
            if (base->clazz.accessFlags & ACC_PUBLIC) { 
                cb->accessFlags |= ACC_PUBLIC;
            }
            ((ARRAY_CLASS)cb)->flags &= ~ARRAY_FLAG_BASE_NOT_LOADED;
            cb = ((ARRAY_CLASS)cb)->u.elemClass; 
        } while (IS_ARRAY_CLASS(cb));
    }
}

/*=========================================================================
 * Helper functions
 *=======================================================================*/

/*=========================================================================
 * FUNCTION:      appendToStatic(), moveClassFieldsToStatic()
 * TYPE:          private functions
 * OVERVIEW:      Helper functions to move data structures from
 *                from dynamic memory to static memory.
 * INTERFACE:
 *   parameters:
 *   returns:
 *=======================================================================*/

#if USESTATIC

static void
appendToStatic (char *staticMemory, void* valuePtr, int *offset);

static void
moveClassFieldsToStatic(INSTANCE_CLASS CurrentClass)
{
    /* We are going to have to walk the method table, so get info */
    METHODTABLE methodTable   = CurrentClass->methodTable;
    FIELDTABLE  fieldTable    = CurrentClass->fieldTable;
    CONSTANTPOOL constantPool = CurrentClass->constPool;
    unsigned short* ifaceTable          = CurrentClass->ifaceTable;

    char* staticBytes;
    int offset;
    
    int totalSize = getObjectSize((cell *)constantPool) /* never NULL */
             + ((fieldTable == NULL) ? 0 : getObjectSize((cell *)fieldTable))
             + ((ifaceTable == NULL) ? 0 : getObjectSize((cell *)ifaceTable))
             + ((methodTable == NULL) ? 0 : getObjectSize((cell *)methodTable));

    FOR_EACH_METHOD(thisMethod, methodTable)
        if ((thisMethod->accessFlags & ACC_NATIVE) == 0) { 
            if (!ENABLEFASTBYTECODES && thisMethod->u.java.code != NULL) {
                totalSize += getObjectSize((cell *)thisMethod->u.java.code);
            } 
            if (thisMethod->u.java.handlers != NULL) { 
                totalSize += getObjectSize((cell *)thisMethod->u.java.handlers);
            }
        }
    END_FOR_EACH_METHOD

    staticBytes = mallocStaticBytes(totalSize << log2CELL);
    
    /* Copy the constant pool */
    offset = 0;

    appendToStatic(staticBytes, &CurrentClass->constPool,  &offset);
    appendToStatic(staticBytes, &CurrentClass->fieldTable, &offset);
    appendToStatic(staticBytes, &CurrentClass->ifaceTable, &offset);
    
    FOR_EACH_METHOD(thisMethod, methodTable)
        if ((thisMethod->accessFlags & ACC_NATIVE) == 0) { 
            if (!ENABLEFASTBYTECODES) { 
                appendToStatic(staticBytes, &thisMethod->u.java.code, &offset);
            }
            appendToStatic(staticBytes, &thisMethod->u.java.handlers, &offset);
        }
    END_FOR_EACH_METHOD

    appendToStatic(staticBytes, &CurrentClass->methodTable, &offset);
    if (offset != totalSize << log2CELL) { 
        fatalError(KVM_MSG_UNABLE_TO_COPY_TO_STATIC_MEMORY);
    }
}

/* This is a helper function for moveClassFieldsToStatic() above, and is
 * only ever called from it. 
 * It appends the object *valuePtr to the allocated memory starting at
 * offset *offsetP.  
 * It then updates *offsetP to include the length of this object, and
 * valuePtr to point to the copy of the object */

static void
appendToStatic (char *staticBytes, void *valuePtr, int *offsetP)
{ 
    cell *value = *(cell **)valuePtr;
    if (value != NULL) { 
        int size = getObjectSize(value) << log2CELL;
        int offset = *offsetP;
        modifyStaticMemory(staticBytes, offset, value, size);
        *offsetP = offset + size;
        *(cell **)valuePtr = (cell *)&staticBytes[offset]; 
    }
}

#endif /* USESTATIC */

/*=========================================================================
 * FUNCTION:      NoClassDefFoundError
 * TYPE:          auxiliary private function
 * OVERVIEW:      Fatal error indicating class not found
 * INTERFACE:
 *   parameters:  class whose definition cannot be found
 *   returns:     doesn't.
 *=======================================================================*/

static void 
NoClassDefFoundError(INSTANCE_CLASS thisClass) { 
    sprintf(str_buffer, "NoClassDefFoundError: %s", getClassName((CLASS)thisClass));
    fatalError(str_buffer);
}

/*=========================================================================
 * FUNCTION:      replaceLetters()
 * TYPE:          auxiliary private function
 * OVERVIEW:      Convert all characters c1 in the given string to 
 *                character c2.
 * INTERFACE:
 *   parameters:  string, two characters
 *   returns:     modified string
 *=======================================================================*/

char* replaceLetters(char* string, char c1, char c2)
{
    unsigned int length = strlen(string);
    unsigned int i;
    char* c = string;

    for (i = 0; i < length; i++, c++) {
        if (*c == c1) *c = c2;
    }

    return string;
}


