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


#include "../../include/recore/REDictionaryObjectN.h"

/* REObject */
const REUInt32 REDictionaryObjectN::getClassIdentifier() const
{
	return REDictionaryObjectN::classIdentifier();
}

const REUInt32 REDictionaryObjectN::classIdentifier()
{
	static const REUInt32 clasIdentif = REMD5Generator::generateFromString("REDictionaryObjectN");
	return clasIdentif;
}

REBOOL REDictionaryObjectN::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REDictionaryObjectN::classIdentifier() == classIdentifier) || REObject::isImplementsClass(classIdentifier));
}

REBOOL REDictionaryObjectN::isEqual(REObject * anotherObject)
{
	if (REObject::isEqual(anotherObject)) { return true; }
	
	if (anotherObject)
	{
		if (anotherObject->isImplementsClass(REDictionaryObjectN::classIdentifier())) 
		{
			REDictionaryObjectN * dict = REPtrCast<REDictionaryObjectN, REObject>(anotherObject);
			return REDictionary::isEqualToDictionary(*dict);
		}
	}
	return false;
}

void REDictionaryObjectN::onReleased()
{
	this->clearPairs();
}

REBOOL REDictionaryObjectN::initializeFromJSONData(const REUByte * jsonData, 
												   const REUInt32 jsonDataSize)
{
	this->clearPairs();
	if (jsonData && jsonDataSize)
	{
		return this->readJSONData(jsonData, jsonDataSize, REPtrTypeREObject);
	}
	return false;
}

REBOOL REDictionaryObjectN::setValueObject(REObject * newValue, const char * key)
{
	RETypedPtr v;
	v.setREObject<REObject>(newValue, REPtrTypeREObject);
	
	return REDictionary::setValue(v, key);
}

REBOOL REDictionaryObjectN::setValueObject(REObject * newValue, REObject * key)
{
	RETypedPtr v, k;
	
	v.setREObject<REObject>(newValue, REPtrTypeREObject);
	k.setREObject<REObject>(key, REPtrTypeREObject);
	
	return REDictionary::setValue(v, k);
}

REObject * REDictionaryObjectN::valueObjectForKey(const char * key) const
{
	return REDictionary::valueForKey(key).getREObject<REObject>();
}

REDictionaryObjectN::REDictionaryObjectN() : REDictionary(), REObject()
{
	
}

REDictionaryObjectN::~REDictionaryObjectN()
{
	this->clearPairs();
}

REDictionaryObjectN * REDictionaryObjectN::create()
{
	return (new REDictionaryObjectN());
}

