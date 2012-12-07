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


#ifndef __REXMLPROPERTYLISTWRITER_H__
#define __REXMLPROPERTYLISTWRITER_H__

#include "RECommonHeader.h"
#include "REBuffer.h"
#include "REObjectsDictionary.h"
#include "REStringObject.h"
#include "REObjectsArray.h"
#include "RENumberObject.h"
#include "REBufferObject.h"

/// Class using for generating XML property list from dictionary key/pairs
class __RE_PUBLIC_CLASS_API__ REXMLPropertyListWriter
{
private:
	REString _errorString;
	REString * _xmlStr;
	
	
	REBOOL WritePair(const REString & prefixString, REObjectsDictionary::KeyObjectStruct * pair);
	REBOOL WriteObject(const REString & prefixString, REObject * obj);
	REBOOL WriteString(const REString & prefixString, REStringObject * strObj);
	REBOOL WriteArray(const REString & prefixString, REObjectsArray * arrObj);
	REBOOL WriteDictionary(const REString & prefixString, REObjectsDictionary * dict);
	REBOOL WriteNumber(const REString & prefixString, RENumberObject * number);
	REBOOL WriteBuffer(const REString & prefixString, REBufferObject * buff);
	
public:
	/// Writes to 'listString' string pointer generated XML property list from 'pairs' dictionary key/pairs.
	/// Returns true if both pointers not NULL and operation done, otherwice returns false.
	REBOOL WriteToString(REArray<REObjectsDictionary::KeyObjectStruct> * pairs, REString * listString);
	
	/// Default constructor.
	REXMLPropertyListWriter();
	
	/// Destructor.
	~REXMLPropertyListWriter();
};


#endif /* __REXMLPROPERTYLISTWRITER_H__ */


