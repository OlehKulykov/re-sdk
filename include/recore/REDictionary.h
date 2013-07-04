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


#ifndef __REDICTIONARY_H__
#define __REDICTIONARY_H__

#include "RECommonHeader.h"
#include "REArray.h"
#include "RETypedPtr.h"
#include "RETypedArray.h"


#if (defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) || defined(__USING_RECORE_DYNAMIC_LIBRARY__)) 
/// Warning message 4251: Class 'REArray<T>' needs to have dll-interface to be used by clients of class.
/// Why disable 4251: Class 'REArray<T>' defined as private field and no clents can access it.
/// To access must use public methods.
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif
#endif

/// Class for storing objects using by key value.
class __RE_PUBLIC_CLASS_API__ REDictionary
{
public:
	typedef struct _pair
	{
		RETypedPtr key;
		RETypedPtr value;
	} Pair;
	
protected:
	REArray<REDictionary::Pair> _pairs;
	/// If pair not found and pointer to index exists, value by index pointer will not be changed.
	REDictionary::Pair * pairForKey(const RETypedPtr & key, REUInt32 * resultIndex = NULL) const;
	void clearPairs();
<<<<<<< HEAD
	REBOOL readJSONData(const REUByte * jsonData, const REUInt32 jsonDataSize, const REPtrType type);
=======
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
	
public:
	virtual REBOOL isEqualToDictionary(const REDictionary & anotherDictionary) const;
	
	virtual REBOOL setValue(const RETypedPtr & newValue, const RETypedPtr & keyValue);
	
	virtual REBOOL setValue(const RETypedPtr & newValue, const char * key);
	
	virtual REBOOL removeValue(const char * key);
	
	virtual RETypedPtr valueForKey(const char * key) const;
	
	virtual void clear();
	
	const REUInt32 pairsCount() const;
	
	RETypedArray getAllKeys() const;
	
	RETypedArray getAllValues() const;
	
<<<<<<< HEAD
	virtual REBOOL initializeFromJSONData(const REUByte * jsonData, const REUInt32 jsonDataSize);
=======
	REBOOL initializeFromJSONData(const REUByte * jsonData, const REUInt32 jsonDataSize);
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
	
	REDictionary();
	virtual ~REDictionary();
};

#endif /* __REDICTIONARY_H__ */

