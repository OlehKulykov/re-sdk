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


#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

#if defined(HAVE_FLOAT_H)
#include <float.h>
#endif

class RENumberPrivate 
{	
public:
	static int writeToString(const RENumber & number, char * cStr);
	static REBOOL readFromString(RENumber * number, const char * strValue);
};

REBOOL RENumberPrivate::readFromString(RENumber * number, const char * strValue)
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
#if defined(HAVE_FUNCTION_SSCANF_S)			
			const int readed = sscanf_s(strValue, "%Lf", &resNum);
#else
			const int readed = sscanf(strValue, "%Lf", &resNum);
#endif			
			if (readed == 1)
			{
				number->setFloat64Value((REFloat64)resNum);
				return true;
			}
		}
		else if (isSigned)
		{
			// int
			long long int resNum = 0;
#if defined(HAVE_FUNCTION_SSCANF_S)			
			const int readed = sscanf_s(strValue, "%lld", &resNum);
#else			
			const int readed = sscanf(strValue, "%lld", &resNum);
#endif
			if (readed == 1)
			{
				number->setInt64Value((REInt64)resNum);
				return true;
			}
		}
		else
		{
			// uint
			long long unsigned int resNum = 0;
#if defined(HAVE_FUNCTION_SSCANF_S)		
			const int readed = sscanf_s(strValue, "%llu", &resNum);
#else			
			const int readed = sscanf(strValue, "%llu", &resNum);
#endif			
			if (readed == 1)
			{
				number->setUInt64Value((REUInt64)resNum);
				return true;
			}
		}
		
	}
	return false;
}

int RENumberPrivate::writeToString(const RENumber & number, char * cStr)
{
	if (number.isInteger())
	{
		if (number.isSigned())
		{
#if defined(HAVE_FUNCTION_SPRINTF_S) 
			return sprintf_s(cStr, "%lld", (long long int)number.int64Value());
#else			
			return sprintf(cStr, "%lld", (long long int)number.int64Value());
#endif			
		}
		else if (number.isUnsigned())
		{
#if defined(HAVE_FUNCTION_SPRINTF_S) 			
			return sprintf_s(cStr, "%llu", (long long unsigned int)number.uint64Value());
#else			
			return sprintf(cStr, "%llu", (long long unsigned int)number.uint64Value());
#endif			
		}
	}
	else if (number.isReal())
	{
#if defined(HAVE_FUNCTION_SPRINTF_S) 			
		return sprintf_s(cStr, "%10.9Lf", (long double)number.float64Value());
#else		
		return sprintf(cStr, "%10.9Lf", (long double)number.float64Value());
#endif		
	}
	return -1;
}

RENumberType RENumber::getType() const 
{
	return _type; 
}

REBOOL RENumber::isReal() const
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

REBOOL RENumber::isInteger() const
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

REBOOL RENumber::isUnsigned() const
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

REBOOL RENumber::isSigned() const
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

REBOOL RENumber::boolValue() const
{
	if (this->isInteger()) 
	{
		return (REBOOL)_uint64Value;
	}
	return (REBOOL)((REUInt64)_float64Value);
}

REByte RENumber::byteValue() const
{
	if (this->isInteger()) 
	{
		return (REByte)_int64Value;
	}
	return (REByte)((REInt64)_float64Value);
}

REUByte RENumber::ubyteValue() const
{
	if (this->isInteger()) 
	{
		return (REUByte)_uint64Value;
	}
	return (REUByte)((REUInt64)_float64Value);
}

REInt16 RENumber::int16Value() const
{
	if (this->isInteger()) 
	{
		return (REInt16)_int64Value;
	}
	return (REInt16)((REInt64)_float64Value);
}

REUInt16 RENumber::uint16Value() const
{
	if (this->isInteger()) 
	{
		return (REUInt16)_uint64Value;
	}
	return (REUInt16)((REUInt64)_float64Value);
}

REInt32 RENumber::int32Value() const
{
	if (this->isInteger()) 
	{
		return (REInt32)_int64Value;
	}
	return (REInt32)((REInt64)_float64Value);
}

REUInt32 RENumber::uint32Value() const
{
	if (this->isInteger()) 
	{
		return (REUInt32)_uint64Value;
	}
	return (REUInt32)((REUInt64)_float64Value);
}

REInt64 RENumber::int64Value() const
{
	if (this->isInteger()) 
	{
		return (REInt64)_int64Value;
	}
	return (REInt64)((REInt64)_float64Value);
}

