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


/*
 
 RENumberTypeNone = 0,
 RENumberTypeBool,
 RENumberTypeByte,
 RENumberTypeUByte,
 RENumberTypeInt16,
 RENumberTypeUInt16,
 RENumberTypeInt32,
 RENumberTypeUInt32,
 RENumberTypeInt64,
 RENumberTypeUInt64,
 RENumberTypePointer,
 RENumberTypeFloat32,
 RENumberTypeFloat64,
 RENumberTypeTimeInterval 
 
*/



#include "../../include/recore/RENumber.h"
#include <float.h>

REBOOL RENumber::IsReal() const
{
	switch (_type) 
	{
		case RENumberTypeFloat32:
		case RENumberTypeFloat64:
		case RENumberTypeTimeInterval:
			return true;
			break;
		default:
			return false;
			break;
	}
	return false;
}

REBOOL RENumber::IsInteger() const
{
	switch (_type) 
	{
		case RENumberTypeNone:
		case RENumberTypeFloat32:
		case RENumberTypeFloat64:
		case RENumberTypeTimeInterval:
			return false;
			break;
		default:
			return true;
			break;
	}
	return true;
}

REBOOL RENumber::IsUnsigned() const
{
	switch (_type) 
	{
		case RENumberTypeBool:
		case RENumberTypeUByte:
		case RENumberTypeUInt16:
		case RENumberTypeUInt32:
		case RENumberTypeUInt64:
		case RENumberTypePointer:
			return true;
			break;
		default:
			return false;
			break;
	}
	return false;
}

REBOOL RENumber::IsSigned() const
{
	switch (_type) 
	{
		case RENumberTypeByte:
		case RENumberTypeInt16:
		case RENumberTypeInt32:
		case RENumberTypeInt64:
		case RENumberTypeFloat32:
		case RENumberTypeFloat64:
		case RENumberTypeTimeInterval:
			return true;
			break;
		default:
			return false;
			break;
	}
	return false;
}

REBOOL RENumber::GetBoolValue() const
{
	if (this->IsInteger()) 
	{
		return (REBOOL)_uint64Value;
	}
	return (REBOOL)((REUInt64)_float64Value);
}

REByte RENumber::GetByteValue() const
{
	if (this->IsInteger()) 
	{
		return (REByte)_int64Value;
	}
	return (REByte)((REInt64)_float64Value);
}

REUByte RENumber::GetUByteValue() const
{
	if (this->IsInteger()) 
	{
		return (REUByte)_uint64Value;
	}
	return (REUByte)((REUInt64)_float64Value);
}

REInt16 RENumber::GetInt16Value() const
{
	if (this->IsInteger()) 
	{
		return (REInt16)_int64Value;
	}
	return (REInt16)((REInt64)_float64Value);
}

REUInt16 RENumber::GetUInt16Value() const
{
	if (this->IsInteger()) 
	{
		return (REUInt16)_uint64Value;
	}
	return (REUInt16)((REUInt64)_float64Value);
}

REInt32 RENumber::GetInt32Value() const
{
	if (this->IsInteger()) 
	{
		return (REInt32)_int64Value;
	}
	return (REInt32)((REInt64)_float64Value);
}

REUInt32 RENumber::GetUInt32Value() const
{
	if (this->IsInteger()) 
	{
		return (REUInt32)_uint64Value;
	}
	return (REUInt32)((REUInt64)_float64Value);
}

REInt64 RENumber::GetInt64Value() const
{
	if (this->IsInteger()) 
	{
		return (REInt64)_int64Value;
	}
	return (REInt64)((REInt64)_float64Value);
}

REUInt64 RENumber::GetUInt64Value() const
{
	if (this->IsInteger()) 
	{
		return (REUInt64)_uint64Value;
	}
	return (REUInt64)((REUInt64)_float64Value);
}

void * RENumber::GetPointerValue() const
{
	if (this->IsInteger()) 
	{
		return _pointerValue;
	}
	return (void*)((REUInt64)_float64Value);
}

REFloat32 RENumber::GetFloat32Value() const
{
	if (this->IsReal()) 
	{
		return (REFloat32)_float64Value;
	}
	return (REFloat32)(_int64Value);
}

REFloat64 RENumber::GetFloat64Value() const
{
	if (this->IsReal()) 
	{
		return (REFloat64)_float64Value;
	}
	return (REFloat64)(_int64Value);
}

RETimeInterval RENumber::GetTimeIntervalValue() const
{
	if (this->IsReal()) 
	{
		return (RETimeInterval)_float64Value;
	}
	return (RETimeInterval)(_int64Value);
}

void RENumber::SetBoolValue(REBOOL newValue)
{
	_type = RENumberTypeBool;
	_uint64Value = 0;
	_uint64Value = (REUInt64)newValue;
}

