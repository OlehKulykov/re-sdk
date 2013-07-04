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


#include "../RECommonHeader.h"
#include "../RETypedPtr.h"
#include "../REArray.h"
#include "../REDictionaryObjectN.h"


#ifndef __REDICTIONARYJSONCALLBACKS_H__
#define __REDICTIONARYJSONCALLBACKS_H__

class REDictionaryJSONCallbacks 
{
public:
	REArray<RETypedPtr *> pointers;
	
	~REDictionaryJSONCallbacks();
	
	static void* newMem(const int size);	
	static void freeMem(void*m);	
	static void* createNullObject(void * userData);	
	static void* createNull(void * userData);	
	static void* createNumberObjectWithBool(const int isTrue, void * userData);	
	static void* createNumberWithBool(const int isTrue, void * userData);	
	static void* createNumberObjectWithLongLong(const long long value, void * userData);	
	static void* createNumberWithLongLong(const long long value, void * userData);	
	static void* createNumberObjectWithDouble(const double value, void * userData);	
	static void* createNumberWithDouble(const double value, void * userData);	
	static void* createStringObjectWithUTF8(const char* utf8str, const int inLen, void * userData);	
	static void* createStringWithUTF8(const char* utf8str, const int inLen, void * userData);	
	static void* createArrayObject(void * userData);	
	static void* createArray(void * userData);	
	static void* createDictionaryObject(void * userData);	
	static void* createDictionary(void * userData);	
	static void deleteObject(void* object);
	static void deleteREObject(void* object);
	static void addToArray(void* array, void* object);
	static void addToArrayObject(void* array, void* object);
	static void addToDictionary(void* dict, void* key, void* value);
	static void addToDictionaryObject(void* dict, void* key, void* value);
};

#endif

