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


#ifndef __REFLOAT16_H__
#define __REFLOAT16_H__

#include "RECommonHeader.h"

#ifdef __RE_RECORE_NO_FLOAT16_SUPPORT__

/// If builded without Float16 support whan Float16 will be Float32
typedef REFloat32 REFloat16;

/// If builded without Float16 support whan FLOAT16_MAX (maximum float type value) will be FLT_MAX
#define FLOAT16_MAX FLT_MAX

/// If builded without Float16 support whan FLOAT16_MIN (minimum float type value) will be FLT_MIN
#define FLOAT16_MIN FLT_MIN

#else

/// Maximum Float16 type value.
#define FLOAT16_MAX 65504.0f

/// Minimum Float16 type value.
#define FLOAT16_MIN 6.1035156e-5f


/// Class using for float16 type.
class __RE_PUBLIC_CLASS_API__ REFloat16
{
private:
	REUInt16 _data;
public:
	/// Settes value from Float32 value
	void Set(const REFloat32 & f)
	{
		if ((f >= FLOAT16_MIN) || (f <= -FLOAT16_MIN))
        {
			union
			{
				REFloat32 f;
				REUInt32 ui;
			} v;
			v.f = f;
			REUInt32 uiData = v.ui;
			_data = (REUInt16)(((uiData >> 16) & 0x8000) | (((((uiData >> 23) & 0xFF) - 127) + 15) << 10) | ((uiData >> (23 - 10)) & 0x3FF));
        }
        else
        {
			_data = 0;
        }
	}
	
	/// Getter of float16 value. Result is Float32.
	const REFloat32 Get() const
	{
		if (_data)
        {
			union
			{
				REFloat32 f;
				REUInt32 ui;
			} v;
			REUInt32 uiData = _data;
			v.ui = ((uiData & 0x8000) << 16) | ((((uiData >> 10) & 0x1F) + (127 - 15)) << 23) | ((uiData & 0x3FF) << (23 - 10));
			return v.f;
        }
        return 0.0f;
	}
	
	/// Getter operator of float32 value.
	operator REFloat32() const { return this->Get(); }
	
	/// Getter operator of float64 value.
	operator REFloat64() const { return (REFloat64)this->Get(); }
	
	/// Overloaded basic assignment operator.
	REFloat16 & operator=(const REFloat16 & f) { _data = f._data; return (*this); }
	
	/// Overloaded basic assignment operator.
	REFloat16 & operator=(const REFloat32 & f) { this->Set(f); return (*this); }
	
	/// Overloaded basic assignment operator.
	REFloat16 & operator=(const REFloat64 & f) { this->Set((REFloat32)f); return (*this); }
	
	/// Overloaded addition operator.
	REFloat16 operator+(const REFloat16 & f) const { REFloat16 r(this->Get() + f.Get()); return r; }
	
	/// Overloaded addition operator.
	REFloat16 operator+(const REFloat32 & f) const { REFloat16 r(this->Get() + f); return r; }
	
	/// Overloaded addition operator.
	friend REFloat16 operator+(const REFloat16 & a, const REFloat16 & b) { REFloat16 r(a.Get() + b.Get()); return r; }
	
	/// Overloaded addition operator.
	friend REFloat16 operator+(const REFloat16 & a, const REFloat32 & b) { REFloat16 r(a.Get() + b); return r; }
	
	/// Overloaded addition operator.
	friend REFloat32 operator+(const REFloat32 & a, const REFloat16 & b) { return (a + b.Get()); }
	
	/// Overloaded subtraction operator.
	REFloat16 operator-(const REFloat16 & f) const { REFloat16 r(this->Get() - f.Get()); return r; }
	
	/// Overloaded subtraction operator.
	REFloat16 operator-(const REFloat32 & f) const { REFloat16 r(this->Get() - f); return r; }
	
	/// Overloaded subtraction operator.
	friend REFloat16 operator-(const REFloat16 & a, const REFloat16 & b) { REFloat16 r(a.Get() - b.Get()); return r; }
	
	/// Overloaded subtraction operator.
	friend REFloat16 operator-(const REFloat16 & a, const REFloat32 & b) { REFloat16 r(a.Get() - b); return r; }
	
	/// Overloaded subtraction operator.
	friend REFloat32 operator-(const REFloat32 & a, const REFloat16 & b) { return (a - b.Get()); }
	
	/// Overloaded Unary plus (integer promotion) operator.
	REFloat16 operator+() const { REFloat16 f; f._data = _data; return f; }
	
	/// Overloaded Unary minus (additive inverse operator.
	REFloat16 operator-() const { REFloat16 f; f._data = _data ^ 0x8000; return f; }
	
	/// Overloaded multiplication operator.
	REFloat16 operator*(const REFloat16 & f) const { REFloat16 r(this->Get() * f.Get()); return r; }
	
