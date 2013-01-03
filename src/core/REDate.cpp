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

#define IS_LEAP_YEAR(y) ((((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0)) ? 1 : 0)

const RETimeInterval REDate::GetSeconds() const { return _seconds; }
const REUInt32 REDate::GetYear() const { return _year; }
const REUInt32 REDate::GetMonth() const { return _month; }
const REUInt32 REDate::GetDay() const { return _day; }
const REUInt32 REDate::GetHour() const { return _hour; }
const REUInt32 REDate::GetMinutes() const { return _minutes; }
const REUInt32 REDate::GetMilliseconds() const
{
	double intPart; 
	const double fractPart = modf(_seconds, &intPart);
	return (REUInt32)(fractPart * 1000);
	// 1 second = 1 000 milliseconds
}

const REUInt32 REDate::GetMicroseconds() const
{
	double intPart; 
	const double fractPart = modf(_seconds, &intPart);
	return (REUInt32)(fractPart * 1000000);
	// 1 second = 1 000 000 microseconds
}

void REDate::SetYear(const REUInt32 year)
{
	_year = (REUInt16)year;
}

void REDate::SetMonth(const REUInt32 month)
{
	if ((month < 13) && (month > 0)) { _month = (REUByte)month; }
}

void REDate::SetDay(const REUInt32 day)
{
	if ((day < 32) && (day > 0)) { _day = (REUByte)day; }
}

void REDate::SetHour(const REUInt32 hour)
{
	if (hour < 24) { _hour = (REUByte)hour; }
}

void REDate::SetMinutes(const REUInt32 minutes)
{
	if (minutes < 60) { _minutes = (REUByte)minutes; }
}

void REDate::SetSeconds(const RETimeInterval seconds)
{
	if (seconds < 60.0) { _seconds = seconds; }
}

REDate & REDate::operator=(const REDate & anotherDate)
{
	_seconds = anotherDate._seconds;
	_year = anotherDate._year;
	_month = anotherDate._month;
	_day = anotherDate._day;
	_hour = anotherDate._hour;
	_minutes = anotherDate._minutes;
	
	return (*this);
}

REDate::REDate(const REDate & anotherDate) :
	_seconds(anotherDate._seconds),
	_year(anotherDate._year),
	_month(anotherDate._month),
	_day(anotherDate._day),
	_hour(anotherDate._hour),
	_minutes(anotherDate._minutes)
{
	
}

REDate::REDate() :
	_seconds(0.0),
	_year(0),
	_month(0),
	_day(0),
	_hour(0),
	_minutes(0)
{
	
}

REDate::~REDate()
{
	
}

REUInt32 REDate::GetDaysPerMonth(const REUInt32 month, const REUInt32 year)
{
	if (IS_LEAP_YEAR(year) && (month == 2))
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

REBOOL REDate::IsLeapYear(const REUInt32 year)
{
	return IS_LEAP_YEAR(year);
}

