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
 * FILE:      loader.h
 * OVERVIEW:  Internal definitions needed for loading 
 *            Java class files (class loader). 
 * AUTHOR:    Antero Taivalsaari, Sun Labs
 *            Sheng Liang, Frank Yellin, many others...
 *=======================================================================*/

/*=========================================================================
 * COMMENTS:
 * This file defines a JVM Specification compliant classfile reader.
 * It is capable of reading any standard Java classfile, and generating
 * the necessary corresponding VM runtime structures.
 *=======================================================================*/

/*=========================================================================
 * Include files
 *=======================================================================*/

/*=========================================================================
 * Definitions and declarations
 *=======================================================================*/

extern char* UserClassPath; /* set in main() or elsewhere */

#ifndef PATH_SEPARATOR
#   define PATH_SEPARATOR ':'
#endif

/*=========================================================================
 * Classfile loading data structures
 *=======================================================================*/

/*  This structure is used for referring to open "files" when  */
/*  loading Java classfiles from the storage system of the host */
/*  operating system. It replaces the standard FILE* structure */
/*  used in C program. We need our own structure, since many of */
/*  our target devices don't have a regular file system. */

/*  FILEPOINTER */
struct filePointerStruct;

#define SIZEOF_FILEPOINTER       StructSizeInCells(filePointerStruct)

    /* This pointerlist maintains a mapping between an integer file
     * descriptor and filepointers for resource files.  Added to support
     * MIDP changes to ResourceInputStream.java for commonality between KVM
     * and Project Monty
     */

#define FILE_OBJECT_SIZE 4
extern POINTERLIST filePointerRoot;



/*=========================================================================
 * Class file verification operations (performed during class loading)
 *=======================================================================*/

enum verifyName_type {LegalMethod, LegalField, LegalClass};
bool_t verifyName(const char* name, enum verifyName_type, bool_t abortOnError);

/*=========================================================================
 * Class loading operations
 *=======================================================================*/

void loadClassfile(INSTANCE_CLASS CurrentClass, bool_t fatalErrorIfFail);
void loadArrayClass(ARRAY_CLASS);

/*=========================================================================
 * Generic class file reading operations
 *=======================================================================*/

/*
 * NOTE: The functions below define an abstract interface for
 * reading data from class files.  The actual implementations
 * of these functions are platform-dependent, and therefore are
 * not provided in VmCommon. An implementation of these functions
 * must be provided for each platform. A sample implementation 
 * can be found in VmExtra/src/loaderFile.c.
 */
int            loadByteNoEOFCheck(FILEPOINTER_HANDLE);
int            loadBytesNoEOFCheck(FILEPOINTER_HANDLE, char* buffer, int pos, int length);
unsigned char  loadByte(FILEPOINTER_HANDLE);
unsigned short loadShort(FILEPOINTER_HANDLE);
unsigned long  loadCell(FILEPOINTER_HANDLE);
void           loadBytes(FILEPOINTER_HANDLE, char* buffer, int length);
void           skipBytes(FILEPOINTER_HANDLE, unsigned long i);
int            getBytesAvailable(FILEPOINTER_HANDLE);

void           InitializeClassLoading(void);
void           FinalizeClassLoading();

FILEPOINTER    openClassfile(INSTANCE_CLASS clazz);
FILEPOINTER    openResourcefile(BYTES resourceName);
void           closeClassfile(FILEPOINTER_HANDLE);

int setFilePointer(FILEPOINTER_HANDLE);
FILEPOINTER getFilePointer(int);
void clearFilePointer(int);

/*=========================================================================
 * Helper functions
 *=======================================================================*/

char* replaceLetters(char* string, char c1, char c2);

