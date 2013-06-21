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


#ifndef __RECLASSMETHOD_H__
#define __RECLASSMETHOD_H__

#include "RECommonHeader.h"
#include "REObject.h"

class __RE_PUBLIC_CLASS_API__ REClassMethod
{
public:
    /// Invoke class method with some object pointer.
    virtual void invokeWithObject(REObject * object) { }

    /// Destructor.
    virtual ~REClassMethod() { }
};

/// Class method. Using for invoking methods.
template<class T, typename MethodTypeName>
class REClassMethodWrapper : public REClassMethod
{
protected:
/// Interface for class methods.
	T * _classPointer;
	MethodTypeName _method;
public:
	/// Try to invoke class method with some object.
	virtual void invokeWithObject(REObject * object)
	{
		if (_classPointer && _method)
		{
			(_classPointer->*_method)(object);
		}
	}
	/// Constructs object with class pointer and method.
	REClassMethodWrapper(T * classPtr, MethodTypeName method) : _classPointer(classPtr), _method(method) { }
	
	/// Destructor.
	virtual ~REClassMethodWrapper() { }
};


#ifndef NEW_CLASS_METHOD
#define NEW_CLASS_METHOD(CLASS_NAME,CLASS_PTR,METHOD_NAME) ((REClassMethod*)(new REClassMethodWrapper<CLASS_NAME, void(CLASS_NAME::*)(REObject*)>(CLASS_PTR,&CLASS_NAME::METHOD_NAME)) )
#endif

#endif /* __RECLASSMETHOD_H__ */

