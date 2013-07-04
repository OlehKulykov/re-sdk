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


#include "../../include/recore/REDictionary.h"

#include "json/OKJSONParser.h"

#include "../../include/recore/private/REDictionaryJSONCallbacks.h"

static void __REDictionary__SetupJSONReaderCallbacks(OKJSONParserCallbacks * jsonCallbacks,
													 const REPtrType type)
{
	jsonCallbacks->newMem = REDictionaryJSONCallbacks::newMem;
	jsonCallbacks->freeMem = REDictionaryJSONCallbacks::freeMem;
	
	if (type == REPtrTypeREObject)
	{
		jsonCallbacks->createNull = REDictionaryJSONCallbacks::createNullObject;
		jsonCallbacks->createNumberWithBool = REDictionaryJSONCallbacks::createNumberObjectWithBool;
		jsonCallbacks->createNumberWithLongLong = REDictionaryJSONCallbacks::createNumberObjectWithLongLong;
		jsonCallbacks->createNumberWithDouble = REDictionaryJSONCallbacks::createNumberObjectWithDouble;
		jsonCallbacks->createStringWithUTF8 = REDictionaryJSONCallbacks::createStringObjectWithUTF8;
		jsonCallbacks->createArray = REDictionaryJSONCallbacks::createArrayObject;
		jsonCallbacks->createDictionary = REDictionaryJSONCallbacks::createDictionaryObject;
		jsonCallbacks->addToArray = REDictionaryJSONCallbacks::addToArrayObject;
		jsonCallbacks->addToDictionary = REDictionaryJSONCallbacks::addToDictionaryObject;
		jsonCallbacks->deleteObject = REDictionaryJSONCallbacks::deleteREObject;
	}
	else
	{
		jsonCallbacks->createNull = REDictionaryJSONCallbacks::createNull;
		jsonCallbacks->createNumberWithBool = REDictionaryJSONCallbacks::createNumberWithBool;
		jsonCallbacks->createNumberWithLongLong = REDictionaryJSONCallbacks::createNumberWithLongLong;
		jsonCallbacks->createNumberWithDouble = REDictionaryJSONCallbacks::createNumberWithDouble;
		jsonCallbacks->createStringWithUTF8 = REDictionaryJSONCallbacks::createStringWithUTF8;
		jsonCallbacks->createArray = REDictionaryJSONCallbacks::createArray;
		jsonCallbacks->createDictionary = REDictionaryJSONCallbacks::createDictionary;
		jsonCallbacks->addToArray = REDictionaryJSONCallbacks::addToArray;
		jsonCallbacks->addToDictionary = REDictionaryJSONCallbacks::addToDictionary;
		jsonCallbacks->deleteObject = REDictionaryJSONCallbacks::deleteObject;
	}
}

REBOOL REDictionary::readJSONData(const REUByte * jsonData, 
								  const REUInt32 jsonDataSize,
								  const REPtrType type)
{
	REDictionaryJSONCallbacks dictCallbacks;
	OKJSONParserCallbacks jsonCallbacks;
	__REDictionary__SetupJSONReaderCallbacks(&jsonCallbacks, type);
	jsonCallbacks.userData = &dictCallbacks;
	
	PARSED_OBJECT parsedObject = OKJSONParserParse((const uint8_t *)jsonData, (uint32_t)jsonDataSize, &jsonCallbacks);
	if (parsedObject)
	{
		if (type == REPtrTypeREObject)
		{
			RETypedPtr parsedPointer = *((RETypedPtr*)parsedObject);
			if (parsedPointer.getType() == REPtrTypeDictionaryObject)
			{
				REDictionaryObjectN * dict = parsedPointer.getREObject<REDictionaryObjectN>();
				for (REUInt32 i = 0; i < dict->_pairs.count(); i++) 
				{
					REDictionary::Pair newPair;
					newPair.key = dict->_pairs[i].key;
					newPair.value = dict->_pairs[i].value;
					if (!_pairs.add(newPair))
					{
						this->clearPairs();
						return false;
					}
				}
			}
		}
		else
		{
			RETypedPtr parsedPointer = *((RETypedPtr*)parsedObject);
			REDictionary * dict = parsedPointer.getDictionary();
			if (dict)
			{
				for (REUInt32 i = 0; i < dict->_pairs.count(); i++) 
				{
					REDictionary::Pair newPair;
					newPair.key = dict->_pairs[i].key;
					newPair.value = dict->_pairs[i].value;
					if (!_pairs.add(newPair))
					{
						this->clearPairs();
						return false;
					}
				}
				return true;
			}
		}
	}
	return false;
}

