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
#include "../../include/recore/REMath.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

#if defined(HAVE_TIME_H)
#include <time.h>
#endif

class REDateInternal 
{
private: 
	void setTimeZone(const char * tz);
	
public:
	time_t rawtime;
	struct tm timestruct;
	
	REDateInternal(const REDateInternal & di);
	REDateInternal(const time_t rt, struct tm ts);
	~REDateInternal();
};

void REDateInternal::setTimeZone(const char * tz)
{
	if (timestruct.tm_zone) 
	{
		char * prevTZ = const_cast<char *>(timestruct.tm_zone);
		free(prevTZ);
		timestruct.tm_zone = NULL;
	}

	if (tz)
	{
		const size_t len = strlen(tz);
		if (len > 0)
		{
			char * newTMZone = (char *)malloc(len + 1);
			if (newTMZone) 
			{
				memcpy(newTMZone, tz, len);
				newTMZone[len] = 0;
				timestruct.tm_zone = newTMZone;
			}
		}
	}
}

REDateInternal::REDateInternal(const REDateInternal & di) :
	rawtime(di.rawtime),
	timestruct(di.timestruct)
{
	timestruct.tm_zone = NULL;
	this->setTimeZone(di.timestruct.tm_zone);
}

REDateInternal::REDateInternal(const time_t rt, struct tm ts) : 
	rawtime(rt),
	timestruct(ts)
{
	timestruct.tm_zone = NULL;
	this->setTimeZone(ts.tm_zone);
}

REDateInternal::~REDateInternal()
{
	if (timestruct.tm_zone) 
	{
		char * prevTZ = const_cast<char *>(timestruct.tm_zone);
		free(prevTZ);
	}
}

#define IS_LEAP_YEAR(y) ((((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0)) ? 1 : 0)

const RETimeInterval REDate::difference(const REDate & anotherDate) const
{
	struct tm t1 = _t->timestruct;
	struct tm t2 = anotherDate._t->timestruct;
	
	const time_t tt1 = mktime(&t1);
	const time_t tt2 = mktime(&t2);
	
	// !!!! 2 - 1 - normal first less next
	const double dif = difftime(tt2, tt1);
	return (RETimeInterval)dif;
}

RETimeInterval REDate::timeIntervalSince1970() const
{
	return _t.isNotEmpty() ? (RETimeInterval)_t->rawtime : 0;
}

const RETimeInterval REDate::minimumDifference(const REDate & anotherDate) const
{
	struct tm t1 = _t->timestruct;
	struct tm t2 = anotherDate._t->timestruct;
	
	const time_t tt1 = mktime(&t1);
	time_t tt2 = mktime(&t2);
	
	// !!!! 2 - 1 - normal first less next
	const double dif1 = difftime(tt2, tt1);
	
	t2.tm_mday++;
	tt2 = mktime(&t2);
	
	const double dif2 = difftime(tt2, tt1);
	
	if ( (ABS(dif1)) < (ABS(dif2)) ) 
	{
		return (RETimeInterval)dif1;
	}
	
	return (RETimeInterval)dif2;
}

const char * REDate::formatString()
{
	return "%Y-%m-%dT%H:%M:%ss-%z";
}

REString REDate::toString() const
{
	if (_t.isNotEmpty())
	{
		//2012-01-10T12:48:12-07:00
		char buffer[64] = { 0 };
		const size_t writed = strftime(buffer, 64, REDate::formatString(), &_t->timestruct);
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

const RETimeInterval REDate::seconds() const 
{
	return _t.isNotEmpty() ? _t->timestruct.tm_sec : 0; 
}

const REUInt32 REDate::year() const 
{
	return _t.isNotEmpty() ? (1900 + _t->timestruct.tm_year) : 0;
}

const REUInt32 REDate::month() const 
{
	return _t.isNotEmpty() ? _t->timestruct.tm_mon : 0;
}

const REUInt32 REDate::day() const
{
	return _t.isNotEmpty() ? _t->timestruct.tm_mday : 0;
}

const REUInt32 REDate::hour() const 
{
	return _t.isNotEmpty() ? _t->timestruct.tm_hour : 0;
}

const REUInt32 REDate::minutes() const
{ 
	return _t.isNotEmpty() ? _t->timestruct.tm_min : 0;
}

const REUInt32 REDate::milliseconds() const
{
	double intPart; 
	const RETimeInterval seconds = _t.isNotEmpty() ? _t->timestruct.tm_sec : 0;
	const double fractPart = modf(seconds, &intPart);
	return (REUInt32)(fractPart * 1000);
	// 1 second = 1 000 milliseconds
}

const REUInt32 REDate::microseconds() const
{
	double intPart; 
	const RETimeInterval seconds = _t.isNotEmpty() ? _t->timestruct.tm_sec : 0;
	const double fractPart = modf(seconds, &intPart);
	return (REUInt32)(fractPart * 1000000);
	// 1 second = 1 000 000 microseconds
}

void REDate::setYear(const REUInt32 year)
{	
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
	if (anotherDate._t.isNotEmpty())
	{
		_t = REPtr<REDateInternal>(new REDateInternal(*anotherDate._t));
	}
	return (*this);
}

REDate::REDate(const REDate & anotherDate)
{
	if (anotherDate._t.isNotEmpty())
	{
		_t = REPtr<REDateInternal>(new REDateInternal(*anotherDate._t));
	}
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
		_t = REPtr<REDateInternal>(new REDateInternal(rawtime, *ptm));
	}
}

REDate::~REDate()
{
	
}

REUInt32 REDate::getDaysPerMonth(const REUInt32 month, const REUInt32 year)
{
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
			return (IS_LEAP_YEAR(year)) ? 29 : 28;
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

