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


#ifndef __REDATE_H__
#define __REDATE_H__

#include "RECommonHeader.h"

/// Class presents date object.
class __RE_PUBLIC_CLASS_API__ REDate
{
private:
	RETimeInterval _seconds;
	REUInt16 _year;
	REUByte _month;
	REUByte _day;
	REUByte _hour;
	REUByte _minutes;
	
public:
	/// Getter for year.
	const REUInt32 GetYear() const;
	
	/// Getter for month.
	const REUInt32 GetMonth() const;
	
	/// Getter for day.
	const REUInt32 GetDay() const;
	
	/// Getter for hour.
	const REUInt32 GetHour() const;
	
	/// Getter for minutes.
	const REUInt32 GetMinutes() const;
	
	/// Getter for seconds.
	const RETimeInterval GetSeconds() const;
	
	/// Getter for milliseconds.
	const REUInt32 GetMilliseconds() const;
	
	/// Getter for microseconds.
	const REUInt32 GetMicroseconds() const;
	
	/// Setting date year.
	void SetYear(const REUInt32 year);
	
	/// Setting date month.
	void SetMonth(const REUInt32 month);
	
	/// Setting date day.
	void SetDay(const REUInt32 day);
	
	/// Setting date hour.
	void SetHour(const REUInt32 hour);
	
	/// Setting date minutes.
	void SetMinutes(const REUInt32 minutes);
	
	/// Setting date seconds.
	void SetSeconds(const RETimeInterval seconds);
	
	/// Assing operator.
	REDate & operator=(const REDate & anotherDate);
	
	/// Constructs date from another date object.
	REDate(const REDate & anotherDate);
	
	/// Constructs empty date.
	REDate();
	
	/// Desctructor.
	virtual ~REDate();
	
	/// Returns days count of month for year.
	static REUInt32 GetDaysPerMonth(const REUInt32 month, const REUInt32 year);
	
	/// Checks is year leap.
	static REBOOL IsLeapYear(const REUInt32 year);
};

#endif /* __REDATE_H__ */


