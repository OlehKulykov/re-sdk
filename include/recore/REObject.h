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


#ifndef __REOBJECT_H__
#define __REOBJECT_H__

#include "RECommonHeader.h"
#include "RESerializable.h"


/// Base class for objects.
class __RE_PUBLIC_CLASS_API__ REObject
{
private:
	REUIdentifier _reObjectIdentifier;
	REUInt32 _reObjectRetainCount;

protected:
	/// Called when object released and added to auto release pool.
	/// Good place to release any used objects, stop any internal animations etc.
	/// Default implementation do nothing, but if you override this method
	/// don't forged call this method of base object.
	virtual void onReleased();
	
	/// Constucts object with retain counter value 1 and assigning object identifier
	REObject();
	
	/// Destructor.
	virtual ~REObject();
	
	/// Deletes object 
    static void deleteObject(REObject * object);
public:	
	/// Return type: REUIdentifier that is "unsigned int (32/64 bit, dependes of compiled platform)"
	/// Return value is integer representation of "this" pointer when REObject creating.
	const REUIdentifier objectIdentifier() const;
		
	/// Checking equals with onother object. Default implementation check equality of object identifiers.
	/// If anotherObject is NULL than not equal
	virtual REBOOL isEqual(REObject * anotherObject);
	
	/// Incrementing retain counter of object.
	REObject & retain();
	
	/// Decrementing retain counter of object.
	/// If counter is 0 than object placed to auto release pool
	/// and if successfully placed called OnReleased() method.
	void release();
	
	/// Returns retain counter of object.
	const REUInt32 retainCount() const;
};


#endif /* __REOBJECT_H__ */

