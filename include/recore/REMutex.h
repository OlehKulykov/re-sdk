/*
 *   Copyright 2012 Kulykov Oleh
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


#ifndef __REMUTEX_H__
#define __REMUTEX_H__

#include "RECommonHeader.h"

/// Enums type of mutexes.
typedef enum _reMutexType
{
	/// Mutex is not initialized.
	REMutexTypeNone = 0,
	
	/// This type of mutex does not detect deadlock. 
	/// An attempt to relock this mutex without first unlocking it deadlocks.
	/// Attempting to unlock a mutex locked by a different thread results in undefined behavior. 
	/// Attempting to unlock an unlocked mutex results in undefined behavior.
	REMutexTypeNormal = 1,
	
	/// A thread attempting to relock this mutex without first unlocking it succeeds in locking the mutex. 
	/// The relocking deadlock that can occur with mutexes of type NORMAL cannot occur with this type of mutex. 
	/// Multiple locks of this mutex require the same number of unlocks to release the mutex before another thread can acquire the mutex. 
	/// An attempt to unlock a mutex that another thread has locked returns with an error. 
	/// An attempt to unlock an unlocked mutex returns with an error.
	REMutexTypeRecursive = 2
	
} 
/// Enums type of mutexes.
REMutexType;

#ifndef __RE_USING_PTHREADS__
#ifdef __RE_OS_WINDOWS__
#ifndef __RE_USING_WINDOWS_THREADS__
#define __RE_USING_WINDOWS_THREADS__
#endif
#include <Windows.h>
#endif
#endif

/// Class of thread mutex.
class __RE_PUBLIC_CLASS_API__ REMutex
{
private:
#if defined(__RE_USING_PTHREADS__)	
	void * _pthreadMutexPtr;
#elif defined(__RE_USING_WINDOWS_THREADS__)	
	HANDLE _mutexHANDLE;
#endif
	REUInt32 _successfulLocks;
	
public:
	/// Checks is mutex initialized.
	REBOOL IsInitialized() const;
	
	/// Initializes mutex with selected type.
	/// Returns 'true' if successfuly initialized or allready initialized.
	REBOOL Init(const REMutexType type);
	
	/// Locks mutex. Returns 'true' if success and 'false' - if fail.
	REBOOL Lock();
	
	/// Unlocks mutex. Returns 'true' if success and 'false' - if fail.
	REBOOL Unlock();
	
	/// Returns value indicated that mutex is successfuly locked.
	REBOOL IsLocked() const;
	
	REMutex();
	virtual ~REMutex();
};

#endif /* __REMUTEX_H__ */
