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
 * SUBSYSTEM: Main Java program
 * FILE:      StartJVM.c
 * OVERVIEW:  System initialization and start of JVM
 * AUTHOR:    Antero Taivalsaari, Sun Labs
 *            Edited by Doug Simon 11/1998
 *            Frank Yellin
 *            Bill Pittore (Java-level debugging)
 *=======================================================================*/

/*=========================================================================
 * Include files
 *=======================================================================*/

#include <global.h>

/*=========================================================================
 * Functions
 *=======================================================================*/

/*=========================================================================
 * FUNCTION:      readCommandLineArguments()
 * TYPE:          constructor (kind of)
 * OVERVIEW:      Read the extra command line arguments provided
 *                by the user and construct a string array containing
 *                the arguments. Note: before executing 'main' the
 *                string array must be pushed onto the operand stack.
 * INTERFACE:
 *   parameters:  command line parameters
 *   returns:     string array containing the command line arguments
 *=======================================================================*/

static ARRAY
readCommandLineArguments(int argc, char* argv[])
{
    /*  Get the number of command line arguments */
    ARRAY_CLASS arrayClass = getArrayClass(1, JavaLangString, 0);
    ARRAY stringArray;
    int numberOfArguments = argc;
    int argCount = (numberOfArguments > 0) ? numberOfArguments : 0;
    int i;

    /*  Create a string array for the arguments */
    START_TEMPORARY_ROOTS
        IS_TEMPORARY_ROOT(stringArray, instantiateArray(arrayClass, argCount));
        for (i = 0; i < numberOfArguments; i++) {
            stringArray->data[i].cellp =
                (cell *)instantiateString(argv[i], strlen(argv[i]));
        }
    END_TEMPORARY_ROOTS
    return stringArray;
}

/*=========================================================================
 * FUNCTION:      loadMainClass()
 * TYPE:          private operation
 * OVERVIEW:      The main class is loaded in a slightly different
 *                way than the other classes.  We need to make sure
 *                that the given main class is not an array class.
 *                Also, we need to replace all the '.' characters
 *                in the class name with '/' characters.
 * INTERFACE:
 *   parameters:  class name
 *   returns:     the main class
 *=======================================================================*/

static INSTANCE_CLASS
loadMainClass(char* className)
{
    /* Make sure this is not an array class */
    if (*className == '[') return NULL;

    /* Replace all the '.' characters in the class name with '/' chars */
    replaceLetters(className, '.', '/');

    return (INSTANCE_CLASS)getClass(className);
}

/*=========================================================================
 * FUNCTION:      KVM_Start
 * TYPE:          private operation
 * OVERVIEW:      Initialize everything.  This operation is called
 *                when the VM is launched.
 * INTERFACE:
 *   parameters:  command line parameters
 *   returns:     zero if everything went well, non-zero otherwise
 *=======================================================================*/

int KVM_Start(int argc, char* argv[])
{
    INSTANCE_CLASS mainClass;
    volatile int returnValue = 0; /* Needed to make compiler happy */

    ERROR_TRY {
        /* If ROMIZING and RELOCATABLE_ROM */
        CreateROMImage();

#if ASYNCHRONOUS_NATIVE_FUNCTIONS
        /* Initialize asynchronous I/O system */
        InitalizeAsynchronousIO();
#endif

        /* Initialize all the essential runtime structures of the VM */
        InitializeNativeCode();
        InitializeVM();

        /* Initialize global variables */
        InitializeGlobals();

        /* Initialize profiling variables */
        InitializeProfiling();

        /* Initialize the memory system */
        InitializeMemoryManagement();

        /* Initialize internal hash tables */
        InitializeHashtables();

        /* Initialize inline caching structures */
        InitializeInlineCaching();

        /* Initialize the class loading interface */
        InitializeClassLoading();

        /* Load and initialize the Java system classes needed by the VM */
        InitializeJavaSystemClasses();

        /* Initialize the class file verifier */
        InitializeVerifier();

        /* Initialize the event handling system */
        InitializeEvents();

        /* Load the main application class */
        mainClass = loadMainClass(argv[0]);
        if (!mainClass) {
            sprintf(str_buffer, KVM_MSG_CLASS_NOT_FOUND_1STRPARAM, argv[0]);
            AlertUser(str_buffer);
            returnValue = 1;
        } else {

            /* Parse command line arguments */
            ARRAY arguments = readCommandLineArguments(argc - 1, argv + 1);

            /* Now that we have a main class, initialize */
            /* the multithreading system */
            InitializeThreading(mainClass, arguments);

            /* Instances of these classes may have been created without
             * explicitly executing the "new" instruction. Thus we have
             * to make sure they are initialized.
             */
#if ENABLE_JAVA_DEBUGGER
            /* Prepare the VM for Java-level debugging */
            if (debuggerActive) {
                InitDebugger();
            }
#endif
            /* These are pushed onto the stack.  So JavaLangClass is
             * the first class that is initialized.
             */
            initializeClass(JavaLangOutOfMemoryError);
            initializeClass(JavaLangSystem);
            initializeClass(JavaLangString);
            initializeClass(JavaLangThread);
            initializeClass(JavaLangClass);

#if ENABLE_JAVA_DEBUGGER
            /* Prepare the VM for Java-level debugging */
            if (vmDebugReady) {
                setEvent_VMInit();
                if (CurrentThread == NIL) {
                    CurrentThread = removeFirstRunnableThread();
                    /* make sure xp_globals are synched with thread data */
                    loadExecutionEnvironment(CurrentThread);
                }
                sendAllClassPrepares();
            }
#endif /* ENABLE_JAVA_DEBUGGER */

            /* Start the interpreter */
            Interpret();
        }
    } ERROR_CATCH (error) {
        returnValue = error;
    } ERROR_END_CATCH

    return returnValue;
}

/*=========================================================================
 * FUNCTION:      KVM_Cleanup
 * TYPE:          private operation
 * OVERVIEW:      Clean up everything.  This operation is called
 *                when the VM is shut down.
 * INTERFACE:
 *   parameters:  <none>
 *   returns:     <nothing>
 *=======================================================================*/

void KVM_Cleanup()
{
#if ENABLE_JAVA_DEBUGGER
    if (vmDebugReady) {
        setEvent_VMDeath();
        CloseDebugger();
        clearAllBreakpoints();
    }
#endif
    FinalizeVM();
    FinalizeInlineCaching();
    FinalizeNativeCode();
    FinalizeJavaSystemClasses();
    FinalizeClassLoading();
    FinalizeMemoryManagement();
    DestroyROMImage();
    FinalizeHashtables();
}

/*=========================================================================
 * FUNCTION:      StartJVM
 * TYPE:          public global operation
 * OVERVIEW:      Boots up the virtual machine and executes 'main'.
 * INTERFACE:
 *   parameters:  command line arguments
 *   returns:     zero if everything went fine, non-zero otherwise.
 *=======================================================================*/

int StartJVM(int argc, char* argv[])
{
    volatile int returnValue = 0;

    /* Ensure that we have a class to run */
    if (argc <= 0 || argv[0] == NULL) {
        AlertUser(KVM_MSG_MUST_PROVIDE_CLASS_NAME);
        return -1;
    }

    returnValue = KVM_Start(argc, argv);
    KVM_Cleanup();
    return returnValue;
}


