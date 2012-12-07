/*
 *   Copyright 2012 Kulykov Oleh
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


#include "../../include/recore/REObjectsDictionary.h"
#include "../../include/recore/REXMLPropertyListWriter.h"
#include "../../include/recore/REXMLPropertyListReader.h"
#include "../../include/recore/REFile.h"
#include "../../include/recore/REData.h"

REArray<REObjectsDictionary::KeyObjectStruct> * REObjectsDictionary::GetPairs()
{
	return (&_pairs);
}

REBOOL REObjectsDictionary::ReadPairsFromFilePath(REArray<REObjectsDictionary::KeyObjectStruct> * pairs, const REString & path)
{
	REData data;
	if (data.InitFromPath(path))
	{
		REXMLPropertyListReader r;
		return r.ReadFromString((const char *)data.GetBytes(), pairs);
	}
	return false;
}

REBOOL REObjectsDictionary::InitFromFilePath(const REString & path)
{
	return ReadPairsFromFilePath(&_pairs, path);
}

REBOOL REObjectsDictionary::WritePairsToFilePath(REArray<REObjectsDictionary::KeyObjectStruct> * pairs, const REString & path)
{
	FILE * f = REFile::FOpen(path, "wb+");
	if (f)
	{
		REXMLPropertyListWriter w;
		REString plistStr;
		w.WriteToString(pairs, &plistStr);
		const REUInt32 writed = (REUInt32)fwrite(plistStr.UTF8String(), 1, plistStr.Length(), f);
		fclose(f);
		return (writed == plistStr.Length());
	}
	return false;
}

REBOOL REObjectsDictionary::WriteToFile(const REString & path)
{
	return REObjectsDictionary::WritePairsToFilePath(&_pairs, path);
}

/* REObject */
const REUInt32 REObjectsDictionary::GetClassIdentifier() const
{
	return REObjectsDictionary::ClassIdentifier();
}

const REUInt32 REObjectsDictionary::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REMD5Generator::GenerateFromString("REObjectsDictionary");
	return clasIdentif;
}

REBOOL REObjectsDictionary::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REObjectsDictionary::ClassIdentifier() == classIdentifier) || REObject::IsImplementsClass(classIdentifier));
}

