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


#include "../../include/recore/REDictionaryObject.h"

class REDictionaryObjectPrivate 
{	
public:
	
};

/* REObject */
const REUInt32 REDictionaryObject::getClassIdentifier() const
{
	return REDictionaryObject::classIdentifier();
}

const REUInt32 REDictionaryObject::classIdentifier()
{
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("REDictionaryObject");
	return clasIdentif;
}

REBOOL REDictionaryObject::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REDictionaryObject::classIdentifier() == classIdentifier) || 
			REObject::isImplementsClass(classIdentifier) ||
			(REObject::generateClassIdentifierFromClassName("REDictionary") == classIdentifier));
}

REBOOL REDictionaryObject::isEqual(REObject * anotherObject)
{
	if (anotherObject) 
	{
		if (anotherObject->isImplementsClass(REDictionaryObject::classIdentifier())) 
		{
			REDictionaryObject * dict = anotherObject->casted<REDictionaryObject>();
			dict = NULL;
		}
	}
	return false;
}

REBOOL REDictionaryObject::setObject(REObject * newValue, REObject * key)
{
	return this->setValue(RETypedPtr(REPtrCast<void, REObject>(newValue) ,REPtrTypeREObject), 
						  RETypedPtr(REPtrCast<void, REObject>(key) ,REPtrTypeREObject));
}

REObject * REDictionaryObject::objectForKey(REObject * key)
{
	if (key)
	{
		RETypedPtr pkey(REPtrCast<void, REObject>(key) ,REPtrTypeREObject);
		REDictionary::Pair * pair = this->pairForKey(pkey);
		if (pair)
		{
			return pair->value.getREObject();
		}
	}
	return NULL;
}

REBOOL REDictionaryObject::initializeFromJSONData(const REUByte * jsonData, const REUInt32 jsonDataSize)
{
	this->clearPairs();
	if (jsonData && jsonDataSize)
	{
		return this->readJSONData(jsonData, jsonDataSize, REPtrTypeREObject);
	}
	return false;
}

REDictionaryObject::REDictionaryObject() : REDictionary(), REObject()
{
	
}

REDictionaryObject::~REDictionaryObject()
{
	
}

REDictionaryObject * REDictionaryObject::create()
{
	REDictionaryObject * d = new REDictionaryObject();
	return d;
}

REDictionaryObject * REDictionaryObject::createWithCapacity(const REUInt32 capacity)
{
	REDictionaryObject * d = new REDictionaryObject();
	if (d) 
	{
		d->_pairs.setCapacity(capacity);
		return d;
	}
	return NULL;
}


