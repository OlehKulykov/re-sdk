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


#ifndef __REXMLPROPERTYLISTWRITER_H__
#define __REXMLPROPERTYLISTWRITER_H__

#include "RECommonHeader.h"
#include "REBuffer.h"
#include "REDictionaryObject.h"
#include "REStringObject.h"
#include "REArrayObject.h"
#include "RENumberObject.h"
#include "REBufferObject.h"

/// Class using for generating XML property list from dictionary key/pairs
class __RE_PUBLIC_CLASS_API__ REXMLPropertyListWriter
{
private:
	REString _errorString;
	REMutableString * _xmlStr;
	
	
<<<<<<< HEAD
	REBOOL writePair(const REString & prefixString, REDictionaryObject::KeyObjectStruct * pair);
	REBOOL writeObject(const REString & prefixString, REObject * obj);
	REBOOL writeString(const REString & prefixString, REStringObject * strObj);
	REBOOL writeArray(const REString & prefixString, REArrayObject * arrObj);
	REBOOL writeDictionary(const REString & prefixString, REDictionaryObject * dict);
=======
	REBOOL writePair(const REString & prefixString, REObjectsDictionary::KeyObjectStruct * pair);
	REBOOL writeObject(const REString & prefixString, REObject * obj);
	REBOOL writeString(const REString & prefixString, REStringObject * strObj);
	REBOOL writeArray(const REString & prefixString, REObjectsArray * arrObj);
	REBOOL writeDictionary(const REString & prefixString, REObjectsDictionary * dict);
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
	REBOOL writeNumber(const REString & prefixString, RENumberObject * number);
	REBOOL writeBuffer(const REString & prefixString, REBufferObject * buff);
	
public:
	/// Writes to 'listString' string pointer generated XML property list from 'pairs' dictionary key/pairs.
	/// Returns true if both pointers not NULL and operation done, otherwice returns false.
<<<<<<< HEAD
	REBOOL writeToString(REArray<REDictionaryObject::KeyObjectStruct> * pairs, REMutableString * listString);
=======
	REBOOL writeToString(REArray<REObjectsDictionary::KeyObjectStruct> * pairs, REMutableString * listString);
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
	
	/// Default constructor.
	REXMLPropertyListWriter();
	
	/// Destructor.
	~REXMLPropertyListWriter();
};


#endif /* __REXMLPROPERTYLISTWRITER_H__ */


