/*
 *   Copyright 2012 Kulikov Oleg
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


#include "../../include/renetwork/REURLObject.h"
#include "../../include/recore/REMD5Generator.h"

const REUInt32 REURLObject::GetClassIdentifier() const
{
	return REURLObject::ClassIdentifier();
}

const REUInt32 REURLObject::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REURLObject");
	return clasIdentif;
}

REBOOL REURLObject::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REURLObject::ClassIdentifier() == classIdentifier) ||
			(REObject::GenerateClassIdentifierFromClassName("REURL") == classIdentifier) ||
			REObject::IsImplementsClass(classIdentifier));
}

REBOOL REURLObject::IsEqual(REObject * anotherObject)
{
	if (anotherObject) 
	{
		if (anotherObject->IsImplementsClass(REObject::GenerateClassIdentifierFromClassName("REURL")))
		{
			REURL * url = (REURL *)anotherObject;
			if (this->Length() == url->Length()) 
			{
				if (this->Length()) 
				{
					return (strcmp(this->UTF8String(), url->UTF8String()) == 0);
				}
				// both empty
				return true;
			}
		}
	}
	return false;
}

REURLObject::REURLObject(const REURL & urlValue) : REObject(), REURL(urlValue)
{
	
}

REURLObject::REURLObject(const REString & stringValue) : REObject(), REURL(stringValue)
{
	
}

REURLObject::REURLObject(const wchar_t * stringValue) : REObject(), REURL(stringValue)
{
	
}

REURLObject::REURLObject(const char * stringValue) : REObject(), REURL(stringValue)
{
	
}

REURLObject::~REURLObject()
{
	
}

REURLObject * REURLObject::CreateWithURL(const REURL & urlValue)
{
	if (urlValue.Length()) 
	{
		REURLObject * newObj = new REURLObject(urlValue);
		if (newObj) 
		{
			if (newObj->Length()) 
			{
				return newObj;
			}
			delete newObj;
		}
	}
	return NULL;
}

REURLObject * REURLObject::CreateWithString(const REString & stringValue)
{
	if (!stringValue.IsEmpty()) 
	{
		REURLObject * newObj = new REURLObject(stringValue);
		if (newObj) 
		{
			if (newObj->Length()) 
			{
				return newObj;
			}
			delete newObj;
		}
	}
	return NULL;
}

REURLObject * REURLObject::CreateWithUTF8String(const char * stringValue)
{
	if (stringValue) 
	{
		REURLObject * newObj = new REURLObject(stringValue);
		if (newObj) 
		{
			if (newObj->Length()) 
			{
				return newObj;
			}
			delete newObj;
		}
	}
	return NULL;
}

REURLObject * REURLObject::CreateWithWideString(const wchar_t * stringValue)
{
	if (stringValue) 
	{
		REURLObject * newObj = new REURLObject(stringValue);
		if (newObj) 
		{
			if (newObj->Length()) 
			{
				return newObj;
			}
			delete newObj;
		}
	}
	return NULL;
}