REUInt64 RENumber::uint64Value() const
{
	if (this->isInteger()) 
	{
		return (REUInt64)_uint64Value;
	}
	return (REUInt64)((REUInt64)_float64Value);
}

void * RENumber::pointerValue() const
{
	if (this->isInteger()) 
	{
		return _pointerValue;
	}
	return (void*)((REUInt64)_float64Value);
}

REFloat32 RENumber::float32Value() const
{
	if (this->isReal()) 
	{
		return (REFloat32)_float64Value;
	}
	return (REFloat32)(_int64Value);
}

REFloat64 RENumber::float64Value() const
{
	if (this->isReal()) 
	{
		return (REFloat64)_float64Value;
	}
	return (REFloat64)(_int64Value);
}

RETimeInterval RENumber::timeIntervalValue() const
{
	if (this->isReal()) 
	{
		return (RETimeInterval)_float64Value;
	}
	return (RETimeInterval)(_int64Value);
}

void RENumber::setBoolValue(REBOOL newValue)
{
	_type = RENumberTypeBool;
	_uint64Value = 0;
	_uint64Value = (REUInt64)newValue;
}

void RENumber::setByteValue(REByte newValue)
{
	_type = RENumberTypeByte;
	_uint64Value = 0;
	_int64Value = (REInt64)newValue;
}

void RENumber::setUByteValue(REUByte newValue)
{
	_type = RENumberTypeUByte;
	_uint64Value = 0;
	_uint64Value = (REUInt64)newValue;
}

void RENumber::setInt16Value(REInt16 newValue)
{
	_type = RENumberTypeInt16;
	_uint64Value = 0;
	_int64Value = (REInt64)newValue;
}

void RENumber::setUInt16Value(REUInt16 newValue)
{
	_type = RENumberTypeUInt16;
	_uint64Value = 0;
	_uint64Value = (REUInt64)newValue;
}

void RENumber::setInt32Value(REInt32 newValue)
{
	_type = RENumberTypeInt32;
	_uint64Value = 0;
	_int64Value = (REInt64)newValue;
}

void RENumber::setUInt32Value(REUInt32 newValue)
{
	_type = RENumberTypeUInt32;
	_uint64Value = 0;
	_uint64Value = (REUInt64)newValue;
}

void RENumber::setInt64Value(REInt64 newValue)
{
	_type = RENumberTypeInt64;
	_int64Value = newValue;
}

void RENumber::setUInt64Value(REUInt64 newValue)
{
	_type = RENumberTypeUInt64;
	_uint64Value = newValue;
}

void RENumber::setPointerValue(void * newValue)
{
	_type = RENumberTypePointer;
	_pointerValue = newValue;
}

void RENumber::setFloat32Value(REFloat32 newValue)
{
	_type = RENumberTypeFloat32;
	_uint64Value = 0;
	_float64Value = (REFloat64)newValue;
}

void RENumber::setFloat64Value(REFloat64 newValue)
{
	_type = RENumberTypeFloat64;
	_uint64Value = 0;
	_float64Value = newValue;
}

void RENumber::setTimeIntervalValue(RETimeInterval newValue)
{
	_type = RENumberTypeTimeInterval;
	_uint64Value = 0;
	_float64Value = newValue;
}

REBOOL RENumber::isEqualToNumber(const RENumber & anotherNumber) const
{
	const int compResult = this->compareWithNumber(anotherNumber);
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
	const int compResult = this->compareWithNumber(anotherNumber);
	return (compResult > 0);
}

bool RENumber::operator<(const RENumber & anotherNumber) const
{
	const int compResult = this->compareWithNumber(anotherNumber);
	return (compResult < 0);
}

bool RENumber::operator==(const RENumber & anotherNumber) const
{
	const int compResult = this->compareWithNumber(anotherNumber);
	return (compResult == 0);
}

bool RENumber::operator!=(const RENumber & anotherNumber) const
{
	const int compResult = this->compareWithNumber(anotherNumber);
	return (compResult != 0);
}

void RENumber::clear()
{
	_uint64Value = 0;
	_type = RENumberTypeNone;
}

RENumber::RENumber(const RENumber & anotherNumber) : 
	_type(anotherNumber._type)
{
	_uint64Value = anotherNumber._uint64Value;
}

RENumber::RENumber(const REBOOL boolValue) :
	_type(RENumberTypeNone)
{
	this->setBoolValue(boolValue);
}

RENumber::RENumber() : 
	_type(RENumberTypeNone)
{
	_uint64Value = 0;
}

