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


#ifndef __REAUTORELEASEPOOL_H__
#define __REAUTORELEASEPOOL_H__

#include "RECommonHeader.h"
#include "REObject.h"
#include "REMainLoopUpdatable.h"
#include "REArray.h"
#include "REThread.h"
#include "REMutex.h"


#if (defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) || defined(__USING_RECORE_DYNAMIC_LIBRARY__)) 
/// Warning message 4251: Class 'REArray<T>' needs to have dll-interface to be used by clients of class.
/// Why disable 4251: Class 'REArray<T>' defined as private field and no clents can access it.
/// To access must use public methods.
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif
#endif


class __RE_PUBLIC_CLASS_API__ REAutoReleasePool : public REObject, public REMainLoopUpdatable
{
private:
	REArray<REObject *> _pool;
	REMutex _updateMutex;
	REUInt32 _index;
	REUInt32 _isBusy;
	
	static REAutoReleasePool * _defaultPool;
	
public:
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	REBOOL AddObject(REObject * autoReleasableObject);

	/* REMainLoopUpdatable */
	virtual void Update(const RETimeInterval currentTime);
	virtual const REUIdentifier GetMainLoopUpdatableIdentifier() const;
	
	REAutoReleasePool();
	virtual ~REAutoReleasePool();
	
	static REAutoReleasePool * GetDefaultPool();
	static void ReleaseDefaultPool();
};

#endif /* __REAUTORELEASEPOOL_H__ */

