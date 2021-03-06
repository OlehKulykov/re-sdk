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
#include "REList.h"

class REMutableString;
class RETypedPtr;

/// Class for storing objects using by key value.
class __RE_PUBLIC_CLASS_API__ REDictionary
{
public:
	class __RE_PUBLIC_CLASS_API__ Pair 
	{	
	public:
		RETypedPtr value;
		RETypedPtr key;
		REDictionary::Pair & operator=(const REDictionary::Pair & ap);
		void release();
		Pair(const REDictionary::Pair & ap);
		Pair(const RETypedPtr & newValue, const RETypedPtr & newKey);
		~Pair();
	};
	
private:	
	REBOOL copyPairs(const REList<REDictionary::Pair> & pairs);
	
protected:
	REList<REDictionary::Pair> _pairs;

	/// If pair not found and pointer to index exists, value by index pointer will not be changed.
	REList<REDictionary::Pair>::Node * nodeForKey(const RETypedPtr & key) const;
	void clearPairs();
	REBOOL readJSONData(const REUByte * jsonData, const REUInt32 jsonDataSize, const REPtrType type);

public:
	const REList<REDictionary::Pair> & pairs() const;
	
	virtual REBOOL isEqualToDictionary(const REDictionary & anotherDictionary) const;
	
	virtual REBOOL setValue(const RETypedPtr & newValue, const RETypedPtr & keyValue);
	
	virtual REBOOL setValue(const RETypedPtr & newValue, const char * key);
	
	virtual REBOOL removeValue(const char * key);
	
	virtual RETypedPtr valueForKey(const char * key) const;
	
	virtual RETypedPtr valueForKey(const RETypedPtr & keyValue) const;
	
	virtual void clear();
	
	const REUInt32 count() const;
	
	virtual REBOOL initializeFromJSONData(const REUByte * jsonData, const REUInt32 jsonDataSize);
	
	virtual REMutableString JSONString() const;
	
	virtual REMutableString propertyListString() const;
	
	REDictionary & operator=(const REDictionary & dictionary);
	
	REDictionary();
	
	REDictionary(const REDictionary & dictionary);
	
	virtual ~REDictionary();
	
//#if (defined(__RE_OS_IPHONE__) || defined(__RE_OS_MACOSX__)) && (defined(FOUNDATION_STATIC_INLINE) || defined(FOUNDATION_EXTERN_INLINE))
//	NSDictionary * getNSDictionary() const;
//	void setNSDictionary(NSDictionary * dict);
//#endif
};

#endif /* __REDICTIONARY_H__ */

