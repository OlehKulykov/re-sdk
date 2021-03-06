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
#include "REPtr.h"

class REDateInternal;
class REString;

/// Class presents date object.
class __RE_PUBLIC_CLASS_API__ REDate
{
private:
	REPtr<REDateInternal> _t;
	
public:
	const RETimeInterval difference(const REDate & anotherDate) const;
	
	const RETimeInterval minimumDifference(const REDate & anotherDate) const;
	
	RETimeInterval timeIntervalSince1970() const;
	
	/// Using defaultFormatString
	REString toString() const;
	
	REBOOL isEqualToDate(const REDate & anotherDate) const;
	
	/// Getter for year.
	const REUInt32 year() const;
	
	/// Getter for month.
	const REUInt32 month() const;
	
	/// Getter for day.
	const REUInt32 day() const;
	
	/// Getter for hour.
	const REUInt32 hour() const;
	
	/// Getter for minutes.
	const REUInt32 minutes() const;
	
	/// Getter for seconds.
	const RETimeInterval seconds() const;
	
	/// Getter for milliseconds.
	const REUInt32 milliseconds() const;
	
	/// Getter for microseconds.
	const REUInt32 microseconds() const;
	
	/// Setting date year.
	void setYear(const REUInt32 year);
	
	/// Setting date month.
	void setMonth(const REUInt32 month);
	
	/// Setting date day.
	void setDay(const REUInt32 day);
	
	/// Setting date hour.
	void setHour(const REUInt32 hour);
	
	/// Setting date minutes.
	void setMinutes(const REUInt32 minutes);
	
	/// Setting date seconds.
	void setSeconds(const RETimeInterval seconds);
	
	/// Assing operator.
	REDate & operator=(const REDate & anotherDate);
	
	/// Constructs date from another date object.
	REDate(const REDate & anotherDate);
	
	/// Constructs empty date.
	REDate();
	
	/// Desctructor.
	virtual ~REDate();
	
	/// Returns days count of month for year.
	static REUInt32 getDaysPerMonth(const REUInt32 month, const REUInt32 year);
	
	/// Checks is year leap.
	static REBOOL isLeapYear(const REUInt32 year);
	
	static const char * formatString();
};

#endif /* __REDATE_H__ */


