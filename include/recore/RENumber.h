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


#ifndef __RENUMBER_H__
#define __RENUMBER_H__

#include "RECommonHeader.h"
#include "REObject.h"
#include "REString.h"

/// Type of stored value in number object
typedef enum _reNumberType 
{
	/// Undefined value.
	RENumberTypeNone = 0,
	
	/// Boolean value.
	RENumberTypeBool,
	
	/// Signed byte value.
	RENumberTypeByte,
	
	/// Unsigned byte value.
	RENumberTypeUByte,
	
	/// Signed integer 16 bit value.
	RENumberTypeInt16,
	
	/// Unsigned integer 16 bit value.
	RENumberTypeUInt16,
	
	/// Signed integer 32 bit value.
	RENumberTypeInt32,
	
	/// Unsigned integer 32 bit value.
	RENumberTypeUInt32,
	
	/// Signed integer 64 bit value.
	RENumberTypeInt64,
	
	/// Unsigned integer 64 bit value.
	RENumberTypeUInt64,
	
	/// Pointer value (32 or 64 bit).
	RENumberTypePointer,
	
	/// Float 32 bit value.
	RENumberTypeFloat32,
	
	/// Float 64 bit value.
	RENumberTypeFloat64,
	
	/// Time interval value.
	RENumberTypeTimeInterval
}
/// Type of stored value in number object
RENumberType;

/// Class presents number object.
class __RE_PUBLIC_CLASS_API__ RENumber
{
protected:
	union
	{
		REInt64 _int64Value;
		REUInt64 _uint64Value;
		void * _pointerValue;
		REFloat64 _float64Value;
	};
	RENumberType _type;
	
	static REBOOL applyValueFromString(RENumber * number, const char * strValue);
	static void applyNumberValueToString(const RENumber & number, REString * string);
	
public:
	REString toString() const;
	
	/// Returns type of number.
	RENumberType getType() const;
	
	/// Check is number signed.
	REBOOL isSigned() const;
	
	/// Check is number unsigned.
	REBOOL isUnsigned() const;
	
	/// Check is number is integer.
	REBOOL isInteger() const;
	
	/// Check is number real value.
	REBOOL isReal() const;
	
	/// Returns boolena value.
	REBOOL getBoolValue() const;
	
	/// Returns signed byte value.
	REByte getByteValue() const;
	
	/// Returns unsigned byte value.
	REUByte getUByteValue() const;
	
	/// Returns signed integer 16 bit value.
	REInt16 getInt16Value() const;
	
	/// Returns unsigned integer 16 bit value.
	REUInt16 getUInt16Value() const;
	
	/// Returns signed integer 32 bit value.
	REInt32 getInt32Value() const;
	
	/// Returns unsigned integer 16 bit value.
	REUInt32 getUInt32Value() const;
	
	/// Returns signed integer 64 bit value.
	REInt64 getInt64Value() const;
	
	/// Returns unsigned integer 64 bit value.
	REUInt64 getUInt64Value() const;
	
	/// Returns pointer value.
	void * getPointerValue() const;
	
	/// Returns float 32 bit value.
	REFloat32 getFloat32Value() const;
	
	/// Returns float 64 bit value.
	REFloat64 getFloat64Value() const;
	
	/// Returns time interval value.
	RETimeInterval getTimeIntervalValue() const;
	
	/// Returns string presentation of value.
	REString getStringValue() const;
	
	/// Setting value from string.
	REBOOL setValueFromString(const REString & strValue);
	
	/// Setting value from string.
	REBOOL setValueFromString(const char * strValue);
	
	/// Setting boolean value.
	void setBoolValue(REBOOL newValue);
	
	/// Setting signed byte value.
	void setByteValue(REByte newValue);
	
	/// Setting unsigned byte value.
	void setUByteValue(REUByte newValue);
	
	/// Setting signed integer 16 bit value.
	void setInt16Value(REInt16 newValue);
	
	/// Setting unsigned integer 16 bit value.
	void setUInt16Value(REUInt16 newValue);
	
	/// Setting signed integer 32 bit value.
	void setInt32Value(REInt32 newValue);
	
	/// Setting unsigned integer 32 bit value.
	void setUInt32Value(REUInt32 newValue);
	
	/// Setting signed integer 64 bit value.
	void setInt64Value(REInt64 newValue);
	
	/// Setting unsigned integer 64 bit value.
	void setUInt64Value(REUInt64 newValue);
	
	/// Setting pointer value.
	void setPointerValue(void * newValue);
	
	/// Setting float 32 bit value.
	void setFloat32Value(REFloat32 newValue);
	
	/// Setting float 64 bit value.
	void setFloat64Value(REFloat64 newValue);
	
	/// Setting time interval value.
	void setTimeIntervalValue(RETimeInterval newValue);
	
	/// Basic assignment operator
	RENumber & operator=(const RENumber & anotherNumber);
	
	/// Greater than operator.
	bool operator>(const RENumber & anotherNumber) const;
	
	/// Less than operator.
	bool operator<(const RENumber & anotherNumber) const;
	
	/// Equal to operator.
	bool operator==(const RENumber & anotherNumber) const;
	
	/// Not equal to operator.
	bool operator!=(const RENumber & anotherNumber) const;
	
	/// Checks is number equal with another number.
	REBOOL isEqualToNumber(const RENumber & anotherNumber) const;
	
	/// -1 number < anotherNumber
	/// 0 number == anotherNumber
	/// 1 number > anotherNumber OR anotherNumber is NULL, mean nothing.
	static int compareNumbers(const RENumber & number, const RENumber & anotherNumber);
	
	/// -1 this < anotherNumber
	/// 0 this == anotherNumber
	/// 1 this > anotherNumber OR anotherNumber is NULL, mean nothing.
	int compareWithNumber(const RENumber & anotherNumber) const;
	
	/// Clears number object.
	void clear();
	
	/// Constructs number from another number value.
	RENumber(const RENumber & anotherNumber);
	
	/// Constructs number from string value.
	RENumber(const REString & stringValue);
	
	/// Constructs number from string value.
	RENumber(const char * stringValue);
	
	/// Constructs number from REInt64 value.
	RENumber(const REInt64 int64Value);
	
	/// Constructs number from REUInt64 value.
	RENumber(const REUInt64 uint64Value);
	
	/// Constructs number from pointer value.
	RENumber(void * pointerValue);
	
	/// Constructs number from REFloat64 value.
	RENumber(const REFloat64 float64Value);
	
	/// Constructs empty number.
	RENumber();
	
	/// Destructor.
	virtual ~RENumber();
};


#endif /* __RE_PUBLIC_CLASS_API__ */


