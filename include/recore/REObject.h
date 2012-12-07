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


#ifndef __REOBJECT_H__
#define __REOBJECT_H__

#include "RECommonHeader.h"
#include "REMD5Generator.h"

/// Base class for objects.
class __RE_PUBLIC_CLASS_API__ REObject
{
private:
	REUIdentifier _reObjectIdentifier;
	REUInt32 _reObjectRetainCount;

protected:
	/// Deletes object
    static void Delete(REObject * object);
	
	/// Constucts object with retain counter value 1 and assigning object identifier
	REObject();
	
	/// Destructor.
	virtual ~REObject();
public:
	/// Return type: REUIdentifier that is "unsigned int (32/64 bit, dependes of compiled platform)"
	/// Return value is integer representation of "this" pointer when REObject creating.
	const REUIdentifier GetObjectIdentifier() const { return _reObjectIdentifier; }
	
	/// Returns integer identifier of class. Each new class that inherits REObject must implement this method.
	/// Return value is md5 value of name class. Usialy return ::ClassIdentifier() value.
	virtual const REUInt32 GetClassIdentifier() const;
	
	/// Check object is implements class by it's identifier or object is instance of the class by it's identifier.
	/// Must be overided in all REObject subclasses.
	/// Default implementation of REObject compares REObject::ClassIdentifier() with passed argument.
	/// If your class implements some abstract class you need also compare passed argument with
	/// class identifier generated from it's name using REObject::GenerateClassIdentifierFromClassName(...) method.
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	
	/// Returns integer identifier of class. Each new class that REObject must have it's own method that is
	/// md5 value of name class. Using REMD5Generator::GenerateFromString(const char * s);
	static const REUInt32 ClassIdentifier();
	
	/// Generates integer identifier of class using it's name.
	/// Return value is md5 value of name class.
	/// Example: const REUInt32 classID = REObject::GenerateClassIdentifierFromClassName("REObject");
	static const REUInt32 GenerateClassIdentifierFromClassName(const char * className);
	
	/// Checking equals with onother object. Default implementation check equality of object identifiers.
	/// If anotherObject is NULL than not equal
	virtual REBOOL IsEqual(REObject * anotherObject);
	
	/// Incrementing retain counter of object.
	REObject & Retain();
	
	/// Decrementing retain counter of object.
	/// If counter is 0 than object placed to auto release pool
	/// and if successfully placed called OnReleased() method.
	void Release();
	
	/// Returns retain counter of object.
	const REUInt32 GetRetainCount() const { return _reObjectRetainCount; }

	/// Called when object released and added to auto release pool.
	/// Good place to release any used objects, stop any internal animations etc.
	/// Default implementation do nothing, but if you override this method
	/// don't forged call this method of base object.
	virtual void OnReleased() { }
};


#endif /* __REOBJECT_H__ */

