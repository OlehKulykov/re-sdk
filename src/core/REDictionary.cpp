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
#include "../../include/recore/REMutableString.h"
#include "../../include/recore/REBase64.h"

#include "json/OKJSONParser.h"

#include "../../include/recore/private/REDictionaryJSONCallbacks.h"

class REDictionaryJSONGeneratorPrivate 
{	
private:
	void addValueKey(const RETypedPtr & value, const RETypedPtr & key);
	void addNumber(const RENumber & num);
	void addString(const REString & str);
	void addArray(const RETypedArray & array);
	void addNull();
	void addDate(const REDate & date);
	void addBuffer(const REBuffer & buffer);
	void addPtr(const RETypedPtr & ptr);
public:
	REMutableString json;
	void addDict(const REDictionary & dict);
};

void REDictionaryJSONGeneratorPrivate::addBuffer(const REBuffer & buffer)
{
	if (buffer.getSize() > 0)
	{
		REString b64Str;
		REBase64 b64;
		if (b64.bufferToBase64String(buffer, &b64Str))
		{
			if (b64Str.isNotEmpty())
			{
				json.append(b64Str.getChars(), b64Str.getLength());
				return;
			}
		}
	}
	json.append("\"\"", 2);
}

void REDictionaryJSONGeneratorPrivate::addDate(const REDate & date)
{
	this->addString(date.toString());
}

void REDictionaryJSONGeneratorPrivate::addNull()
{
	json.append("null", 4);
}

void REDictionaryJSONGeneratorPrivate::addPtr(const RETypedPtr & ptr)
{
	switch (ptr.getType()) 
	{
		case REPtrTypeArray:
			this->addArray(*ptr.getArray());
			break;
		case REPtrTypeDictionary:
			this->addDict(*ptr.getDictionary());
			break;
		case REPtrTypeNumber:
			this->addNumber(*ptr.getNumber());
			break;
		case REPtrTypeString:
			this->addString(*ptr.getString());
			break;
		case REPtrTypeNull:
			this->addNull();
			break;
		case REPtrTypeDate:
			this->addDate(*ptr.getDate());
			break;
		case REPtrTypeBuffer:
			this->addBuffer(*ptr.getBuffer());
			break;
		default:
			break;
	}
}

void REDictionaryJSONGeneratorPrivate::addArray(const RETypedArray & array)
{
	json.append("[", 1);
	for (REUInt32 i = 0; i < array.count(); i++) 
	{
		if (i) json.append(",", 1);
		this->addPtr(array[i]);
	}
	json.append("]", 1);
}

void REDictionaryJSONGeneratorPrivate::addNumber(const RENumber & num)
{
	if (num.getType() == RENumberTypeBool)
	{
		if (num.getBoolValue()) { json.append("true", 4); }
		else { json.append("false", 5); }
	}
	else 
	{
		REString numStr = num.toString();
		if (numStr.isNotEmpty()) json.append(numStr);
		else json.append("0", 1);
	}
}

void REDictionaryJSONGeneratorPrivate::addString(const REString & str)
{
	const char * v = str.getChars();
	json.appendFormat("\"%s\"", v ? v : "");
}

void REDictionaryJSONGeneratorPrivate::addValueKey(const RETypedPtr & value, const RETypedPtr & key)
{
	if (RETypedPtr::isNotEmpty(&key, REPtrTypeString) && 
		RETypedPtr::isNotEmpty(&value))
	{
		this->addString(*key.getString());
		json.append(":", 1);
		this->addPtr(value);
	}
}

void REDictionaryJSONGeneratorPrivate::addDict(const REDictionary & dict)
{
	json.append("{", 1);
	
	const REArray<REDictionary::Pair> & pairs = dict.getPairs();
	for (REUInt32 i = 0; i < pairs.count(); i++) 
	{
		if (i) json.append(",", 1);
		this->addValueKey(pairs[i].value, pairs[i].key);
	}
	
	json.append("}", 1);
}

class REDictionaryPrivate
{
public:
	static void setupJSONReaderCallbacks(OKJSONParserCallbacks * jsonCallbacks);
};

void REDictionaryPrivate::setupJSONReaderCallbacks(OKJSONParserCallbacks * jsonCallbacks)
{
	jsonCallbacks->newMem = REDictionaryJSONCallbacks::newMem;
	jsonCallbacks->freeMem = REDictionaryJSONCallbacks::freeMem;	
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

REBOOL REDictionary::readJSONData(const REUByte * jsonData, 
								  const REUInt32 jsonDataSize)
{
	REDictionaryJSONCallbacks dictCallbacks;
	OKJSONParserCallbacks jsonCallbacks;
	REDictionaryPrivate::setupJSONReaderCallbacks(&jsonCallbacks);
	jsonCallbacks.userData = &dictCallbacks;
	
	PARSED_OBJECT parsedObject = OKJSONParserParse((const uint8_t *)jsonData, (uint32_t)jsonDataSize, &jsonCallbacks);
	if (parsedObject)
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
	return false;
}

REBOOL REDictionary::initializeFromJSONData(const REUByte * jsonData, const REUInt32 jsonDataSize)
{
	this->clearPairs();
	if (jsonData && jsonDataSize)
	{
		return this->readJSONData(jsonData, jsonDataSize);
	}
	return false;
}

REMutableString REDictionary::getJSONString() const
{
	if (_pairs.isEmpty())
	{
		return REMutableString("{}", 3);
	}
	
	REDictionaryJSONGeneratorPrivate g;
	g.addDict(*this);
	return g.json;
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

const REArray<REDictionary::Pair> & REDictionary::getPairs() const
{
	return _pairs;
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