REBOOL REObjectsDictionary::IsEqualByPairs(REObject * anotherObject, REArray<REObjectsDictionary::KeyObjectStruct> * pairs)
{
	if (anotherObject)
	{
		if (anotherObject->GetClassIdentifier() == REObjectsDictionary::ClassIdentifier())
		{
			REObjectsDictionary * d = (REObjectsDictionary*)anotherObject;
			if (pairs->Count() == d->_pairs.Count())
			{
				for (REUInt32 i = 0; i < pairs->Count(); i++)
				{
					if ( !(*pairs)[i].keyValue->IsEqual((REObject*)d->_pairs[i].keyValue) )
					{
						return false;
					}
					if ( !(*pairs)[i].objValue->IsEqual((REObject*)d->_pairs[i].objValue) )
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

REBOOL REObjectsDictionary::IsEqual(REObject * anotherObject)
{
	if (REObject::IsEqual(anotherObject)) { return true; }
	
	return REObjectsDictionary::IsEqualByPairs(anotherObject, &_pairs);
}

REBOOL REObjectsDictionary::ReadAllKeys(REArray<REObject*> * keys)
{
	if (keys) 
	{
		keys->Clear();
		keys->SetCapacity(_pairs.Count());
		for (REUInt32 i = 0; i < _pairs.Count(); i++) 
		{
			if (!keys->Add(_pairs[i].keyValue))
			{
				keys->Clear();
				return false;
			}
		}
		return true;
	}
	return false;
}

REObjectsDictionary::KeyObjectStruct * REObjectsDictionary::PairForKey(REObject * keyObj)
{
	for (REUInt32 i = 0; i < _pairs.Count(); i++) 
	{
		if ( _pairs[i].keyValue->IsEqual(keyObj) )
		{
			return (&_pairs[i]);
		}
	}
	return NULL;
}

void REObjectsDictionary::Clear()
{
	for (REUInt32 i = 0; i < _pairs.Count(); i++) 
	{
		_pairs[i].keyValue->Release();
		_pairs[i].objValue->Release();
	}
	_pairs.Clear();
}

REBOOL REObjectsDictionary::SetObject(REObject * objValue, REObject * keyObject)
{
	if (objValue && keyObject) 
	{
		if (keyObject->GetClassIdentifier() != REStringObject::ClassIdentifier()) 
		{
			return false;
		}
		
		REObjectsDictionary::KeyObjectStruct * existedPair = this->PairForKey(keyObject);
		if (existedPair) 
		{
			existedPair->objValue->Release();
			existedPair->objValue = objValue;
			existedPair->objValue->Retain();
			return true;
		}
		else
		{
			REObjectsDictionary::KeyObjectStruct newPair;
			newPair.objValue = objValue;
			newPair.keyValue = keyObject;
			if ( _pairs.Add(newPair) ) 
			{
				newPair.objValue->Retain();
				newPair.keyValue->Retain();
				return true;
			}
		}
	}
	return false;
}

REBOOL REObjectsDictionary::SetObject(REObject * objValue, const REString & stringKey)
{
	REStringObject * keyObj = REStringObject::CreateWithString(stringKey);
	if (keyObj) 
	{
		const REBOOL settingResult = this->SetObject(objValue, keyObj);
		keyObj->Release();
		return settingResult;
	}
	return false;
}

/// Setting object value for key as in 'REBOOL SetObject(REObject * objValue, REObject * keyObject);'
/// As 'keyObject' will use REStringObject initialized from 'stringKey'.
REBOOL REObjectsDictionary::SetObject(REObject * objValue, const char * stringKey)
{
	if (stringKey) 
	{
		REStringObject * keyObj = REStringObject::CreateWithChars(stringKey);
		if (keyObj) 
		{
			const REBOOL settingResult = this->SetObject(objValue, keyObj);
			keyObj->Release();
			return settingResult;
		}
	}
	return false;
}

REObject * REObjectsDictionary::GetObjectForKey(REObject * keyObject)
{
	if (keyObject) 
	{
		REObjectsDictionary::KeyObjectStruct * existedPair = this->PairForKey(keyObject);
		if (existedPair) 
		{
			return existedPair->objValue;
		}
	}
	return NULL;
}

/// Return object or NULL for key as in 'REObject * GetObjectForKey(REObject * keyObject);'
/// As 'keyObject' will use REStringObject initialized from 'stringKey'.
REObject * REObjectsDictionary::GetObjectForKey(const REString & stringKey)
{
	REStringObject * keyObj = REStringObject::CreateWithString(stringKey);
	if (keyObj) 
	{
		REObject * gettedObject = this->GetObjectForKey(keyObj);
		keyObj->Release();
		return gettedObject;
	}
	return NULL;
}


/// Return object or NULL for key as in 'REObject * GetObjectForKey(REObject * keyObject);'
/// As 'keyObject' will use REStringObject initialized from 'stringKey'.
REObject * REObjectsDictionary::GetObjectForKey(const char * stringKey)
{
	if (stringKey) 
	{
		REStringObject * keyObj = REStringObject::CreateWithChars(stringKey);
		if (keyObj) 
		{
			REObject * gettedObject = this->GetObjectForKey(keyObj);
			keyObj->Release();
			return gettedObject;
		}
	}
	return NULL;
}

REObjectsDictionary::REObjectsDictionary() : REObject()
{
	
}

void REObjectsDictionary::OnReleased()
{
	this->Clear();
	
	REObject::OnReleased();
}

REObjectsDictionary::~REObjectsDictionary()
{
	
}

REObjectsDictionary * REObjectsDictionary::Create()
{
	REObjectsDictionary * dict = new REObjectsDictionary();
	return dict;
}



