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

class REDictionaryObjectNPrivate 
{	
public:
	
};

REBOOL REDictionaryObjectN::setValue(REObject * newValue, REObject * key)
{
	return this->REDictionary::setValue(RETypedPtr(REPtrCast<void, REObject>(newValue) ,REPtrTypeREObject), 
										RETypedPtr(REPtrCast<void, REObject>(key) ,REPtrTypeREObject));
}

REObject * REDictionaryObjectN::valueForKey(REObject * key)
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

REBOOL REDictionaryObjectN::initializeFromJSONData(const REUByte * jsonData, const REUInt32 jsonDataSize)
{
	this->clearPairs();
	if (jsonData && jsonDataSize)
	{
		return this->readJSONData(jsonData, jsonDataSize, REPtrTypeREObject);
	}
	return false;
}

REDictionaryObjectN::REDictionaryObjectN() : REDictionary(), REObject()
{
	
}

REDictionaryObjectN::~REDictionaryObjectN()
{
	
}

REDictionaryObjectN * REDictionaryObjectN::create()
{
	REDictionaryObjectN * d = new REDictionaryObjectN();
	return d;
}

REDictionaryObjectN * REDictionaryObjectN::createWithCapacity(const REUInt32 capacity)
{
	REDictionaryObjectN * d = new REDictionaryObjectN();
	if (d) 
	{
		d->_pairs.setCapacity(capacity);
		return d;
	}
	return NULL;
}