void RENumber::SetByteValue(REByte newValue)
{
	_type = RENumberTypeByte;
	_uint64Value = 0;
	_int64Value = (REInt64)newValue;
}

void RENumber::SetUByteValue(REUByte newValue)
{
	_type = RENumberTypeUByte;
	_uint64Value = 0;
	_uint64Value = (REUInt64)newValue;
}

void RENumber::SetInt16Value(REInt16 newValue)
{
	_type = RENumberTypeInt16;
	_uint64Value = 0;
	_int64Value = (REInt64)newValue;
}

void RENumber::SetUInt16Value(REUInt16 newValue)
{
	_type = RENumberTypeUInt16;
	_uint64Value = 0;
	_uint64Value = (REUInt64)newValue;
}

void RENumber::SetInt32Value(REInt32 newValue)
{
	_type = RENumberTypeInt32;
	_uint64Value = 0;
	_int64Value = (REInt64)newValue;
}

void RENumber::SetUInt32Value(REUInt32 newValue)
{
	_type = RENumberTypeUInt32;
	_uint64Value = 0;
	_uint64Value = (REUInt64)newValue;
}

void RENumber::SetInt64Value(REInt64 newValue)
{
	_type = RENumberTypeInt64;
	_int64Value = newValue;
}

void RENumber::SetUInt64Value(REUInt64 newValue)
{
	_type = RENumberTypeUInt64;
	_uint64Value = newValue;
}

void RENumber::SetPointerValue(void * newValue)
{
	_type = RENumberTypePointer;
	_pointerValue = newValue;
}

void RENumber::SetFloat32Value(REFloat32 newValue)
{
	_type = RENumberTypeFloat32;
	_uint64Value = 0;
	_float64Value = (REFloat64)newValue;
}

void RENumber::SetFloat64Value(REFloat64 newValue)
{
	_type = RENumberTypeFloat64;
	_uint64Value = 0;
	_float64Value = newValue;
}

void RENumber::SetTimeIntervalValue(RETimeInterval newValue)
{
	_type = RENumberTypeTimeInterval;
	_uint64Value = 0;
	_float64Value = newValue;
}

REBOOL RENumber::IsEqualToNumber(const RENumber & anotherNumber) const
{
	const int compResult = this->CompareWithNumber(anotherNumber);
	return (compResult == 0);
}

RENumber & RENumber::operator=(const RENumber & anotherNumber)
{
	_type = anotherNumber._type;
	_uint64Value = anotherNumber._uint64Value;
	return (*this);
}

bool RENumber::operator>(const RENumber & anotherNumber) const
{
	const int compResult = this->CompareWithNumber(anotherNumber);
	return (compResult > 0);
}

bool RENumber::operator<(const RENumber & anotherNumber) const
{
	const int compResult = this->CompareWithNumber(anotherNumber);
	return (compResult < 0);
}

bool RENumber::operator==(const RENumber & anotherNumber) const
{
	const int compResult = this->CompareWithNumber(anotherNumber);
	return (compResult == 0);
}

bool RENumber::operator!=(const RENumber & anotherNumber) const
{
	const int compResult = this->CompareWithNumber(anotherNumber);
	return (compResult != 0);
}

void RENumber::Clear()
{
	_uint64Value = 0;
	_type = RENumberTypeNone;
}

RENumber::RENumber(const RENumber & anotherNumber) : 
	_type(anotherNumber._type)
{
	_uint64Value = anotherNumber._uint64Value;
}

RENumber::RENumber() : 
	_type(RENumberTypeNone)
{
	_uint64Value = 0;
}

RENumber::RENumber(const REString & stringValue) :
	_type(RENumberTypeNone)
{
	if (!this->SetValueFromString(stringValue))
	{
		this->Clear();
	}
}

RENumber::RENumber(const char * stringValue) :
	_type(RENumberTypeNone)
{
	if (!this->SetValueFromString(stringValue))
	{
		this->Clear();
	}
}

RENumber::~RENumber()
{
	
}

REBOOL RENumber::ApplyValueFromString(RENumber * number, const char * strValue)
{
	if (strValue)
	{
		const char * testStr = strValue;
		REBOOL isSigned = false;
		REBOOL isReal = false;
		while (*testStr)
		{
			switch (*testStr)
			{
				case '-':
					isSigned = true;
					break;
				case '.':
				case ',':
					isReal = true;
					break;
				default:
					break;
			}
			testStr++;
		}
		
		if (isReal)
		{
			//float
			long double resNum = 0;
			if (sscanf(strValue, "%Lf", &resNum) == 1)
			{
				number->SetFloat64Value((REFloat64)resNum);
				return true;
			}
		}
		else if (isSigned)
		{
			// int
			long long int resNum = 0;
			if (sscanf(strValue, "%lld", &resNum) == 1)
			{
				number->SetInt64Value((REInt64)resNum);
				return true;
			}
		}
		else
		{
			// uint
			long long unsigned int resNum = 0;
			if (sscanf(strValue, "%llu", &resNum) == 1)
			{
				number->SetUInt64Value((REUInt64)resNum);
				return true;
			}
		}
		
	}
	return false;
}

