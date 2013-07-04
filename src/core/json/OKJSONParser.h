/*
 * Copyright 2012 - 2013 Kulykov Oleh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef OKJSONParser__OKJSONParser_h
#define OKJSONParser__OKJSONParser_h

#include <stdint.h>

#ifdef __cplusplus
#define EXTERN_C       extern "C"
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END   }
#else
#define EXTERN_C       /* Nothing */
#define EXTERN_C_BEGIN /* Npthing */
#define EXTERN_C_END   /* Nothing */
#endif

#define PARSED_OBJECT void*

typedef void* (*newMem)(const int);
typedef void (*freeMem)(void*);

typedef void* (*createNull)(void*);
typedef void* (*createNumberWithBool)(const int,void*);
typedef void* (*createNumberWithLongLong)(const long long,void*);
typedef void* (*createNumberWithDouble)(const double,void*);
typedef void* (*createStringWithUTF8)(const char*,const int,void*);
typedef void* (*createArray)(void*);
typedef void* (*createDictionary)(void*);

typedef void (*deleteObject)(void*);
// array, object 
typedef void (*addToArray)(void*,void*);
// dictionary, key, value
typedef void (*addToDictionary)(void*,void*,void*);

typedef struct _OKJSONParserCallbacks
{
	newMem newMem;
	freeMem freeMem;
	createNull createNull;
	createNumberWithBool createNumberWithBool;
	createNumberWithLongLong createNumberWithLongLong;
	createNumberWithDouble createNumberWithDouble;
	createStringWithUTF8 createStringWithUTF8;
	createArray createArray;
	createDictionary createDictionary;
	deleteObject deleteObject;
	addToArray addToArray;
	addToDictionary addToDictionary;
	void * userData;
} OKJSONParserCallbacks;

/// Returns NOT !!!! autoreleased JSON object. If use this function directly
/// you need release object manualy or use OKJSON Objective-C wrapper.
/// Input data and it's length should not be null or empty.
/// Void double pointer as allways is pointer to NSError * variable.
EXTERN_C PARSED_OBJECT OKJSONParserParse(const uint8_t * inData, 
										 const uint32_t inDataLength, 
										 OKJSONParserCallbacks * callbacks);

#endif
