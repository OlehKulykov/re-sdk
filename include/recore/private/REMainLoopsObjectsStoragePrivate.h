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


#ifndef __REMAINLOOPSOBJECTSSTORAGEPRIVATE_H__
#define __REMAINLOOPSOBJECTSSTORAGEPRIVATE_H__

#include "../RECommonHeader.h"
#include "../REThread.h"
#include "../REMutex.h"
#include "../REMainLoopUpdatable.h"


class REMainLoopsObjectsStoragePrivate
{
private:
	REArray<REMainLoopUpdatable *> _objects;
	REMutex _updateMutex;
	
	REBOOL add(REMainLoopUpdatable * object);
	REBOOL remove(REMainLoopUpdatable * object);
	void update(const RETimeInterval time);
	REBOOL isEmptyAndIDLE() const;
	
	static REUInt32 index(REArray<REMainLoopUpdatable *> * arr, const REUIdentifier objectId);
	
	REMainLoopsObjectsStoragePrivate();
	~REMainLoopsObjectsStoragePrivate();
	
	static REMainLoopsObjectsStoragePrivate * getStorage();
	static void releaseStorage();
	static REMainLoopsObjectsStoragePrivate * _storage;
public:
	static REBOOL addObject(REMainLoopUpdatable * object);
	static REBOOL removeObject(REMainLoopUpdatable * object);
	static void updateStorage(const RETimeInterval time);
};


#endif /* __REMAINLOOPSOBJECTSSTORAGEPRIVATE_H__ */


