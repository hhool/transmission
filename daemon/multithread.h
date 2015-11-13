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
 * $Id: multithread.h 343 2005-11-16 20:11:07Z picard $
 *
 * The Core Pocket Media Player
 * Copyright (c) 2004-2005 Gabor Kovacs
 *
 ****************************************************************************/

#ifndef __MULTITHREAD_H
#define __MULTITHREAD_H

#ifdef __cplusplus
extern "C" {
#endif
void* LockCreate();
void LockDelete(void*);
void LockEnter(void*);
void LockLeave(void*);


void* ThreadCreate(void*(*Start)(void*) ,void* Parameter,int Quantum);
void ThreadTerminate(void*);
int ThreadPriority(void*, int); //0:normal 1:below normal -1:above normal -100:time critical
void ThreadSleep(int Tick);
int ThreadId();

void* EventCreate(int ManualReset,int InitState);
void EventSet(void*);
void EventReset(void*);
void EventClose(void*);
int EventWait(void*,int Tick,void*);
#ifdef __cplusplus
}
#endif
#endif
