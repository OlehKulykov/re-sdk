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
const REUInt32 RENumberObject::getClassIdentifier() const
{
	return RENumberObject::classIdentifier();
}

const REUInt32 RENumberObject::classIdentifier()
{
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("RENumberObject");
	return clasIdentif;
}

REBOOL RENumberObject::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((RENumberObject::classIdentifier() == classIdentifier) || REObject::isImplementsClass(classIdentifier) ||
			(REObject::generateClassIdentifierFromClassName("RENumber") == classIdentifier));
}

REBOOL RENumberObject::isEqual(REObject * anotherObject)
{
	if (REObject::isEqual(anotherObject)) { return true; }
	
	if (anotherObject) 
	{
		if (anotherObject->getClassIdentifier() == RENumberObject::classIdentifier()) 
		{
			return RENumber::isEqualToNumber(*(RENumberObject*)anotherObject);
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




RENumberObject * RENumberObject::create()
{
	return (new RENumberObject());
}

RENumberObject * RENumberObject::createWithBool(REBOOL newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setBoolValue(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::createWithByte(REByte newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setByteValue(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::createWithUByte(REUByte newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setUByteValue(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::createWithInt16(REInt16 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setInt16Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::createWithUInt16(REUInt16 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setUInt16Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::createWithInt32(REInt32 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setInt32Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::createWithUInt32(REUInt32 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setUInt32Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::createWithInt64(REInt64 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setInt64Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::createWithUInt64(REUInt64 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setUInt64Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::createWithPointer(void * newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setPointerValue(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::createWithFloat32(REFloat32 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setFloat32Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::createWithFloat64(REFloat64 newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setFloat64Value(newValue);
		return newNumber;
	}
	return NULL;
}

RENumberObject * RENumberObject::createWithTimeInterval(RETimeInterval newValue)
{
	RENumberObject * newNumber = new RENumberObject();
	if (newNumber) 
	{
		newNumber->setTimeIntervalValue(newValue);
		return newNumber;
	}
	return NULL;
}


RENumberObject * RENumberObject::createWithStringPresentation(const char * stringPresentation)
{
	if (stringPresentation) 
	{
		RENumberObject * newNumber = new RENumberObject();
		if (newNumber) 
		{
			if (newNumber->setValueFromString(stringPresentation)) 
			{
				return newNumber;
			}
			
			delete newNumber;
		}
	}
	return NULL;
}





