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

class REDictionaryJSONCallbacks 
{
public:
	REArray<RETypedPtr*> pointers;
	~REDictionaryJSONCallbacks()
	{
		for (REUInt32 i = 0; i < pointers.count(); i++) 
		{
			RETypedPtr * p = pointers[i];
			delete p;
		}
		pointers.clear();
	}
	
	static void* newMem(const int size)
	{
		return malloc(size);
	}
	
	static void freeMem(void*m)
	{
		free(m);
	}
	
	static void** createNull(void * userData)
	{
		RENULLObject * n = RENULLObject::NULLObject();
		if (n)
		{
			RETypedPtr * p = new RETypedPtr(n, REPtrTypeNull);
			if (RETypedPtr::isNotEmpty(p))
			{
				((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
				return (void**)p;
			}
			RE_SAFE_DELETE(p);
		}
		return (void**)0;
	}
	
	static void** createNumberWithBool(const int isTrue, void * userData)
	{
		RENumber * n = new RENumber();
		if (n)
		{
			RETypedPtr * p = new RETypedPtr(n, REPtrTypeNumber);
			if (RETypedPtr::isNotEmpty(p))
			{
				n->setBoolValue(isTrue ? true : false);
				((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
				return (void**)p;
			}
			RE_SAFE_DELETE(p);
			delete n;
		}
		return (void**)0;
	}
	
	static void** createNumberWithLongLong(const long long value, void * userData)
	{
		RENumber * n = new RENumber();
		if (n)
		{
			RETypedPtr * p = new RETypedPtr(n, REPtrTypeNumber);
			if (RETypedPtr::isNotEmpty(p))
			{
				n->setInt64Value((REInt64)value);
				((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
				return (void**)p;
			}
			RE_SAFE_DELETE(p);
			delete n;
		}
		return (void**)0;
	}
	
	static void** createNumberWithDouble(const double value, void * userData)
	{
		RENumber * n = new RENumber();
		if (n)
		{
			RETypedPtr * p = new RETypedPtr(n, REPtrTypeNumber);
			if (RETypedPtr::isNotEmpty(p))
			{
				n->setFloat64Value((REFloat64)value);
				((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
				return (void**)p;
			}
			RE_SAFE_DELETE(p);
			delete n;
		}
		return (void**)0;
	}
	
	static void** createStringWithUTF8(const char* utf8str, const int inLen, void * userData)
	{
		REString * s = new REString(utf8str, inLen);
		if (s)
		{
			RETypedPtr * p = new RETypedPtr(s, REPtrTypeString);
			if (RETypedPtr::isNotEmpty(p))
			{
				((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
				return (void**)p;
			}
			RE_SAFE_DELETE(p);
			delete s;
		}
		return (void**)0;
	}
	
	static void** createArray(void * userData)
	{
		RETypedArray * a = new RETypedArray();
		if (a)
		{
			RETypedPtr * p = new RETypedPtr(a, REPtrTypeArray);
			if (RETypedPtr::isNotEmpty(p))
			{
				((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
				return (void**)p;
			}
			RE_SAFE_DELETE(p);
			delete a;
		}
		return (void**)0;
	}
	
	static void** createDictionary(void * userData)
	{
		REDictionary * d = new REDictionary();
		if (d)
		{
			RETypedPtr * p = new RETypedPtr(d, REPtrTypeDictionary);
			if (RETypedPtr::isNotEmpty(p))
			{
				((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
				return (void**)p;
			}
			RE_SAFE_DELETE(p);
			delete d;
		}
		return (void**)0;
	}
	
	static void deleteObject(void** object)
	{
		RETypedPtr * p = (RETypedPtr *)object;
		delete p;
	}
	
	static void addToArray(void** array, void** object)
	{
		RETypedPtr * a = (RETypedPtr *)array;
		RETypedPtr * o = (RETypedPtr *)object;
		a->getArray()->add(*o);
	}
	
	static void addToDictionary(void** dict, void** key, void** value)
	{
		RETypedPtr * d = (RETypedPtr *)dict;
		RETypedPtr * k = (RETypedPtr *)key;
		RETypedPtr * v = (RETypedPtr *)value;
		d->getDictionary()->setValue(*v, *k);
	}
};



REBOOL REDictionary::initializeFromJSONData(const REUByte * jsonData, const REUInt32 jsonDataSize)
{
	this->clearPairs();
	
	if (jsonData && jsonDataSize)
	{
		const uint32_t dataSize = jsonDataSize;
		const uint8_t * uData = (const uint8_t *)jsonData;
		
		REDictionaryJSONCallbacks c;
		OKJSONParserCallbacks callbacks;
		
		callbacks.userData = &c;
		callbacks.newMem = REDictionaryJSONCallbacks::newMem;
		callbacks.freeMem = REDictionaryJSONCallbacks::freeMem;
		callbacks.createNull = REDictionaryJSONCallbacks::createNull;
		callbacks.createNumberWithBool = REDictionaryJSONCallbacks::createNumberWithBool;
		callbacks.createNumberWithLongLong = REDictionaryJSONCallbacks::createNumberWithLongLong;
		callbacks.createNumberWithDouble = REDictionaryJSONCallbacks::createNumberWithDouble;
		callbacks.createStringWithUTF8 = REDictionaryJSONCallbacks::createStringWithUTF8;
		callbacks.createArray = REDictionaryJSONCallbacks::createArray;
		callbacks.createDictionary = REDictionaryJSONCallbacks::createDictionary;
		callbacks.deleteObject = REDictionaryJSONCallbacks::deleteObject;
		callbacks.addToArray = REDictionaryJSONCallbacks::addToArray;
		callbacks.addToDictionary = REDictionaryJSONCallbacks::addToDictionary;
		
		PARSED_OBJECT parsedObject = OKJSONParserParse(uData, dataSize, &callbacks);
		if (parsedObject)
		{
			RETypedPtr parsedPointer(*(RETypedPtr *)parsedObject);
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

