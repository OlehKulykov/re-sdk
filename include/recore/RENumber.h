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
	
	static REBOOL ApplyValueFromString(RENumber * number, const char * strValue);
	static void ApplyNumberValueToString(const RENumber & number, REString * string);
	
public:
	/// Returns type of number.
	RENumberType GetType() const { return _type; }
	
	/// Check is number signed.
	REBOOL IsSigned() const;
	
	/// Check is number unsigned.
	REBOOL IsUnsigned() const;
	
	/// Check is number is integer.
	REBOOL IsInteger() const;
	
	/// Check is number real value.
	REBOOL IsReal() const;
	
	/// Returns boolena value.
	REBOOL GetBoolValue() const;
	
	/// Returns signed byte value.
	REByte GetByteValue() const;
	
	/// Returns unsigned byte value.
	REUByte GetUByteValue() const;
	
	/// Returns signed integer 16 bit value.
	REInt16 GetInt16Value() const;
	
	/// Returns unsigned integer 16 bit value.
	REUInt16 GetUInt16Value() const;
	
	/// Returns signed integer 32 bit value.
	REInt32 GetInt32Value() const;
	
	/// Returns unsigned integer 16 bit value.
	REUInt32 GetUInt32Value() const;
	
	/// Returns signed integer 64 bit value.
	REInt64 GetInt64Value() const;
	
	/// Returns unsigned integer 64 bit value.
	REUInt64 GetUInt64Value() const;
	
	/// Returns pointer value.
	void * GetPointerValue() const;
	
	/// Returns float 32 bit value.
	REFloat32 GetFloat32Value() const;
	
	/// Returns float 64 bit value.
	REFloat64 GetFloat64Value() const;
	
	/// Returns time interval value.
	RETimeInterval GetTimeIntervalValue() const;
	
	/// Returns string presentation of value.
	REString GetStringValue() const;
	
	/// Setting value from string.
	REBOOL SetValueFromString(const REString & strValue);
	
	/// Setting value from string.
	REBOOL SetValueFromString(const char * strValue);
	
	/// Setting boolean value.
	void SetBoolValue(REBOOL newValue);
	
	/// Setting signed byte value.
	void SetByteValue(REByte newValue);
	
	/// Setting unsigned byte value.
	void SetUByteValue(REUByte newValue);
	
	/// Setting signed integer 16 bit value.
	void SetInt16Value(REInt16 newValue);
	
	/// Setting unsigned integer 16 bit value.
	void SetUInt16Value(REUInt16 newValue);
	
	/// Setting signed integer 32 bit value.
	void SetInt32Value(REInt32 newValue);
	
	/// Setting unsigned integer 32 bit value.
	void SetUInt32Value(REUInt32 newValue);
	
	/// Setting signed integer 64 bit value.
	void SetInt64Value(REInt64 newValue);
	
	/// Setting unsigned integer 64 bit value.
	void SetUInt64Value(REUInt64 newValue);
	
	/// Setting pointer value.
	void SetPointerValue(void * newValue);
	
	/// Setting float 32 bit value.
	void SetFloat32Value(REFloat32 newValue);
	
	/// Setting float 64 bit value.
	void SetFloat64Value(REFloat64 newValue);
	
	/// Setting time interval value.
	void SetTimeIntervalValue(RETimeInterval newValue);
	
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
	REBOOL IsEqualToNumber(const RENumber & anotherNumber) const;
	
	/// -1 number < anotherNumber
	/// 0 number == anotherNumber
	/// 1 number > anotherNumber OR anotherNumber is NULL, mean nothing.
	static int CompareNumbers(const RENumber & number, const RENumber & anotherNumber);
	
	/// -1 this < anotherNumber
	/// 0 this == anotherNumber
	/// 1 this > anotherNumber OR anotherNumber is NULL, mean nothing.
	int CompareWithNumber(const RENumber & anotherNumber) const;
	
	/// Clears number object.
	void Clear();
	
	/// Constructs number from another number value.
	RENumber(const RENumber & anotherNumber);
	
	/// Constructs number from string value.
	RENumber(const REString & stringValue);
	
	/// Constructs number from string value.
	RENumber(const char * stringValue);
	
	/// Constructs empty number.
	RENumber();
	
	/// Destructor.
	virtual ~RENumber();
};


#endif /* __RE_PUBLIC_CLASS_API__ */


