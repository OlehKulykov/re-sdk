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


#ifndef __REOBJECTPROPERTY_H__
#define __REOBJECTPROPERTY_H__

#include "RECommonHeader.h"
#include "REObject.h"


/// Common interface for object properties templates
class __RE_PUBLIC_CLASS_API__ IREObjectProperty 
{ 
public: 
    /// Settes property value from object.
	virtual void SetObject(REObject * v) = 0;
	
	/// Getter for pointer of object.
	virtual REObject * GetObject() const = 0;
	
	/// Desctructor.
	virtual ~IREObjectProperty() { } 
};


/// Template for object property.
/// Example: REProperty<REView*> prop;
template <class PT>
class RERetainProperty : public IREObjectProperty
{
private:
	PT _value;
public:
	/// Settes property value from object.
	virtual void SetObject(REObject * v)
	{
		if (_value) { _value->Release(); _value = NULL; }
		if (v) { _value = (PT)v; _value->Retain(); }
	}
	
	/// Getter for pointer of object.
	virtual REObject * GetObject() const { return _value; }
	
	/// Check is object value exists and has null value
	REBOOL IsNull() const { return ((_value) ? false : true); }
	
	/// Check is object value exists and not null value
	REBOOL IsNotNull() const { return ((_value) ? true : false); }
	
	/// Overloaded basic assignment operator.
	RERetainProperty & operator=(const PT & v) { this->SetObject(v); return (*this); }
	
	/// Overloaded Cast operator.
	operator PT() const { return *((PT*)(&_value)); }
	
	/// Overloaded Cast operator.
	operator PT & () const { return *((PT*)(&_value)); }
	
	/// Overloaded Structure dereference operator.
	PT operator->() { return _value; }
	
	/// Constructor with property value.
	/// Example: RERetainProperty<REView*> prop(view);
	RERetainProperty(const RERetainProperty & v) : _value(NULL) { this->SetObject(v._value); }
	
	/// Constructor with property value setted to zero.
	RERetainProperty() : _value(NULL) { }
	
	/// Desctructor.
	virtual ~RERetainProperty() { if (_value) { _value->Release(); } }
};


#endif /* __REOBJECTPROPERTY_H__ */


