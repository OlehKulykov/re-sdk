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
#include "../../include/recore/REXMLPropertyListWriter.h"
#include "../../include/recore/REXMLPropertyListReader.h"
#include "../../include/recore/REFile.h"
#include "../../include/recore/REData.h"

REArray<REDictionaryObject::KeyObjectStruct> * REDictionaryObject::getPairs()
{
	return (&_pairs);
}

REBOOL REDictionaryObject::readPairsFromFilePath(REArray<REDictionaryObject::KeyObjectStruct> * pairs, const REString & path)
{
	REData data;
	if (data.initFromPath(path))
	{
		REXMLPropertyListReader r;
		return r.ReadFromString((const char *)data.getBytes(), pairs);
	}
	return false;
}

REBOOL REDictionaryObject::initFromFilePath(const REString & path)
{
	return readPairsFromFilePath(&_pairs, path);
}

REBOOL REDictionaryObject::writePairsToFilePath(REArray<REDictionaryObject::KeyObjectStruct> * pairs, const REString & path)
{
	FILE * f = REFile::fileOpen(path, "wb+");
	if (f)
	{
		REXMLPropertyListWriter w;
		REMutableString plistStr;
		w.writeToString(pairs, &plistStr);
		const REUInt32 writed = (REUInt32)fwrite(plistStr.getChars(), 1, plistStr.getLength(), f);
		fclose(f);
		return (writed == plistStr.getLength());
	}
	return false;
}

REBOOL REDictionaryObject::writeToFile(const REString & path)
{
	return REDictionaryObject::writePairsToFilePath(&_pairs, path);
}

/* REObject */
const REUInt32 REDictionaryObject::getClassIdentifier() const
{
	return REDictionaryObject::classIdentifier();
}

const REUInt32 REDictionaryObject::classIdentifier()
{
	static const REUInt32 clasIdentif = REMD5Generator::generateFromString("REDictionaryObject");
	return clasIdentif;
}

REBOOL REDictionaryObject::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REDictionaryObject::classIdentifier() == classIdentifier) || REObject::isImplementsClass(classIdentifier));
}

REBOOL REDictionaryObject::isEqualByPairs(REObject * anotherObject, REArray<REDictionaryObject::KeyObjectStruct> * pairs)
{
	if (anotherObject)
	{
		if (anotherObject->getClassIdentifier() == REDictionaryObject::classIdentifier())
		{
			REDictionaryObject * d = (REDictionaryObject*)anotherObject;
			if (pairs->count() == d->_pairs.count())
			{
				for (REUInt32 i = 0; i < pairs->count(); i++)
				{
					if ( !(*pairs)[i].keyValue->isEqual((REObject*)d->_pairs[i].keyValue) )
					{
						return false;
					}
					if ( !(*pairs)[i].objValue->isEqual((REObject*)d->_pairs[i].objValue) )
					{
						return false;
					}
				}
				return true;
			}
		}
	}
	return false;
}

REBOOL REDictionaryObject::isEqual(REObject * anotherObject)
{
	if (REObject::isEqual(anotherObject)) { return true; }
	
	return REDictionaryObject::isEqualByPairs(anotherObject, &_pairs);
}

REBOOL REDictionaryObject::readAllKeys(REArray<REObject*> * keys)
{
	if (keys) 
	{
		keys->clear();
		keys->setCapacity(_pairs.count());
		for (REUInt32 i = 0; i < _pairs.count(); i++) 
		{
			if (!keys->add(_pairs[i].keyValue))
			{
				keys->clear();
				return false;
			}
		}
		return true;
	}
	return false;
}

REDictionaryObject::KeyObjectStruct * REDictionaryObject::pairForKey(REObject * keyObj)
{
	for (REUInt32 i = 0; i < _pairs.count(); i++) 
	{
		if ( _pairs[i].keyValue->isEqual(keyObj) )
		{
			return (&_pairs[i]);
		}
	}
	return NULL;
}

void REDictionaryObject::clear()
{
	for (REUInt32 i = 0; i < _pairs.count(); i++) 
	{
		_pairs[i].keyValue->release();
		_pairs[i].objValue->release();
	}
	_pairs.clear();
}

REBOOL REDictionaryObject::setObject(REObject * objValue, REObject * keyObject)
{
	if (objValue && keyObject) 
	{
		if (keyObject->getClassIdentifier() != REStringObject::classIdentifier()) 
		{
			return false;
		}
		
		REDictionaryObject::KeyObjectStruct * existedPair = this->pairForKey(keyObject);
		if (existedPair) 
		{
			existedPair->objValue->release();
			existedPair->objValue = objValue;
			existedPair->objValue->retain();
			return true;
		}
		else
		{
			REDictionaryObject::KeyObjectStruct newPair;
			newPair.objValue = objValue;
			newPair.keyValue = keyObject;
			if ( _pairs.add(newPair) ) 
			{
				newPair.objValue->retain();
				newPair.keyValue->retain();
				return true;
			}
		}
	}
	return false;
}

REBOOL REDictionaryObject::setObject(REObject * objValue, const REString & stringKey)
{
	REStringObject * keyObj = REStringObject::createWithString(stringKey);
	if (keyObj) 
	{
		const REBOOL settingResult = this->setObject(objValue, keyObj);
		keyObj->release();
		return settingResult;
	}
	return false;
}

/// Setting object value for key as in 'REBOOL SetObject(REObject * objValue, REObject * keyObject);'
/// As 'keyObject' will use REStringObject initialized from 'stringKey'.
REBOOL REDictionaryObject::setObject(REObject * objValue, const char * stringKey)
{
	if (stringKey) 
	{
		REStringObject * keyObj = REStringObject::createWithChars(stringKey);
		if (keyObj) 
		{
			const REBOOL settingResult = this->setObject(objValue, keyObj);
			keyObj->release();
			return settingResult;
		}
	}
	return false;
}

REObject * REDictionaryObject::getObjectForKey(REObject * keyObject)
{
	if (keyObject) 
	{
		REDictionaryObject::KeyObjectStruct * existedPair = this->pairForKey(keyObject);
		if (existedPair) 
		{
			return existedPair->objValue;
		}
	}
	return NULL;
}

/// Return object or NULL for key as in 'REObject * GetObjectForKey(REObject * keyObject);'
/// As 'keyObject' will use REStringObject initialized from 'stringKey'.
REObject * REDictionaryObject::getObjectForKey(const REString & stringKey)
{
	REStringObject * keyObj = REStringObject::createWithString(stringKey);
	if (keyObj) 
	{
		REObject * gettedObject = this->getObjectForKey(keyObj);
		keyObj->release();
		return gettedObject;
	}
	return NULL;
}


/// Return object or NULL for key as in 'REObject * GetObjectForKey(REObject * keyObject);'
/// As 'keyObject' will use REStringObject initialized from 'stringKey'.
REObject * REDictionaryObject::getObjectForKey(const char * stringKey)
{
	if (stringKey) 
	{
		REStringObject * keyObj = REStringObject::createWithChars(stringKey);
		if (keyObj) 
		{
			REObject * gettedObject = this->getObjectForKey(keyObj);
			keyObj->release();
			return gettedObject;
		}
	}
	return NULL;
}

REDictionaryObject::REDictionaryObject() : REObject()
{
	
}

void REDictionaryObject::onReleased()
{
	this->clear();
	
	REObject::onReleased();
}

REDictionaryObject::~REDictionaryObject()
{
	
}

REDictionaryObject * REDictionaryObject::create()
{
	REDictionaryObject * dict = new REDictionaryObject();
	return dict;
}



