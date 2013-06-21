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

REBOOL REDictionary::setValue(const RETypedPtr & newValue, const char * key)
{
	if (key) 
	{
		RETypedPtr pkey(new REString(key), REPtrTypeString);
		if (pkey.isNotEmpty())
		{
			REUInt32 index = RENotFound;
			REDictionary::Pair * pair = this->pairForKey(pkey, &index);
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
				newPair.key = pkey;
				newPair.value = newValue;
				return _pairs.add(newPair);
			}
		}
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

