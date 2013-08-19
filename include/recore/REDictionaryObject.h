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


#ifndef __REDICTIONARYOBJECT_H__
#define __REDICTIONARYOBJECT_H__

#include "RECommonHeader.h"
#include "REDictionary.h"
#include "REObject.h"

#if (defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) || defined(__USING_RECORE_DYNAMIC_LIBRARY__)) 
/// Warning message 4251: Class 'REArray<T>' needs to have dll-interface to be used by clients of class.
/// Why disable 4251: Class 'REArray<T>' defined as private field and no clents can access it.
/// To access must use public methods.
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif
#endif

/// Class for storing objects using by key value.
class __RE_PUBLIC_CLASS_API__ REDictionaryObject : public REDictionary, public REObject
{
protected:
	virtual ~REDictionaryObject();
	
public:
	/* REObject */
	virtual const REUInt32 getClassIdentifier() const;
	static const REUInt32 classIdentifier();
	virtual REBOOL isImplementsClass(const REUInt32 classIdentifier) const;
	virtual REBOOL isEqual(REObject * anotherObject);
	
	virtual REBOOL initializeFromJSONData(const REUByte * jsonData, const REUInt32 jsonDataSize);
	
	REBOOL setObject(REObject * newValue, REObject * key);
	
	REObject * objectForKey(REObject * key);
	
	REDictionaryObject();
	
	static REDictionaryObject * create();
	
	static REDictionaryObject * createWithCapacity(const REUInt32 capacity);
};

#endif /* __REDICTIONARYOBJECT_H__ */

