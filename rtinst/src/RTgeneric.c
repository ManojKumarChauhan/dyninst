/*
 * Copyright (c) 1993, 1994 Barton P. Miller, Jeff Hollingsworth,
 *     Bruce Irvin, Jon Cargille, Krishna Kunchithapadam, Karen
 *     Karavanic, Tia Newhall, Mark Callaghan.  All rights reserved.
 * 
 * This software is furnished under the condition that it may not be
 * provided or otherwise made available to, or used by, any other
 * person, except as provided for by the terms of applicable license
 * agreements.  No title to or ownership of the software is hereby
 * transferred.  The name of the principals may not be used in any
 * advertising or publicity related to this software without specific,
 * written prior authorization.  Any use of this software must include
 * the above copyright notice.
 *
 */





/************************************************************************
 * RTgeneric.c: generic timer functions for POSIX systems
************************************************************************/





/************************************************************************
 * header files.
************************************************************************/

#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/resource.h>

#include "kludges.h"
#include "rtinst/h/rtinst.h"

#if defined(hppa1_1_hp_hpux)

#include <sys/pstat.h>
#include <sys/unistd.h>
#define _PROTOTYPES
#else
extern int getrusage (int, struct rusage *);
#endif




/************************************************************************
 * symbolic constants.
************************************************************************/

static const double MILLION = 1000000.0;





/************************************************************************
 * void DYNINSTos_init(void)
 *
 * null initialization for generic operating system.
************************************************************************/

void
DYNINSTos_init(void) {
}





/************************************************************************
 * time64 DYNINSTgetCPUtime(void)
 *
 * get the total CPU time used for "an" LWP of the monitored process.
 * this functions needs to be rewritten if a per-thread CPU time is
 * required.  time for a specific LWP can be obtained via the "/proc"
 * filesystem.
 * return value is in usec units.
************************************************************************/

#if defined(hppa1_1_hp_hpux)
/* 
 * The compilng warning could be eliminated if we use cc instead
 * of gcc. Change it if you think that's good. --ling
 *
 * The cpu time I got in this way is not good. Absolutely 
 * needs improvement 
 */

time64
DYNINSTgetCPUtime(void) {
    time64 now;
    static time64 previous=0;
    struct pst_status pst;
    int target = (int)getpid();

  try_again:
    if (pstat_getproc(&pst, sizeof(pst), (size_t)0, target) != -1) {
      now = (time64)pst.pst_cptickstotal + (time64)pst.pst_cpticks;
      now *= (time64)10000;
      if (now<previous) {
        goto try_again;
      }
      previous=now;
      return(now);
    }
    else {
      perror("pstat_getproc");
      abort();
  }
}


#else

time64
DYNINSTgetCPUtime(void) {
    time64 now;
    static time64 previous=0;
    struct rusage ru;

try_again:    
    if (!getrusage(RUSAGE_SELF, &ru)) {
      now = (time64)ru.ru_utime.tv_sec + (time64)ru.ru_stime.tv_sec;
      now *= (time64)1000000;
      now += (time64)ru.ru_utime.tv_usec + (time64)ru.ru_stime.tv_usec;
      if (now<previous) {
        goto try_again;
      }
      previous=now;
      return(now);
    }
    else {
      perror("getrusage");
      abort();
    }
}

#endif



/************************************************************************
 * time64 DYNINSTgetWalltime(void)
 *
 * get the total walltime used by the monitored process.
 * return value is in usec units.
************************************************************************/

time64
DYNINSTgetWalltime(void) {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1) {
        perror("gettimeofday");
        abort();
    }
    return ((time64)tv.tv_sec*(time64)1000000 + (time64)tv.tv_usec);
}
