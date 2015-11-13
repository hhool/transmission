#ifndef _SYS_TIME_H_
#define _SYS_TIME_H_
#include <time.h>

#ifdef	__cplusplus
extern "C" {
#endif

/* Provided for compatibility with code that assumes that
   the presence of gettimeofday function implies a definition
   of struct timezone. */
struct timezone
{
  int tz_minuteswest; /* of Greenwich */
  int tz_dsttime;     /* type of dst correction to apply */
};

/*
   Implementation as per:
   The Open Group Base Specifications, Issue 6
   IEEE Std 1003.1, 2004 Edition

   The timezone pointer arg is ignored.  Errors are ignored.
*/ 
int __cdecl gettimeofday(struct timeval *tv, struct timezone *tz);

#ifdef	__cplusplus
}
#endif


#endif /* _SYS_TIME_H_ */
