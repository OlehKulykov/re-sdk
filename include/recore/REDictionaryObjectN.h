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


#ifndef __REDictionaryObjectNN_H__
#define __REDictionaryObjectNN_H__

#include "RECommonHeader.h"
#include "REArray.h"
#include "REObject.h"
#include "REDictionary.h"

#if (defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) || defined(__USING_RECORE_DYNAMIC_LIBRARY__)) 
/// Warning message 4251: Class 'REArray<T>' needs to have dll-interface to be used by clients of class.
/// Why disable 4251: Class 'REArray<T>' defined as private field and no clents can access it.
/// To access must use public methods.
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif
#endif

/// Class for storing objects using by key value.
class __RE_PUBLIC_CLASS_API__ REDictionaryObjectN : public REDictionary, public REObject
{
protected:
	REDictionaryObjectN();
	virtual ~REDictionaryObjectN();
public:
	/* REObject */
	virtual const REUInt32 getClassIdentifier() const;
	static const REUInt32 classIdentifier(); 
	virtual REBOOL isImplementsClass(const REUInt32 classIdentifier) const;
	virtual REBOOL isEqual(REObject * anotherObject);
	virtual void onReleased();
	
	virtual REBOOL setValueObject(REObject * newValue, const char * key);
	
	virtual REBOOL setValueObject(REObject * newValue, REObject * key);
	
	virtual REObject * valueObjectForKey(const char * key) const;
	
	virtual REBOOL initializeFromJSONData(const REUByte * jsonData, const REUInt32 jsonDataSize);
	
	/// Creates and return new dictionary object.
	static REDictionaryObjectN * create();
};

#endif /* __REDictionaryObjectN_H__ */

