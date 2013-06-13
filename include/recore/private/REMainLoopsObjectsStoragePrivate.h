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
	
	REBOOL Add(REMainLoopUpdatable * object);
	REBOOL Remove(REMainLoopUpdatable * object);
	void Update(const RETimeInterval time);
	REBOOL IsEmptyAndIDLE() const;
	
	static REUInt32 Index(REArray<REMainLoopUpdatable *> * arr, const REUIdentifier objectId);
	
	REMainLoopsObjectsStoragePrivate();
	~REMainLoopsObjectsStoragePrivate();
	
	static REMainLoopsObjectsStoragePrivate * GetStorage();
	static void ReleaseStorage();
	static REMainLoopsObjectsStoragePrivate * _storage;
public:
	static REBOOL AddObject(REMainLoopUpdatable * object);
	static REBOOL RemoveObject(REMainLoopUpdatable * object);
	static void UpdateStorage(const RETimeInterval time);
};


#endif /* __REMAINLOOPSOBJECTSSTORAGEPRIVATE_H__ */


