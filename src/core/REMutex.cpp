/*
 *   Copyright 2012 - 2013 Kulykov Oleh
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */


#include "../../include/recore/REMutex.h"
#include "../../include/recore/REMem.h"

#ifdef __RE_USING_PTHREADS__
#include <pthread.h>
#endif

REBOOL REMutex::Init(const REMutexType type)
{
#if defined(__RE_USING_PTHREADS__) 
	if (_pthreadMutexPtr) { return true; }
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	if (_mutexHANDLE) { return true; }
#endif	
	
	REBOOL isInit = false;
	
#if defined(__RE_USING_PTHREADS__) 
	pthread_mutex_t * m = (pthread_mutex_t *)REMem::Malloc(sizeof(pthread_mutex_t));
	if (m) 
	{
		pthread_mutexattr_t attr;
		if (pthread_mutexattr_init(&attr) == 0) 
		{
			int setTypeResult = -1;
			switch (type) 
			{
				case REMutexTypeNormal:
					setTypeResult = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
					break;
				case REMutexTypeRecursive:
					setTypeResult = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
					break;	
				default:
					break;
			}
			if (setTypeResult == 0) 
			{
				if (pthread_mutex_init(m, &attr) == 0)
				{
					isInit = true;
					_successfulLocks = 0;
				}
			}
			pthread_mutexattr_destroy(&attr);
		}
		
		if (isInit)
		{
			_pthreadMutexPtr = m;
		}
		else
		{
			REMem::Free(m);
		}
	}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	HANDLE h = CreateMutex(NULL, FALSE, NULL);
	if (h) 
	{
		_mutexHANDLE = h;
		isInit = true;
		_successfulLocks = 0;
	}
#endif	
	
	return isInit;
}

REBOOL REMutex::Lock()
{
#if defined(__RE_USING_PTHREADS__) 
	if (_pthreadMutexPtr) 
	{
		pthread_mutex_t * m = (pthread_mutex_t *)_pthreadMutexPtr;
		if (pthread_mutex_lock(m) == 0)
		{
			_successfulLocks++;
			return true;
		}
	}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	if (_mutexHANDLE)
	{
		const DWORD r = WaitForSingleObject(_mutexHANDLE, INFINITE);
		if (r != WAIT_FAILED)
		{
			_successfulLocks++;
			return true;
		}
	}
#endif	
	
	return false;
}

REBOOL REMutex::Unlock()
{
#if defined(__RE_USING_PTHREADS__) 
	if (_pthreadMutexPtr) 
	{
		pthread_mutex_t * m = (pthread_mutex_t *)_pthreadMutexPtr;
		if (pthread_mutex_unlock(m) == 0)
		{
			_successfulLocks--;
			return true;
		}
	}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	if (_mutexHANDLE)
	{
		if (ReleaseMutex(_mutexHANDLE))
		{
			_successfulLocks--;
			return true;
		}
	}
#endif		
	
	return false;
}

REBOOL REMutex::IsLocked() const
{
	return (_successfulLocks != 0);
}

REBOOL REMutex::IsInitialized() const
{
#if defined(__RE_USING_PTHREADS__) 
	return (_pthreadMutexPtr != NULL);
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	return (_mutexHANDLE != (HANDLE)0);
#else
	return false;
#endif	
}

REMutex::REMutex()
#if defined(__RE_USING_PTHREADS__) 
: 	_pthreadMutexPtr(NULL)
#elif defined(__RE_USING_WINDOWS_THREADS__) 
:	_mutexHANDLE((HANDLE)0)
#endif
,_successfulLocks(0)
{
	
}

REMutex::~REMutex()
{
#if defined(__RE_USING_PTHREADS__) 
	if (_pthreadMutexPtr) 
	{
		pthread_mutex_t * m = (pthread_mutex_t *)_pthreadMutexPtr;
		pthread_mutex_destroy(m);
		REMem::Free(m);
	}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	if (_mutexHANDLE) 
	{
		CloseHandle(_mutexHANDLE);
	}
#endif	
}

