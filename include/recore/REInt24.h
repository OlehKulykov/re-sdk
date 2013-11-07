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


#ifndef __REINT24_H__
#define __REINT24_H__

#include "RECommonHeader.h"

#define INT24_MAX (8388607)

class __RE_PUBLIC_CLASS_API__ REInt24
{
protected:
	REUByte _data[3];
	
public:
	REInt24()
    {
		memset(_data, 0, 3);
    }
	
    REInt24(const REInt32 v)
    {
		this->set(v);
    }
	
    REInt24(const REInt24 & v)
    {
		this->set(v);
    }
	
	REInt32 get() const
	{
		if (_data[2] & 0x80)
        {
            return (0xff << 24) | (_data[2] << 16) | (_data[1] << 8) | (_data[0] << 0);
        }
        else
        {
            return (_data[2] << 16) | (_data[1] << 8) | (_data[0] << 0);
        }
	}
	
	REInt24 & set(const REInt24 & v)
	{
		memcpy(_data, v._data, 3);
		return (*this);
	}
	
	REInt24 & set(const REInt32 v)
    {
		memcpy(_data, &v, 3);
		return (*this);
    }

    operator REInt32() const
    {
        return this->get();
    }
	
    operator REFloat32() const
    {
        return (REFloat32)this->get();
    }
	
    REInt24 & operator=(const REInt24 & v)
    {
        return this->set(v);
    }
	
    REInt24 & operator=(const REInt32 v)
    {
		return this->set(v);
    }

    REInt24 operator+(const REInt24 & v) const
    {
        return REInt24(this->get() + v.get());
    }
	
    REInt24 operator-(const REInt24 & v) const
    {
        return REInt24(this->get() - v.get());
    }
	
    REInt24 operator*(const REInt24 & v) const
    {
        return REInt24(this->get() * v.get());
    }
	
    REInt24 operator/(const REInt24 & v) const
    {
        return REInt24(this->get() / v.get());
    }
	
    REInt24 operator+(const REInt32 v) const
    {
        return REInt24(this->get() + v);
    }
	
    REInt24 operator-(const REInt32 v) const
    {
        return REInt24(this->get() - v);
    }
	
    REInt24 operator*(const REInt32 v) const
    {
        return REInt24(this->get() * v);
    }
	
    REInt24 operator/(const REInt32 v) const
    {
        return REInt24(this->get() / v);
    }
	
    REInt24 & operator+=(const REInt24 & v)
    {
		return this->set(this->get() + v.get());
    }
	
    REInt24 & operator-=(const REInt24 & v)
    {
		return this->set(this->get() - v.get());
    }
	
    REInt24 & operator*=(const REInt24 & v)
    {
		return this->set(this->get() * v.get());
    }
	
    REInt24 & operator/=(const REInt24 & v)
    {
		return this->set(this->get() / v.get());
    }
	
    REInt24 & operator+=(const REInt32 v)
    {
		return this->set(this->get() + v);
    }
	
    REInt24 & operator-=(const REInt32 v)
    {
		return this->set(this->get() - v);
    }
	
    REInt24 & operator*=(const REInt32 v)
    {
		return this->set(this->get() * v);
    }
	
    REInt24 & operator/=(const REInt32 v)
    {
		return this->set(this->get() / v);
    }
	
    REInt24 operator>>(const REInt32 v) const
    {
        return REInt24(this->get() >> v);
    }
	
    REInt24 operator<<(const REInt32 v) const
    {
        return REInt24(this->get() << v);
    }
	
    REInt24 & operator>>=(const REInt32 v)
    {
		return this->set(this->get() >> v);
    }
	
    REInt24 & operator<<=(const REInt32 v)
    {
		return this->set(this->get() << v);
    }
	
    operator bool() const
    {
        return (this->get() != 0);
    }
	
    bool operator !() const
    {
        return  !this->get();
    }
	
    REInt24 operator-()
    {
        return REInt24(-this->get());
    }
	
    bool operator==(const REInt24 & v) const
    {
		return (this->get() == v.get());
    }
	
    bool operator!=(const REInt24 & v) const
    {
		return (this->get() != v.get());
    }
	
    bool operator>=(const REInt24 & v) const
    {
		return (this->get() >= v.get());
    }
	
    bool operator<=(const REInt24 & v) const
    {
		return (this->get() <= v.get());
    }
	
    bool operator>(const REInt24 & v) const
    {
		return (this->get() > v.get());
    }
	
    bool operator<(const REInt24 & v) const
    {
		return (this->get() < v.get());
    }
	
    bool operator==(const REInt32 v) const
    {
		return (this->get() == v);
    }
	
    bool operator!=(const REInt32 v) const
    {
		return (this->get() != v);
    }
	
    bool operator>=(const REInt32 v) const
    {
		return (this->get() >= v);
    }
	
    bool operator<=(const REInt32 v) const
    {
		return (this->get() <= v);
    }
	
    bool operator>(const REInt32 v) const
    {
		return (this->get() > v);
    }
	
    bool operator<(const REInt32 v) const
    {
		return (this->get() < v);
    }
};

#endif /* __REINT24_H__ */

