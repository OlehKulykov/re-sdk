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


#include "../../include/recore/RENumberObject.h"

/* REObject */
const REUInt32 RENumberObject::GetClassIdentifier() const
{
	return RENumberObject::ClassIdentifier();
}

const REUInt32 RENumberObject::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("RENumberObject");
	return clasIdentif;
}

REBOOL RENumberObject::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((RENumberObject::ClassIdentifier() == classIdentifier) || REObject::IsImplementsClass(classIdentifier) ||
			(REObject::GenerateClassIdentifierFromClassName("RENumber") == classIdentifier));
}

REBOOL RENumberObject::IsEqual(REObject * anotherObject)
{
	if (REObject::IsEqual(anotherObject)) { return true; }
	
	if (anotherObject) 
	{
		if (anotherObject->GetClassIdentifier() == RENumberObject::ClassIdentifier()) 
		{
			return RENumber::IsEqualToNumber(*(RENumberObject*)anotherObject);
		}
	}
	return false;
}

RENumberObject::RENumberObject() : RENumber(), REObject()
{
	
}

RENumberObject::~RENumberObject()
{
	
}




RENumberObject * RENumberObject::CreateWithBool(REBOOL newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetBoolValue(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::CreateWithByte(REByte newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetByteValue(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::CreateWithUByte(REUByte newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetUByteValue(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::CreateWithInt16(REInt16 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetInt16Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::CreateWithUInt16(REUInt16 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetUInt16Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::CreateWithInt32(REInt32 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetInt32Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::CreateWithUInt32(REUInt32 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetUInt32Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::CreateWithInt64(REInt64 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetInt64Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::CreateWithUInt64(REUInt64 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetUInt64Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::CreateWithPointer(void * newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetPointerValue(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::CreateWithFloat32(REFloat32 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetFloat32Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::CreateWithFloat64(REFloat64 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetFloat64Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::CreateWithTimeInterval(RETimeInterval newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->SetTimeIntervalValue(newValue);
		return newNumber;
	}
	return NULL;
}


RENumberObject * RENumberObject::CreateWithStringPresentation(const char * stringPresentation)
{
	if (stringPresentation) 
	{
		RENumberObject * newNumber = new RENumberObject();
		if (newNumber) 
		{
			if (newNumber->SetValueFromString(stringPresentation)) 
			{
				return newNumber;
			}
			
			delete newNumber;
		}
	}
	return NULL;
}





