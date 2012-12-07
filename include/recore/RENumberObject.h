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


#ifndef __RENUMBEROBJECT_H__
#define __RENUMBEROBJECT_H__

#include "RECommonHeader.h"
#include "REObject.h"
#include "RENumber.h"

class __RE_PUBLIC_CLASS_API__ RENumberObject : public RENumber, public REObject
{
protected:
	RENumberObject();
	virtual ~RENumberObject();
	
public:
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual REBOOL IsEqual(REObject * anotherObject);
	
	/// Creates and return new number object with boolean value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithBool(REBOOL newValue);
	
	/// Creates and return new number object with byte value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithByte(REByte newValue);
	
	/// Creates and return new number object with unsigned byte value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithUByte(REUByte newValue);
	
	/// Creates and return new number object with short int value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithInt16(REInt16 newValue);
	
	/// Creates and return new number object with unsigned short int value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithUInt16(REUInt16 newValue);
	
	/// Creates and return new number object with int value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithInt32(REInt32 newValue);
	
	/// Creates and return new number object with unsigned int value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithUInt32(REUInt32 newValue);
	
	/// Creates and return new number object with long int value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithInt64(REInt64 newValue);
	
	/// Creates and return new number object with unsigned long value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithUInt64(REUInt64 newValue);
	
	/// Creates and return new number object with pointer value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithPointer(void * newValue);
	
	/// Creates and return new number object with float value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithFloat32(REFloat32 newValue);
	
	/// Creates and return new number object with double value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithFloat64(REFloat64 newValue);
	
	/// Creates and return new number object with time interval value or NULL on error.
	/// After using this number object call Release().
	static RENumberObject * CreateWithTimeInterval(RETimeInterval newValue);
	
	/// Creates and return new number object with string presentation value or NULL on error.
	/// Example: CreateWithStringPresentation("-513.5"); CreateWithStringPresentation("342");
	/// After using this number object call Release().
	static RENumberObject * CreateWithStringPresentation(const char * stringPresentation);
};


#endif /* __RENUMBEROBJECT_H__ */


