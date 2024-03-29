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
 * KVM
 *=========================================================================
 * SYSTEM:    KVM
 * SUBSYSTEM: Unix-specific functions needed by the virtual machine
 * FILE:      runtime_md.c
 * AUTHOR:    Frank Yellin
 *            Andreas Heilwagen, Kinsley Wong (Linux port)
 *=======================================================================*/

/*=========================================================================
 * Include files
 *=======================================================================*/

#include <global.h>
#include <async.h>

#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>

/*=========================================================================
 * Definitions and variables
 *=======================================================================*/

#define MAXCALENDARFLDS 15

#define YEAR 1
#define MONTH 2
#define DAY_OF_MONTH 5
#define HOUR 10
#define MINUTE 12
#define SECOND 13
#define MILLISECOND 14

static unsigned long date[MAXCALENDARFLDS];

/*=========================================================================
 * Functions
 *=======================================================================*/

/*=========================================================================
 * FUNCTION:      alertUser()
 * TYPE:          error handling operation
 * OVERVIEW:      Show an alert dialog to the user and wait for 
 *                confirmation before continuing execution.
 * INTERFACE:
 *   parameters:  message string
 *   returns:     <nothing>
 *=======================================================================*/

void AlertUser(const char* message)
{
    fprintf(stderr, "ALERT: %s\n", message);
}

/*=========================================================================
 * FUNCTION:      allocateHeap()
 * TYPE:          allocates memory 
 * OVERVIEW:      Show an alert dialog to the user and wait for 
 *                confirmation before continuing execution.
 * INTERFACE:
 *   parameters:  *sizeptr:  INPUT:   Pointer to size of heap to allocate
 *                           OUTPUT:  Pointer to actually size of heap
 *                *realresultptr: Returns pointer to actual pointer than
 *                           was allocated, before any adjustments for
 *                           memory alignment.  This is the value that
 *                           must be passed to "free()"
 * 
 *  returns:      pointer to aligned memory.
 *
 *  Note that "sizeptr" reflects the size of the "aligned" memory, and
 *  note the actual size of the memory that has been allocated.
 *=======================================================================*/

cell *allocateHeap(long *sizeptr, void **realresultptr) { 
    void *space = malloc(*sizeptr + sizeof(cell) - 1);
    *realresultptr = space;
    return (void *) ((((long)space) + (sizeof(cell) - 1)) & ~(sizeof(cell) - 1));
}

/* Virtual memory allocation and protection operations. */
/* Used for testing the correctness of the garbage collector */

void *
allocateVirtualMemory_md(long size) {
    int devZero = open("/dev/zero", O_RDONLY);
    void *result = mmap(0, size, PROT_READ | PROT_WRITE, 
                        MAP_PRIVATE, devZero, 0);
    close(devZero);
    return result;
}

void 
freeVirtualMemory_md(void *address, long size) { 
    munmap(address, size);
}

void  
protectVirtualMemory_md(void *address, long size, int protection) {
    int flag;
    switch (protection) { 
        case PVM_NoAccess:  flag = PROT_NONE; break;
        case PVM_ReadOnly:  flag = PROT_READ; break;
        case PVM_ReadWrite: flag = PROT_READ | PROT_WRITE; break;
        default:            fatalError("Bad argument");
    }
    mprotect(address, size, flag);
}

/*=========================================================================
 * FUNCTION:      signal_handler (showStack)
 * TYPE:          debugging operation
 * OVERVIEW:      called when we receive a coredump signal on Unix
 * INTERFACE:
 *   parameters:  signal
 *   returns:     none
 *=======================================================================*/

static void signal_handler(int sig) {
    char *name;

    switch (sig) {
        case SIGILL:
            name = "SIGILL";
            break;
        case SIGABRT:
            name = "SIGABRT";
            break;
        case SIGBUS:
            name = "SIGBUS";
            break;
        case SIGSEGV:
            name = "SIGSEGV";
            break;
        default:
            name = "??";
    }
    fprintf(stdout, "received signal %s\n", name);
    printStackTrace();
    printProfileInfo();

    signal(SIGILL,  NULL);
    signal(SIGABRT, NULL);
    signal(SIGBUS,  NULL); 
    signal(SIGSEGV, NULL); 

    abort();
}

/*=========================================================================
 * FUNCTION:      InitializeNativeCode
 * TYPE:          initialization
 * OVERVIEW:      called at start up to perform machine-specific initialization. 
 * INTERFACE:
 *   parameters:  none
 *   returns:     none
 *=======================================================================*/

void InitializeNativeCode() {
    /*
     * Create signal handlers to dump Java stack in case of coredumps
     */
    signal(SIGILL,  signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGBUS,  signal_handler); 
    signal(SIGSEGV, signal_handler); 
    signal(SIGPIPE, SIG_IGN);
}

/*=========================================================================
 * FUNCTION:      nativeFinalization
 * TYPE:          initialization
 * OVERVIEW:      called at start up to perform machine-specific initialization. 
 * INTERFACE:
 *   parameters:  none
 *   returns:     none
 *=======================================================================*/