REBOOL REDictionary::initializeFromJSONData(const REUByte * jsonData, const REUInt32 jsonDataSize)
{
	this->clearPairs();
	if (jsonData && jsonDataSize)
	{
		return this->readJSONData(jsonData, jsonDataSize, REPtrTypeNone);
	}
	return false;
}

REDictionary::Pair * REDictionary::pairForKey(const RETypedPtr & key, REUInt32 * resultIndex) const
{
	for (REUInt32 i = 0; i < _pairs.count(); i++) 
	{
		if ( _pairs[i].key.isEqualToTypedPointer(key) )
		{
			if (resultIndex)
			{
				*resultIndex = i;
			}
			return (&_pairs[i]);
		}
	}
	return NULL;
}

const REUInt32 REDictionary::pairsCount() const
{
	return _pairs.count();
}

REBOOL REDictionary::isEqualToDictionary(const REDictionary & anotherDictionary) const
{
	if (_pairs.count() == anotherDictionary._pairs.count())
	{
		for (REUInt32 i = 0; i < _pairs.count(); i++) 
		{
			REDictionary::Pair * pair = anotherDictionary.pairForKey(_pairs[i].key);
			if (pair)
			{
				if (!_pairs[i].value.isEqualToTypedPointer(pair->value))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

REBOOL REDictionary::setValue(const RETypedPtr & newValue, const RETypedPtr & keyValue)
{
	if (keyValue.isNotEmpty())
	{
		REUInt32 index = RENotFound;
		REDictionary::Pair * pair = this->pairForKey(keyValue, &index);
		if (pair)
		{
			if (newValue.isNotEmpty())
			{
				// set new value for existed key
				pair->value = newValue;
				pair->value.release();
				return true;
			}
			else if (index != RENotFound)
			{
				// new value is empty - remove pair
				pair->value.release();
				pair->key.release();
				return _pairs.removeAt(index);
			}
		}
		else if (newValue.isNotEmpty())
		{
			// add new value as pair
			REDictionary::Pair newPair;
			newPair.key = keyValue;
			newPair.value = newValue;
			return _pairs.add(newPair);
		}
	}
	return false;
}

REBOOL REDictionary::setValue(const RETypedPtr & newValue, const char * key)
{
	if (key) 
	{
		RETypedPtr pkey(new REString(key), REPtrTypeString);
		return this->setValue(newValue, pkey);
	}
	return false;
}

RETypedPtr REDictionary::valueForKey(const char * key) const 
{
	if (key)
	{
		RETypedPtr pkey(new REString(key), REPtrTypeString);
		if (pkey.isNotEmpty())
		{			
			REDictionary::Pair * pair = this->pairForKey(pkey);
			if (pair)
			{
				return pair->value;
			}
		}
	}
	return RETypedPtr();
}

REBOOL REDictionary::removeValue(const char * key)
{
	if (key)
	{
		RETypedPtr pkey(new REString(key), REPtrTypeString);
		if (pkey.isNotEmpty())
		{		
			REUInt32 index = RENotFound;
			REDictionary::Pair * pair = this->pairForKey(pkey, &index);
			if (pair && (index != RENotFound))
			{
				pair->value.release();
				pair->key.release();
				return _pairs.removeAt(index);
			}
		}
	}
	return false;
}

RETypedArray REDictionary::getAllKeys() const
{
	RETypedArray arr(_pairs.count() + 1);
	for (REUInt32 i = 0; i < _pairs.count(); i++)
	{
		arr.add(_pairs[i].key);
	}
	return arr;
}

RETypedArray REDictionary::getAllValues() const
{
	RETypedArray arr(_pairs.count() + 1);
	for (REUInt32 i = 0; i < _pairs.count(); i++)
	{
		arr.add(_pairs[i].value);
	}
	return arr;
}

void REDictionary::clearPairs()
{
	for (REUInt32 i = 0; i < _pairs.count(); i++) 
	{
		_pairs[i].key.release();
		_pairs[i].value.release();
	}
	_pairs.clear();
}

void REDictionary::clear()
{
	this->clearPairs();
}

REDictionary::REDictionary()
{
	
}

REDictionary::~REDictionary()
{
	this->clearPairs();
}

