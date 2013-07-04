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


#include "../../include/recore/REDate.h"
#include "../../include/recore/REString.h"

class REDateInternal 
{
private:
	void setTimeZone(const char * tz)
	{
		RE_SAFE_FREE(timestruct.tm_zone);
		if (tz)
		{
			const size_t len = strlen(tz);
			if (len > 0)
			{
				timestruct.tm_zone = (char *)malloc(len + 1);
				memcpy(timestruct.tm_zone, tz, len);
				timestruct.tm_zone[len] = 0;
			}
		}
	}
	
public:
	time_t rawtime;
	struct tm timestruct;
	
	REDateInternal(const REDateInternal & di) :
		rawtime(di.rawtime),
		timestruct(di.timestruct)
	{
		timestruct.tm_zone = 0;
		this->setTimeZone(di.timestruct.tm_zone);
	}
	
	REDateInternal(const time_t rt, const struct tm & ts) : 
		rawtime(rt),
		timestruct(ts)
	{
		timestruct.tm_zone = 0;
		this->setTimeZone(ts.tm_zone);
	}
	
	~REDateInternal()
	{
		RE_SAFE_FREE(timestruct.tm_zone);
	}
};

#define IS_LEAP_YEAR(y) ((((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0)) ? 1 : 0)

REString REDate::toString() const
{
	if (_t.isNotEmpty())
	{
		//2012-01-10T12:48:12-07:00
		char buffer[64] = { 0 };
		const size_t writed = strftime(buffer, 64, "%Y-%m-%dT%H:%M:%ss-%z", &_t->timestruct);
		if (writed > 0)
		{
			return REString(buffer, (REUInt32)writed);
		}
	}
	return REString();
}

REBOOL REDate::isEqualToDate(const REDate & anotherDate) const
{
	//TODO: ...
	return false;
}

const RETimeInterval REDate::getSeconds() const 
{
	return _t.isNotEmpty() ? _t->timestruct.tm_sec : 0; 
}

const REUInt32 REDate::getYear() const 
{
	return _t.isNotEmpty() ? (1900 + _t->timestruct.tm_year) : 0;
}

const REUInt32 REDate::getMonth() const 
{
	return _t.isNotEmpty() ? _t->timestruct.tm_mon : 0;
}

const REUInt32 REDate::getDay() const
{
	return _t.isNotEmpty() ? _t->timestruct.tm_mday : 0;
}

const REUInt32 REDate::getHour() const 
{
	return _t.isNotEmpty() ? _t->timestruct.tm_hour : 0;
}

const REUInt32 REDate::getMinutes() const
{ 
	return _t.isNotEmpty() ? _t->timestruct.tm_min : 0;
}

const REUInt32 REDate::getMilliseconds() const
{
	double intPart; 
	const RETimeInterval seconds = _t.isNotEmpty() ? _t->timestruct.tm_sec : 0;
	const double fractPart = modf(seconds, &intPart);
	return (REUInt32)(fractPart * 1000);
	// 1 second = 1 000 milliseconds
}

const REUInt32 REDate::getMicroseconds() const
{
	double intPart; 
	const RETimeInterval seconds = _t.isNotEmpty() ? _t->timestruct.tm_sec : 0;
	const double fractPart = modf(seconds, &intPart);
	return (REUInt32)(fractPart * 1000000);
	// 1 second = 1 000 000 microseconds
}

void REDate::makeDateInternalCopy()
{
	REDateInternal * oldInternal = _t;
	if (oldInternal)
	{
		REDateInternal * newInternal = new REDateInternal(*oldInternal);
		_t = newInternal;
	}
}

void REDate::setYear(const REUInt32 year)
{
	if (_t.isMultipleOwners())
	{
		this->makeDateInternalCopy();
	}
	
	if (_t.isNotEmpty())
	{
		_t->timestruct.tm_year = year - 1900;
	}
}

void REDate::setMonth(const REUInt32 month)
{

}

void REDate::setDay(const REUInt32 day)
{

}

void REDate::setHour(const REUInt32 hour)
{

}

void REDate::setMinutes(const REUInt32 minutes)
{

}

void REDate::setSeconds(const RETimeInterval seconds)
{
	
}

REDate & REDate::operator=(const REDate & anotherDate)
{
	_t = anotherDate._t;	
	return (*this);
}

REDate::REDate(const REDate & anotherDate)
{
	_t = anotherDate._t;
}

REDate::REDate()
{
/*
 struct tm {
 int tm_sec;   // seconds of minutes from 0 to 61
 int tm_min;   // minutes of hour from 0 to 59
 int tm_hour;  // hours of day from 0 to 24
 int tm_mday;  // day of month from 1 to 31
 int tm_mon;   // month of year from 0 to 11
 int tm_year;  // year since 1900
 int tm_wday;  // days since sunday
 int tm_yday;  // days since January 1st
 int tm_isdst; // hours of daylight savings time
 }
*/	
	time_t rawtime = 0;
	time(&rawtime);
	struct tm * ptm = gmtime(&rawtime);
	if (ptm)
	{
		REDateInternal * s = new REDateInternal(rawtime, *ptm);
		if (s)
		{
			_t = s;
		}
	}
}

REDate::~REDate()
{
	
}

REUInt32 REDate::getDaysPerMonth(const REUInt32 month, const REUInt32 year)
{
	if ( (IS_LEAP_YEAR(year)) && (month == 2))
	{
		return 29;
	}

	switch (month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
			break;
		case 2:
			return 28;
			break;
		default:
			break;
	}
	return 0;
}

REBOOL REDate::isLeapYear(const REUInt32 year)
{
	return IS_LEAP_YEAR(year);
}