	/// Overloaded multiplication operator.
	REFloat16 operator*(const REFloat32 & f) const { REFloat16 r(this->Get() * f); return r; }
	
	/// Overloaded multiplication operator.
	friend REFloat16 operator*(const REFloat16 & a, const REFloat16 & b) { REFloat16 r(a.Get() * b.Get()); return r; }
	
	/// Overloaded multiplication operator.
	friend REFloat16 operator*(const REFloat16 & a, const REFloat32 & b) { REFloat16 r(a.Get() * b); return r; }
	
	/// Overloaded multiplication operator.
	friend REFloat32 operator*(const REFloat32 & a, const REFloat16 & b) { return (a * b.Get()); }
	
	/// Overloaded division operator.
	REFloat16 operator/(const REFloat16 & f) const { REFloat16 r(this->Get() / f.Get()); return r; }
	
	/// Overloaded division operator.
	REFloat16 operator/(const REFloat32 & f) const { REFloat16 r(this->Get() / f); return r; }
	
	/// Overloaded division operator.
	friend REFloat16 operator/(const REFloat16 & a, const REFloat16 & b) { REFloat16 r(a.Get() / b.Get()); return r; }
	
	/// Overloaded division operator.
	friend REFloat16 operator/(const REFloat16 & a, const REFloat32 & b) { REFloat16 r(a.Get() / b); return r; }
	
	/// Overloaded division operator.
	friend REFloat32 operator/(const REFloat32 & a, const REFloat16 & b) { return (a / b.Get()); }
	
	/// Overloaded 'Equal to' operator.
	bool operator==(const REFloat16 & f) const { return (_data == f._data); }
	
	/// Overloaded 'Equal to' operator.
	bool operator==(const REFloat32 & f) const { REFloat16 r(f); return (_data == r._data); }
	
	/// Overloaded 'Equal to' operator.
	friend bool operator==(const REFloat16 & a, const REFloat16 & b) { return (a._data == b._data); }
	
	/// Overloaded 'Equal to' operator.
	friend bool operator==(const REFloat16 & a, const REFloat32 & b) { return (a.Get() == b); }
	
	/// Overloaded 'Equal to' operator.
	friend bool operator==(const REFloat32 & a, const REFloat16 & b) { return (a == b.Get()); }
	
	/// Overloaded 'Not equal to' operator.
	bool operator!=(const REFloat16 & f) const { return (_data != f._data); }
	
	/// Overloaded 'Not equal to' operator.
	bool operator!=(const REFloat32 & f) const { REFloat16 r(f); return (_data != r._data); }
	
	/// Overloaded 'Not equal to' operator.
	friend bool operator!=(const REFloat16 & a, const REFloat16 & b) { return (a._data != b._data); }
	
	/// Overloaded 'Not equal to' operator.
	friend bool operator!=(const REFloat16 & a, const REFloat32 & b) { return (a.Get() != b); }
	
	/// Overloaded 'Not equal to' operator.
	friend bool operator!=(const REFloat32 & a, const REFloat16 & b) { return (a != b.Get()); }
	
	/// Overloaded 'Greater than' operator.
	bool operator>(const REFloat16 & f) const { return (this->Get() > f.Get()); }
	
	/// Overloaded 'Greater than' operator.
	bool operator>(const REFloat32 & f) const { return (this->Get() > f); }
	
	/// Overloaded 'Greater than' operator.
	friend bool operator>(const REFloat16 & a, const REFloat16 & b) { return (a.Get() > b.Get()); }
	
	/// Overloaded 'Greater than' operator.
	friend bool operator>(const REFloat16 & a, const REFloat32 & b) { return (a.Get() > b); }
	
	/// Overloaded 'Greater than' operator.
	friend bool operator>(const REFloat32 & a, const REFloat16 & b) { return (a > b.Get()); }
	
	/// Overloaded 'Less than' operator.
	bool operator<(const REFloat16 & f) const { return (this->Get() < f.Get()); }
	
	/// Overloaded 'Less than' operator.
	bool operator<(const REFloat32 & f) const { return (this->Get() < f); }
	
	/// Overloaded 'Less than' operator.
	friend bool operator<(const REFloat16 & a, const REFloat16 & b) { return (a.Get() < b.Get()); }
	
	/// Overloaded 'Less than' operator.
	friend bool operator<(const REFloat16 & a, const REFloat32 & b) { return (a.Get() < b); }
	
	/// Overloaded 'Less than' operator.
	friend bool operator<(const REFloat32 & a, const REFloat16 & b) { return (a < b.Get()); }
	
	/// Overloaded 'Greater than or equal to' operator.
	bool operator>=(const REFloat16 & f) const { return (this->Get() >= f.Get()); }
	
	/// Overloaded 'Greater than or equal to' operator.
	bool operator>=(const REFloat32 & f) const { return (this->Get() >= f); }
	