void FinalizeNativeCode() { 
}

/*=========================================================================
 * FUNCTION:      CurrentTime_md()
 * TYPE:          machine-specific implementation of native function
 * OVERVIEW:      Returns the current time. 
 * INTERFACE:
 *   parameters:  none
 *   returns:     current time, in centiseconds since startup
 *=======================================================================*/

ulong64
CurrentTime_md(void)
{
    struct timeval tv;
    long long result;
    gettimeofday(&tv, NULL);
    /* We adjust to 1000 ticks per second */
    result = (long long)tv.tv_sec * 1000 + tv.tv_usec/1000;
#if COMPILER_SUPPORTS_LONG 
    return result;
#else 
    /* We use this for testing.  Clearly, the compiler supports longs, as
     * shown by the calculations above! */
    { 
        ulong64 tmp;
        tmp.high = result >> 32;
        tmp.low = result;
        return tmp;
    }
#endif /* COMPILER_SUPPORTS_LONG */
}

/*=========================================================================
 * FUNCTION:      Calendar_md()
 * TYPE:          machine-specific implementation of native function
 * OVERVIEW:      Initializes the calendar fields, which represent the 
 *                Calendar related attributes of a date. 
 * INTERFACE:
 *   parameters:  none
 *   returns:     none
 * AUTHOR:        Tasneem Sayeed
 *=======================================================================*/

unsigned long *
Calendar_md(void)
{
    clock_t clk;
    struct tm *tmP = NULL;

    /* initialize the date array */
    memset(&date, 0, MAXCALENDARFLDS);

    /* returns the time */ 
    time(&clk);  

    /* returns pointer to tm struct */
    tmP = localtime(&clk); 
    
    /* initialize the calendar fields */
    date[YEAR] = tmP->tm_year;
    date[MONTH] = tmP->tm_mon;
    date[DAY_OF_MONTH] = tmP->tm_mday;
    date[HOUR] = tmP->tm_hour;
    date[MINUTE] = tmP->tm_min;
    date[SECOND] = tmP->tm_sec;
    /* We cannot accurately determine this value,
     * so set it to 0 for now
     */
    date[MILLISECOND] = 0;
    return date;
}


#if NOSOCKETS

void Java_com_sun_cldc_io_NetworkConnectionBase_initializeInternal(){}
void Java_com_sun_cldc_io_j2me_comm_Protocol_native_1available(){}
void Java_com_sun_cldc_io_j2me_comm_Protocol_native_1close(){}
void Java_com_sun_cldc_io_j2me_comm_Protocol_native_1openByName(){}
void Java_com_sun_cldc_io_j2me_comm_Protocol_native_1openByNumber(){}
void Java_com_sun_cldc_io_j2me_comm_Protocol_native_1readBytes(){}
void Java_com_sun_cldc_io_j2me_comm_Protocol_native_1writeBytes(){}
void Java_com_sun_cldc_io_j2me_datagram_Protocol_close0(){}
void Java_com_sun_cldc_io_j2me_datagram_Protocol_getIpNumber(){}
void Java_com_sun_cldc_io_j2me_datagram_Protocol_getMaximumLength0(){}
void Java_com_sun_cldc_io_j2me_datagram_Protocol_getNominalLength0(){}
void Java_com_sun_cldc_io_j2me_datagram_Protocol_open0(){}
void Java_com_sun_cldc_io_j2me_datagram_Protocol_receive0(){}
void Java_com_sun_cldc_io_j2me_datagram_Protocol_registerCleanup(){}
void Java_com_sun_cldc_io_j2me_datagram_Protocol_send0(){}
void Java_com_sun_cldc_io_j2me_serversocket_Protocol_accept(){}
void Java_com_sun_cldc_io_j2me_serversocket_Protocol_close(){}
void Java_com_sun_cldc_io_j2me_serversocket_Protocol_open0(){}
void Java_com_sun_cldc_io_j2me_serversocket_Protocol_registerCleanup(){}
void Java_com_sun_cldc_io_j2me_socket_Protocol_available0(){}
void Java_com_sun_cldc_io_j2me_socket_Protocol_close0(){}
void Java_com_sun_cldc_io_j2me_socket_Protocol_open0(){}
void Java_com_sun_cldc_io_j2me_socket_Protocol_read0(){}
void Java_com_sun_cldc_io_j2me_socket_Protocol_registerCleanup(){}
void Java_com_sun_cldc_io_j2me_socket_Protocol_write0(){}
void Java_com_sun_cldc_io_j2me_datagram_Protocol_getHostByAddr(void) {}

#endif /* NOSOCKETS */

#if MISSING_STAT_CALL

#include <sys/stat.h>

int stat(const char *name, struct stat *sbuf) { 
    int fd = open(name, 0);
    int result;
    if (fd < 0) { 
        return -1;
    }
    result = fstat(fd, sbuf);
    close(fd);
    return result;
}

#endif /* MISSING_STAT_CALL */


