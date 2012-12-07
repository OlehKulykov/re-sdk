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


#ifndef __REAUTORELEASEPOOLPRIVATE_H__
#define __REAUTORELEASEPOOLPRIVATE_H__

#include "../RECommonHeader.h"
#include "../REObject.h"
#include "../REMainLoopUpdatable.h"
#include "../REArray.h"
#include "../REThread.h"
#include "../REMutex.h"

/*
class REAutoReleasePoolPrivate : public REObject
{
private:
	REArray<REObject *> _pool;
	REMutex _updateMutex;
	REUInt32 _index;
	
	typedef struct _threadPoolStruct
	{
		REUIdentifier threadId;
		REAutoReleasePoolPrivate * pool;
	}
	ThreadPoolStruct;
	static REArray<REAutoReleasePoolPrivate::ThreadPoolStruct> _threadPools;
protected:
	void UpdatePool();
	void LastUpdate();
	REAutoReleasePoolPrivate();
	~REAutoReleasePoolPrivate();
	
public:
	REBOOL AddObject(REObject * autoReleasableObject);
	
	static void Update();
	static REAutoReleasePoolPrivate * GetCurrentThreadPool();
	static REAutoReleasePoolPrivate * GetPoolForThread(const REUIdentifier threadIdentifier);
	static void ThreadFinished(const REUIdentifier threadIdentifier);
};
*/

#endif /* __REAUTORELEASEPOOLPRIVATE_H__ */