	/// Overloaded 'Greater than or equal to' operator.
	friend bool operator>=(const REFloat16 & a, const REFloat16 & b) { return (a.Get() >= b.Get()); }
	
	/// Overloaded 'Greater than or equal to' operator.
	friend bool operator>=(const REFloat16 & a, const REFloat32 & b) { return (a.Get() >= b); }
	
	/// Overloaded 'Greater than or equal to' operator.
	friend bool operator>=(const REFloat32 & a, const REFloat16 & b) { return (a >= b.Get()); }
	
	/// Overloaded 'Less than or equal to' operator.
	bool operator<=(const REFloat16 & f) const { return (this->Get() <= f.Get()); }
	
	/// Overloaded 'Less than or equal to' operator.
	bool operator<=(const REFloat32 & f) const { return (this->Get() <= f); }
	
	/// Overloaded 'Less than or equal to' operator.
	friend bool operator<=(const REFloat16 & a, const REFloat16 & b) { return (a.Get() <= b.Get()); }
	
	/// Overloaded 'Less than or equal to' operator.
	friend bool operator<=(const REFloat16 & a, const REFloat32 & b) { return (a.Get() <= b); }
	
	/// Overloaded 'Less than or equal to' operator.
	friend bool operator<=(const REFloat32 & a, const REFloat16 & b) { return (a <= b.Get()); }
	
	/// Overloaded 'Addition assignment' operator.
	REFloat16 & operator+=(const REFloat16 & f) { this->Set(this->Get() + f.Get()); return (*this); }
	
	/// Overloaded 'Addition assignment' operator.
	REFloat16 & operator+=(const REFloat32 & f) { this->Set(this->Get() + f); return (*this); }
	
	/// Overloaded 'Addition assignment' operator.
	friend REFloat16 & operator+=(REFloat16 & a, const REFloat16 & b) { a.Set(a.Get() + b.Get()); return a; }
	
	/// Overloaded 'Addition assignment' operator.
	friend REFloat16 & operator+=(REFloat16 & a, const REFloat32 & b) { a.Set(a.Get() + b); return a; }
	
	/// Overloaded 'Subtraction assignment' operator.
	REFloat16 & operator-=(const REFloat16 & f) { this->Set(this->Get() - f.Get()); return (*this); }
	
	/// Overloaded 'Subtraction assignment' operator.
	REFloat16 & operator-=(const REFloat32 & f) { this->Set(this->Get() - f); return (*this); }
	
	/// Overloaded 'Subtraction assignment' operator.
	friend REFloat16 & operator-=(REFloat16 & a, const REFloat16 & b) { a.Set(a.Get() - b.Get()); return a; }
	
	/// Overloaded 'Subtraction assignment' operator.
	friend REFloat16 & operator-=(REFloat16 & a, const REFloat32 & b) { a.Set(a.Get() - b); return a; }
	
	/// Overloaded 'Multiplication assignment' operator.
	REFloat16 & operator*=(const REFloat16 & f) { this->Set(this->Get() * f.Get()); return (*this); }
	
	/// Overloaded 'Multiplication assignment' operator.
	REFloat16 & operator*=(const REFloat32 & f) { this->Set(this->Get() * f); return (*this); }
	
	/// Overloaded 'Multiplication assignment' operator.
	friend REFloat16 & operator*=(REFloat16 & a, const REFloat16 & b) { a.Set(a.Get() * b.Get()); return a; }
	
	/// Overloaded 'Multiplication assignment' operator.
	friend REFloat16 & operator*=(REFloat16 & a, const REFloat32 & b) { a.Set(a.Get() * b); return a; }
	
	/// Overloaded 'Division assignment' operator.
	REFloat16 & operator/=(const REFloat16 & f) { this->Set(this->Get() / f.Get()); return (*this); }
	
	/// Overloaded 'Division assignment' operator.
	REFloat16 & operator/=(const REFloat32 & f) { this->Set(this->Get() / f); return (*this); }
	
	/// Overloaded 'Division assignment' operator.
	friend REFloat16 & operator/=(REFloat16 & a, const REFloat16 & b) { a.Set(a.Get() / b.Get()); return a; }
	
	/// Overloaded 'Division assignment' operator.
	friend REFloat16 & operator/=(REFloat16 & a, const REFloat32 & b) { a.Set(a.Get() / b); return a; }
	
	/// Initialized with float16 value.
	REFloat16(const REFloat16 & f) : _data(f._data) { }
	
	/// Initialized with float32 value.
	REFloat16(const REFloat32 & f) { this->Set(f); }
	
	/// Initialized with float64 value.
	REFloat16(const REFloat64 & f) { this->Set((REFloat32)f); }
	
	/// Initialized with zero value.
	REFloat16() : _data(0) { }
	
	/// Destructor.
	~REFloat16() { }
};

#endif


#endif /* __REFLOAT16_H__ */


