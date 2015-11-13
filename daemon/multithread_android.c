/*****************************************************************************
 *
 * This program is free software ; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * $Id: multithread_win32.c 343 2005-11-16 20:11:07Z picard $
 *
 * The Core Pocket Media Player
 * Copyright (c) 2004-2005 Gabor Kovacs
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

//time usleep()
#include <unistd.h>
#include <pthread.h>
#include "multithread.h"

void* LockCreate()
{
	pthread_mutexattr_t attr;

	pthread_mutex_t *p_mutex = malloc(sizeof(pthread_mutex_t));

	int                 i_result;

	pthread_mutexattr_init( &attr );

	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	i_result = pthread_mutex_init( p_mutex, &attr );

	pthread_mutexattr_destroy( &attr );

	if(i_result)
	{
		free(p_mutex);
	
		return NULL;
	}

	return p_mutex;
}

void LockDelete(void* p)
{
	//DEBUG_MSG(DEBUG_SYS,T("multithread_android::LockDelete"));
	if (p)
	{
		pthread_mutex_destroy(p);
		free(p);
	}
}

void LockEnter(void* p)
{
	//DEBUG_MSG(DEBUG_SYS,T("multithread_android::LockEnter"));
	if (p)
	{
		pthread_mutex_lock(p);
	}
}

void LockLeave(void* p)
{
	//DEBUG_MSG(DEBUG_SYS,T("multithread_android::LockLeave"));
	if (p)
	{
		pthread_mutex_unlock(p);
	}
}

int ThreadPriority(void* Thread,int Priority)
{
	
	return 0;
}

int EventWait(void* Handle,int Time,void* mutex)
{
	if(Time>0)
	{
		struct timespec time;
		struct timeval date;
		int64_t datams;

		bzero(&time, sizeof(struct timespec));
		bzero(&date, sizeof(struct timeval));

		gettimeofday( &date, NULL );

		datams = (((int64_t)date.tv_sec)*1000000 + date.tv_usec)/1000+Time;

		time.tv_sec = datams/1000;
		time.tv_nsec = datams%1000 * 1000000;
		pthread_cond_timedwait((pthread_cond_t*)Handle, (pthread_mutex_t*)mutex,&time);
	}
	else
	{
		pthread_cond_wait((pthread_cond_t*)Handle, (pthread_mutex_t*)mutex );
	}

	return 1;
}
//Fixme
void* EventCreate(int ManualReset,int InitState)
{
	pthread_condattr_t attr;
	pthread_cond_t     *cond = malloc(sizeof(pthread_cond_t));

	pthread_cond_init (cond, &attr);
	return cond;
}
void EventSet(void* Handle)
{
	if(Handle)
	{
		pthread_cond_signal((pthread_cond_t*)Handle);
	}
}
void EventReset(void* Handle)
{
	//ResetEvent(Handle);
}
void EventClose(void* Handle)
{
	if(Handle)
	{
		pthread_cond_destroy((pthread_cond_t*)Handle);
		free((pthread_cond_t*)Handle);
	}
}

int ThreadId()
{
	return pthread_self();
}
void ThreadSleep(int Time)
{
	usleep(Time*1000);
}
void ThreadTerminate(void* Handle)
{
	if(Handle)
	{
		pthread_join(*(pthread_t*)Handle,NULL);
		free((pthread_t*)Handle);
	}
}

void* ThreadCreate(void* (*Start)(void*),void* Parameter,int Quantum)
{
	pthread_attr_t attr;
	struct sched_param param;
	int ret = 0;
	pthread_t* thread_id = (pthread_t*)malloc(sizeof(pthread_t));
	int policy = 0;
	int primax = 0;

	pthread_attr_init (&attr);

	ret = pthread_attr_setschedpolicy(&attr,SCHED_RR);

	ret = pthread_attr_getschedpolicy(&attr,&policy);

	primax = sched_get_priority_max(policy);

	param.sched_priority = primax;

	ret = pthread_attr_setschedparam(&attr,&param);

	ret = pthread_create(thread_id, &attr, Start, Parameter );

	return thread_id;
}
