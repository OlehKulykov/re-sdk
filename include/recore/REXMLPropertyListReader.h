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


#ifndef __REXMLPROPERTYLISTREADER_H__
#define __REXMLPROPERTYLISTREADER_H__

#include "RECommonHeader.h"
#include "REBuffer.h"
#include "REObjectsDictionary.h"
#include "REStringObject.h"
#include "REObjectsArray.h"
#include "RENumberObject.h"
#include "REBufferObject.h"
#include "REXMLPropertyListWriter.h"
#include "REBase64.h"

/// Class using for XML property lists. http://en.wikipedia.org/wiki/Property_list#Mac_OS_X
class __RE_PUBLIC_CLASS_API__ REXMLPropertyListReader
{
private:
	static void ClearPairs(REArray<REObjectsDictionary::KeyObjectStruct> * pairs);
	static REObject * NewObjectFromElement(void * elementObject);
	static REBOOL ParseDictionaryElement(REArray<REObjectsDictionary::KeyObjectStruct> * pairs, void * dictionaryElement);
	static REBOOL ReadPropertyListElement(REArray<REObjectsDictionary::KeyObjectStruct> * pairs, void * propListElement);
public:
	/// Reads from UTF8 string 'listString' and store created objects to 'pairs' array
	REBOOL ReadFromString(const char * listString, REArray<REObjectsDictionary::KeyObjectStruct> * pairs);
	
	/// Reads from UTF8 string 'listString' and store created objects to 'pairs' array
	REBOOL ReadFromString(const REString & listString, REArray<REObjectsDictionary::KeyObjectStruct> * pairs);
	
	/// Default constructor.
	REXMLPropertyListReader();
	
	/// Destructor.
	~REXMLPropertyListReader();
};


#endif /* __REXMLPROPERTYLISTREADER_H__ */