REBOOL RENumber::SetValueFromString(const REString & strValue)
{
	return RENumber::ApplyValueFromString(this, strValue.UTF8String());
}

REBOOL RENumber::SetValueFromString(const char * strValue)
{
	return RENumber::ApplyValueFromString(this, strValue);
}

void RENumber::ApplyNumberValueToString(const RENumber & number, REString * string)
{
	char cStr[129] = { 0 };
	if (number.IsInteger())
	{
		if (number.IsSigned())
		{
			sprintf(cStr, "%lld", (long long int)number.GetInt64Value());
		}
		else if (number.IsUnsigned())
		{
			sprintf(cStr, "%llu", (long long unsigned int)number.GetUInt64Value());
		}
	}
	else if (number.IsReal())
	{
		sprintf(cStr, "%10.9Lf", (long double)number.GetFloat64Value());
	}
	string->Set(cStr);
}

REString RENumber::GetStringValue() const
{
	REString resultString;
	RENumber::ApplyNumberValueToString(*this, &resultString);
	return resultString;
}

int RENumber::CompareNumbers(const RENumber & number, const RENumber & anotherNumber)
{
	if (number.IsInteger())
	{
		if (number.IsSigned())
		{
			const REInt64 thisValue = number.GetInt64Value();
			if (anotherNumber.IsInteger())
			{
				if (anotherNumber.IsSigned())
				{
					const REInt64 anValue = anotherNumber.GetInt64Value();
					if (thisValue == anValue) { return 0; }
					else if (thisValue > anValue) { return 1; }
					else return -1;
				}
				else if (anotherNumber.IsUnsigned())
				{
					const REUInt64 anValue = anotherNumber.GetUInt64Value();
					if (thisValue == anValue) { return 0; }
					else if (thisValue > anValue) { return 1; }
					else return -1;
				}
			}
			else if (anotherNumber.IsReal())
			{
				const REFloat64 anValue = anotherNumber.GetFloat64Value();
				if (thisValue == anValue) { return 0; }
				else if (thisValue > anValue) { return 1; }
				else return -1;
			}
		}
		else if (number.IsUnsigned())
		{
			const REUInt64 thisValue = number.GetUInt64Value();
			if (anotherNumber.IsInteger())
			{
				if (anotherNumber.IsSigned())
				{
					const REInt64 anValue = anotherNumber.GetInt64Value();
					if (thisValue == anValue) { return 0; }
					else if (thisValue > anValue) { return 1; }
					else return -1;
				}
				else if (anotherNumber.IsUnsigned())
				{
					const REUInt64 anValue = anotherNumber.GetUInt64Value();
					if (thisValue == anValue) { return 0; }
					else if (thisValue > anValue) { return 1; }
					else return -1;
				}
			}
			else if (anotherNumber.IsReal())
			{
				const REFloat64 anValue = anotherNumber.GetFloat64Value();
				if (thisValue == anValue) { return 0; }
				else if (thisValue > anValue) { return 1; }
				else return -1;
			}
		}
	}
	else if (number.IsReal())
	{
		const REFloat64 thisValue = number.GetFloat64Value();
		if (anotherNumber.IsInteger())
		{
			if (anotherNumber.IsSigned())
			{
				const REInt64 anValue = anotherNumber.GetInt64Value();
				if (thisValue == anValue) { return 0; }
				else if (thisValue > anValue) { return 1; }
				else return -1;
			}
			else if (anotherNumber.IsUnsigned())
			{
				const REUInt64 anValue = anotherNumber.GetUInt64Value();
				if (thisValue == anValue) { return 0; }
				else if (thisValue > anValue) { return 1; }
				else return -1;
			}
		}
		else if (anotherNumber.IsReal())
		{
			const REFloat64 anValue = anotherNumber.GetFloat64Value();
			if (thisValue == anValue) { return 0; }
			else if (thisValue > anValue) { return 1; }
			else return -1;
		}
	}
	
	return 0;
}

/*
 -1 this < anotherNumber
 0 this == anotherNumber
 1 this > anotherNumber OR anotherNumber is NULL, mean nothing   */
int RENumber::CompareWithNumber(const RENumber & anotherNumber) const
{
	return RENumber::CompareNumbers(*this, anotherNumber);
}

