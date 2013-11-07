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
#include "../../include/recore/REStaticString.h"
#include "../../include/recore/REBase64.h"
#include "../../include/recore/REDate.h"
#include "../../include/recore/RENumber.h"

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
	if (buffer.size() > 0)
	{
		REString b64Str;
		REBase64 b64;
		if (b64.bufferToBase64String(buffer, &b64Str))
		{
			if (b64Str.isNotEmpty())
			{
				json.append(b64Str.UTF8String(), b64Str.length());
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
	switch (ptr.type()) 
	{
		case REPtrTypeArray:
			this->addArray(*ptr.array());
			break;
		case REPtrTypeDictionary:
			this->addDict(*ptr.dictionary());
			break;
		case REPtrTypeNumber:
			this->addNumber(*ptr.number());
			break;
		case REPtrTypeString:
			this->addString(*ptr.string());
			break;
		case REPtrTypeNull:
			this->addNull();
			break;
		case REPtrTypeDate:
			this->addDate(*ptr.date());
			break;
		case REPtrTypeBuffer:
			this->addBuffer(*ptr.buffer());
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
		if (num.boolValue()) { json.append("true", 4); }
		else { json.append("false", 5); }
	}
	else 
	{
		REString numStr(RENumber::toString(num));
		if (numStr.isNotEmpty()) json.append(numStr);
		else json.append("0", 1);
	}
}

void REDictionaryJSONGeneratorPrivate::addString(const REString & str)
{
	const char * v = str.UTF8String();
	json.appendFormat("\"%s\"", v ? v : "");
}

void REDictionaryJSONGeneratorPrivate::addValueKey(const RETypedPtr & value, const RETypedPtr & key)
{
	if (RETypedPtr::isNotEmpty(&key, REPtrTypeString) && 
		RETypedPtr::isNotEmpty(&value))
	{
		this->addString(*key.string());
		json.append(":", 1);
		this->addPtr(value);
	}
}

void REDictionaryJSONGeneratorPrivate::addDict(const REDictionary & dict)
{
	json.append("{", 1);
	
	REUInt32 i = 0;
	REList<REDictionary::Pair>::Iterator iter = dict.pairs().iterator();
	while (iter.next()) 
	{
		if (i) json.append(",", 1);
		this->addValueKey(iter.value().value, iter.value().key);
		i++;
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
    jsonCallbacks->_newMem = REDictionaryJSONCallbacks::newMem;
    jsonCallbacks->_freeMem = REDictionaryJSONCallbacks::freeMem;
    jsonCallbacks->_createNull = REDictionaryJSONCallbacks::createNull;
    jsonCallbacks->_createNumberWithBool = REDictionaryJSONCallbacks::createNumberWithBool;
    jsonCallbacks->_createNumberWithLongLong = REDictionaryJSONCallbacks::createNumberWithLongLong;
    jsonCallbacks->_createNumberWithDouble = REDictionaryJSONCallbacks::createNumberWithDouble;
    jsonCallbacks->_createStringWithUTF8 = REDictionaryJSONCallbacks::createStringWithUTF8;
    jsonCallbacks->_createArray = REDictionaryJSONCallbacks::createArray;
    jsonCallbacks->_createDictionary = REDictionaryJSONCallbacks::createDictionary;
    jsonCallbacks->_addToArray = REDictionaryJSONCallbacks::addToArray;
    jsonCallbacks->_addToDictionary = REDictionaryJSONCallbacks::addToDictionary;
    jsonCallbacks->_deleteObject = REDictionaryJSONCallbacks::deleteObject;
}



REDictionary::Pair & REDictionary::Pair::operator=(const REDictionary::Pair & ap)
{
	value = ap.value;
	key = ap.key;
	return (*this);
}

void REDictionary::Pair::release()
{
	value.release();
	key.release();
}

REDictionary::Pair::Pair(const REDictionary::Pair & ap) : value(ap.value), key(ap.key) 
{
	
}

REDictionary::Pair::Pair(const RETypedPtr & newValue, const RETypedPtr & newKey) : 
	value(newValue), 
	key(newKey) 
{
	
}

REDictionary::Pair::~Pair()
{
	value.release();
	key.release();
}


REBOOL REDictionary::copyPairs(const REList<REDictionary::Pair> & pairs)
{
	REList<REDictionary::Pair>::Iterator iter = pairs.iterator();
	while (iter.next()) 
	{
		if (!_pairs.add(iter.value()))
		{
			return false;
		}
	}
	return true;
}

REBOOL REDictionary::readJSONData(const REUByte * jsonData, 
								  const REUInt32 jsonDataSize, 
								  const REPtrType type)
{
	REDictionaryJSONCallbacks dictCallbacks;
	OKJSONParserCallbacks jsonCallbacks;
	REDictionaryPrivate::setupJSONReaderCallbacks(&jsonCallbacks);
	
	jsonCallbacks.userData = &dictCallbacks;
	
	PARSED_OBJECT parsedObject = OKJSONParserParse((const uint8_t *)jsonData, (uint32_t)jsonDataSize, &jsonCallbacks);
	if (parsedObject)
	{
		RETypedPtr parsedPointer = *((RETypedPtr*)parsedObject);
		REBOOL r = false;
		REDictionary * dict = parsedPointer.dictionary();
		if (dict)
		{
			r = this->copyPairs(dict->_pairs);
		}
		return r;
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

REMutableString REDictionary::JSONString() const
{
	if (_pairs.isEmpty())
	{
		return REMutableString("{ }", 3);
	}
	
	REDictionaryJSONGeneratorPrivate g;
	g.addDict(*this);
	return g.json;
}

REMutableString REDictionary::propertyListString() const
{
	//TODO: ...
	return REMutableString();
}

REList<REDictionary::Pair>::Node * REDictionary::nodeForKey(const RETypedPtr & key) const
{
	REList<REDictionary::Pair>::Iterator iter = _pairs.iterator();
	while (iter.next()) 
	{
		if (iter.value().key.isEqualToTypedPointer(key)) 
		{
			return iter.node();
		}
	}
	return NULL;
}

const REUInt32 REDictionary::count() const
{
	REUInt32 c = 0;
	REList<REDictionary::Pair>::Iterator iter = _pairs.iterator();
	while (iter.next()) 
	{
		c++;
	}
	return c;
}

const REList<REDictionary::Pair> & REDictionary::pairs() const
{
	return _pairs;
}

REBOOL REDictionary::isEqualToDictionary(const REDictionary & anotherDictionary) const
{
	REList<REDictionary::Pair>::Iterator iter = anotherDictionary._pairs.iterator();
	while (iter.next()) 
	{
		REList<REDictionary::Pair>::Node * node = this->nodeForKey(iter.value().key);
		if (node) 
		{
			if (!node->value.value.isEqualToTypedPointer(iter.value().value)) 
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

REBOOL REDictionary::setValue(const RETypedPtr & newValue, const RETypedPtr & keyValue)
{
	if (keyValue.isNotEmpty())
	{
		REList<REDictionary::Pair>::Node * node = this->nodeForKey(keyValue);
		if (node)
		{
			if (newValue.isNotEmpty())
			{
				// set new value for existed key
				node->value.value = newValue;
				return true;
			}
			else
			{
				// new value is empty - remove pair
				return _pairs.removeNode(node);
			}
		}
		else if (newValue.isNotEmpty())
		{
			// add new value as pair
			REDictionary::Pair newPair(newValue, keyValue);
			return _pairs.add(newPair);
		}
	}
	return false;
}

REBOOL REDictionary::setValue(const RETypedPtr & newValue, const char * key)
{
	if (key) 
	{
		return this->setValue(newValue, RETypedPtr(new REString(key), REPtrTypeString));
	}
	return false;
}

RETypedPtr REDictionary::valueForKey(const RETypedPtr & keyValue) const
{
	if (keyValue.isNotEmpty())
	{			
		REList<REDictionary::Pair>::Node * node = this->nodeForKey(keyValue);
		if (node)
		{
			return node->value.value;
		}
	}
	return RETypedPtr();
}

RETypedPtr REDictionary::valueForKey(const char * key) const 
{
	return (key) ? this->valueForKey(RETypedPtr(new REStaticString(key), REPtrTypeString)) : RETypedPtr();
}

REBOOL REDictionary::removeValue(const char * key)
{
	if (key)
	{
		RETypedPtr pkey(new REStaticString(key), REPtrTypeString);
		if (pkey.isNotEmpty())
		{		
			REList<REDictionary::Pair>::Node * node = this->nodeForKey(pkey);
			if (node) 
			{
				return _pairs.removeNode(node);
			}
		}
	}
	return false;
}

void REDictionary::clearPairs()
{
	_pairs.clear();
}

void REDictionary::clear()
{
	this->clearPairs();
}

REDictionary & REDictionary::operator=(const REDictionary & dictionary)
{
	this->clearPairs();
	if (!this->copyPairs(dictionary._pairs))
	{
		this->clearPairs();
	}
	return (*this);
}

REDictionary::REDictionary(const REDictionary & dictionary)
{
	if (!this->copyPairs(dictionary._pairs))
	{
		this->clearPairs();
	}
}

REDictionary::REDictionary()
{
	
}

REDictionary::~REDictionary()
{
	this->clearPairs();
}

