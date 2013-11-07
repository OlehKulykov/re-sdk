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


#ifndef __REOBJECTPROPERTY_H__
#define __REOBJECTPROPERTY_H__

#include "RECommonHeader.h"
#include "REObject.h"


/// Common interface for object properties templates
class __RE_PUBLIC_CLASS_API__ IREObjectProperty 
{ 
public: 
    /// Settes property value from object.
	virtual void setObject(REObject * v) = 0;
	
	/// Getter for pointer of object.
	virtual REObject * getObject() const = 0;
	
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
	virtual void setObject(REObject * v)
	{
		if (_value) { _value->release(); _value = NULL; }
		if (v) { _value = (PT)v; _value->retain(); }
	}
	
	/// Getter for pointer of object.
	virtual REObject * getObject() const { return _value; }
	
	/// Check is object value exists and has null value
	REBOOL isNull() const { return ((_value) ? false : true); }
	
	/// Check is object value exists and not null value
	REBOOL isNotNull() const { return ((_value) ? true : false); }
	
	/// Overloaded basic assignment operator.
	RERetainProperty & operator=(const PT & v) { this->setObject(v); return (*this); }
	
	/// Overloaded Cast operator.
	operator PT() const { return *((PT*)(&_value)); }
	
	/// Overloaded Cast operator.
	operator PT & () const { return *((PT*)(&_value)); }
	
	/// Overloaded Structure dereference operator.
	PT operator->() { return _value; }
	
	/// Constructor with property value.
	/// Example: RERetainProperty<REView*> prop(view);
	RERetainProperty(const RERetainProperty & v) : _value(NULL) { this->setObject(v._value); }
	
	/// Constructor with property value setted to zero.
	RERetainProperty() : _value(NULL) { }
	
	/// Desctructor.
	virtual ~RERetainProperty() { if (_value) { _value->release(); } }
};


#endif /* __REOBJECTPROPERTY_H__ */


