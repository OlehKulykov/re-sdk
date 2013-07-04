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


#ifndef __REPROPERTY_H__
#define __REPROPERTY_H__

#include "RECommonHeader.h"
#include "REArray.h"
#include "REString.h"

/// Common interface for properties templates
class __RE_PUBLIC_CLASS_API__ IREProperty 
{ 
public: 
    /// Settes property value from pointer of another value.
	virtual void setFromValuePointer(const void* v) = 0; 
	
	/// Getter for pointer of propert value.
	virtual void * getValuePointer() const = 0;
	
	/// Desctructor.
	virtual ~IREProperty() { } 
};

/// Template for simple property.
/// Example: REProperty<int> prop;
template <class PT>
class REProperty : public IREProperty
{
private:
	PT _value;
public:
    /// Settes property value from pointer of another value.
	/// Example: REProperty<int> prop; int value = 1; prop.SetFromValuePointer(&value);
	virtual void setFromValuePointer(const void* v) { if (v) _value = *((PT*)const_cast<void*>(v)); }
	
	/// Getter for pointer of propert value.
	virtual void * getValuePointer() const { return (void*)(&_value); }
	
	/// Setter for property. 
	/// Example: REProperty<int> prop; prop.Set(4);
	void set(const PT & v) { _value = v; }
	
	/// Getter of property address value. 
	PT & get() const { return *(PT*)&_value; }
	
	/// Overloaded basic assignment operator.
	REProperty & operator=(const PT & v) { _value = v; return (*this); }
	
	/// Overloaded basic assignment operator.
	REProperty & operator=(const REProperty & v) { _value = v._value; return (*this); }
	
	/// Overloaded Cast operator.
	operator PT() const { return *(PT*)&_value; }
	
	/// Overloaded Cast operator.
	operator PT & () const { return *(PT*)&_value; }
	
	/// Constructor with property value.
	/// Example: REProperty<int> prop(-5);
	REProperty(const REProperty & v) { memset(&_value, 0, sizeof(PT)); this->set(v._value); }
	
	/// Constructor with property value setted to zero.
	REProperty() { memset(&_value, 0, sizeof(PT)); }
	
	/// Desctructor.
	virtual ~REProperty() { }
};

#endif /* __REPROPERTY_H__ */