RENumber::RENumber(const REString & stringValue) :
	_type(RENumberTypeNone)
{
	if (!RENumberPrivate::readFromString(this, stringValue.UTF8String()))
	{
		this->clear();
	}
}

RENumber::RENumber(const char * stringValue) :
	_type(RENumberTypeNone)
{
	if (!RENumberPrivate::readFromString(this, stringValue))
	{
		this->clear();
	}
}

/// Constructs number from REInt64 value.
RENumber::RENumber(const REInt64 int64Value) :
	_type(RENumberTypeInt64)
{
	_int64Value = int64Value;
}

/// Constructs number from REUInt64 value.
RENumber::RENumber(const REUInt64 uint64Value) :
	_type(RENumberTypeUInt64)
{
	_uint64Value = uint64Value;
}

/// Constructs number from pointer value.
RENumber::RENumber(void * pointerValue) :
	_type(RENumberTypePointer)
{
	_pointerValue = pointerValue;
}

/// Constructs number from REFloat64 value.
RENumber::RENumber(const REFloat64 float64Value) :
	_type(RENumberTypeFloat64)
{
	_float64Value = float64Value;
}

RENumber::~RENumber()
{
	
}

RENumber RENumber::fromString(const char * string)
{
	RENumber n;
	if (RENumberPrivate::readFromString(&n, string))
	{
		return n;
	}
	return RENumber();
}

RENumber RENumber::fromString(const REString & string)
{
	RENumber n;
	if (RENumberPrivate::readFromString(&n, string.UTF8String()))
	{
		return n;
	}
	return RENumber();
}

REString RENumber::toString(const RENumber & number)
{
	char cStr[64] = { 0 };
	const int writed = RENumberPrivate::writeToString(number, cStr);
	if (writed > 0)
	{
		return REString(cStr, (REUInt32)writed);
	}
	return REString();
}

int RENumber::compareNumbers(const RENumber & number, const RENumber & anotherNumber)
{
	if (number.isInteger())
	{
		if (number.isSigned())
		{
			const REInt64 thisValue = number.int64Value();
			if (anotherNumber.isInteger())
			{
				if (anotherNumber.isSigned())
				{
					const REInt64 anValue = anotherNumber.int64Value();
					if (thisValue == anValue) { return 0; }
					else if (thisValue > anValue) { return 1; }
					else return -1;
				}
				else if (anotherNumber.isUnsigned())
				{
					const REUInt64 anValue = anotherNumber.uint64Value();
					if (thisValue == anValue) { return 0; }
					else if (thisValue > anValue) { return 1; }
					else return -1;
				}
			}
			else if (anotherNumber.isReal())
			{
				const REFloat64 anValue = anotherNumber.float64Value();
				if (thisValue == anValue) { return 0; }
				else if (thisValue > anValue) { return 1; }
				else return -1;
			}
		}
		else if (number.isUnsigned())
		{
			const REUInt64 thisValue = number.uint64Value();
			if (anotherNumber.isInteger())
			{
				if (anotherNumber.isSigned())
				{
					const REInt64 anValue = anotherNumber.int64Value();
					if (thisValue == anValue) { return 0; }
					else if (thisValue > anValue) { return 1; }
					else return -1;
				}
				else if (anotherNumber.isUnsigned())
				{
					const REUInt64 anValue = anotherNumber.uint64Value();
					if (thisValue == anValue) { return 0; }
					else if (thisValue > anValue) { return 1; }
					else return -1;
				}
			}
			else if (anotherNumber.isReal())
			{
				const REFloat64 anValue = anotherNumber.float64Value();
				if (thisValue == anValue) { return 0; }
				else if (thisValue > anValue) { return 1; }
				else return -1;
			}
		}
	}
	else if (number.isReal())
	{
		const REFloat64 thisValue = number.float64Value();
		if (anotherNumber.isInteger())
		{
			if (anotherNumber.isSigned())
			{
				const REInt64 anValue = anotherNumber.int64Value();
				if (thisValue == anValue) { return 0; }
				else if (thisValue > anValue) { return 1; }
				else return -1;
			}
			else if (anotherNumber.isUnsigned())
			{
				const REUInt64 anValue = anotherNumber.uint64Value();
				if (thisValue == anValue) { return 0; }
				else if (thisValue > anValue) { return 1; }
				else return -1;
			}
		}
		else if (anotherNumber.isReal())
		{
			const REFloat64 anValue = anotherNumber.float64Value();
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
int RENumber::compareWithNumber(const RENumber & anotherNumber) const
{
	return RENumber::compareNumbers(*this, anotherNumber);
}

