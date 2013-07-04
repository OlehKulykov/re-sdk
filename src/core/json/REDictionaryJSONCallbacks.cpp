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


#include "../../../include/recore/private/REDictionaryJSONCallbacks.h"

#include "../../../include/recore/RENULLObject.h"
#include "../../../include/recore/REStringObject.h"
#include "../../../include/recore/RENumberObject.h"
#include "../../../include/recore/REArrayObject.h"

REDictionaryJSONCallbacks::~REDictionaryJSONCallbacks()
{
	for (REUInt32 i = 0; i < pointers.count(); i++) 
	{
		RETypedPtr * p = pointers[i];
		delete p;
	}
	pointers.clear();
}

void* REDictionaryJSONCallbacks::newMem(const int size)
{
	return malloc(size);
}

void REDictionaryJSONCallbacks::freeMem(void*m)
{
	free(m);
}

void* REDictionaryJSONCallbacks::createNullObject(void * userData)
{
	RENULLObject * n = RENULLObject::NULLObject();
	if (n)
	{
		RETypedPtr * p = new RETypedPtr();
		if (p)
		{
			p->setREObject<RENULLObject>(n, REPtrTypeNullObject);
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		n->release();
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createNull(void * userData)
{
	RENULL * n = new RENULL();
	if (n)
	{
		RETypedPtr * p = new RETypedPtr(n, REPtrTypeNull);
		if (RETypedPtr::isNotEmpty(p))
		{
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		delete n;
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createNumberObjectWithBool(const int isTrue, void * userData)
{
	RENumberObject * n = RENumberObject::createWithBool(isTrue ? true : false);
	if (n)
	{
		RETypedPtr * p = new RETypedPtr();
		if (p)
		{
			p->setREObject<RENumberObject>(n, REPtrTypeNumberObject);
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		n->release();
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createNumberWithBool(const int isTrue, void * userData)
{
	RENumber * n = new RENumber();
	if (n)
	{
		RETypedPtr * p = new RETypedPtr(n, REPtrTypeNumber);
		if (RETypedPtr::isNotEmpty(p))
		{
			n->setBoolValue(isTrue ? true : false);
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		delete n;
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createNumberObjectWithLongLong(const long long value, void * userData)
{
	RENumberObject * n = RENumberObject::createWithInt64((REInt64)value);
	if (n)
	{
		RETypedPtr * p = new RETypedPtr();
		if (p)
		{
			p->setREObject<RENumberObject>(n, REPtrTypeNumberObject);
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		n->release();
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createNumberWithLongLong(const long long value, void * userData)
{
	RENumber * n = new RENumber();
	if (n)
	{
		RETypedPtr * p = new RETypedPtr(n, REPtrTypeNumber);
		if (RETypedPtr::isNotEmpty(p))
		{
			n->setInt64Value((REInt64)value);
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		delete n;
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createNumberObjectWithDouble(const double value, void * userData)
{
	RENumberObject * n = RENumberObject::createWithFloat64((REFloat64)value);
	if (n)
	{
		RETypedPtr * p = new RETypedPtr();
		if (p)
		{
			p->setREObject<RENumberObject>(n, REPtrTypeNumberObject);
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		n->release();
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createNumberWithDouble(const double value, void * userData)
{
	RENumber * n = new RENumber();
	if (n)
	{
		RETypedPtr * p = new RETypedPtr(n, REPtrTypeNumber);
		if (RETypedPtr::isNotEmpty(p))
		{
			n->setFloat64Value((REFloat64)value);
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		delete n;
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createStringObjectWithUTF8(const char* utf8str, const int inLen, void * userData)
{
	REStringObject * s = REStringObject::createWithCharsAndLen(utf8str, inLen);
	if (s)
	{
		RETypedPtr * p = new RETypedPtr();
		if (p)
		{
			p->setREObject<REStringObject>(s, REPtrTypeStringObject);
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		s->release();
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createStringWithUTF8(const char* utf8str, const int inLen, void * userData)
{
	REString * s = new REString(utf8str, inLen);
	if (s)
	{
		RETypedPtr * p = new RETypedPtr(s, REPtrTypeString);
		if (RETypedPtr::isNotEmpty(p))
		{
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		delete s;
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createArrayObject(void * userData)
{
	REArrayObject * a = REArrayObject::create();
	if (a)
	{
		RETypedPtr * p = new RETypedPtr();
		if (p)
		{
			p->setREObject<REArrayObject>(a, REPtrTypeArrayObject);
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		a->release();
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createArray(void * userData)
{
	RETypedArray * a = new RETypedArray();
	if (a)
	{
		RETypedPtr * p = new RETypedPtr(a, REPtrTypeArray);
		if (RETypedPtr::isNotEmpty(p))
		{
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		delete a;
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createDictionaryObject(void * userData)
{
	REDictionaryObjectN * d = REDictionaryObjectN::create();
	if (d)
	{
		RETypedPtr * p = new RETypedPtr();
		if (p)
		{
			p->setREObject<REDictionaryObjectN>(d, REPtrTypeDictionaryObject);
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		d->release();
	}
	return (void*)0;
}

void* REDictionaryJSONCallbacks::createDictionary(void * userData)
{
	REDictionary * d = new REDictionary();
	if (d)
	{
		RETypedPtr * p = new RETypedPtr(d, REPtrTypeDictionary);
		if (RETypedPtr::isNotEmpty(p))
		{
			((REDictionaryJSONCallbacks *)userData)->pointers.add(p);
			return REPtrCast<void, RETypedPtr>(p);
		}
		RE_SAFE_DELETE(p);
		delete d;
	}
	return (void*)0;
}

void REDictionaryJSONCallbacks::deleteObject(void* object)
{
	
}

void REDictionaryJSONCallbacks::deleteREObject(void* object)
{

}

void REDictionaryJSONCallbacks::addToArrayObject(void* array, void* object)
{
	RETypedPtr * a = REPtrCast<RETypedPtr, void>(array);
	RETypedPtr * o = REPtrCast<RETypedPtr, void>(object);
	a->getREObject<REArrayObject>()->add(o->getREObject<REObject>());
}

void REDictionaryJSONCallbacks::addToArray(void* array, void* object)
{
	RETypedPtr * a = REPtrCast<RETypedPtr, void>(array);
	RETypedPtr * o = REPtrCast<RETypedPtr, void>(object);
	a->getArray()->add(*o);
}

void REDictionaryJSONCallbacks::addToDictionaryObject(void* dict, void* key, void* value)
{
	RETypedPtr * d = REPtrCast<RETypedPtr, void>(dict);
	RETypedPtr * k = REPtrCast<RETypedPtr, void>(key);
	RETypedPtr * v = REPtrCast<RETypedPtr, void>(value);
	
	REDictionaryObjectN * dd = d->getREObject<REDictionaryObjectN>();
	dd->setValue(*v, *k);
}

void REDictionaryJSONCallbacks::addToDictionary(void* dict, void* key, void* value)
{
	RETypedPtr * d = REPtrCast<RETypedPtr, void>(dict);
	RETypedPtr * k = REPtrCast<RETypedPtr, void>(key);
	RETypedPtr * v = REPtrCast<RETypedPtr, void>(value);
	d->getDictionary()->setValue(*v, *k);
}

