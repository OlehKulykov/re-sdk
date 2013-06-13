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


#ifndef __RESQLITEDBRESULTSET_H__
#define __RESQLITEDBRESULTSET_H__


#include "RECommonHeader.h"
#include "REArray.h"
#include "REString.h"

/// Class of Sqlite data base result sets.
class __RE_PUBLIC_CLASS_API__ RESQLiteDBResultSet
{
public:
	typedef struct _statementStruct
	{
		void * db;
		void * statement;
		REArray<REString *> * columnsNames;
		REUInt32 usingCount;
	}
	StatementStruct;
private:
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__
	RESQLiteDBResultSet::StatementStruct * _st;
#endif
	
public:
	/// Checks is result set null(empty).
	REBOOL IsNull() const;
	
	/// Closes result set.
	void Close();
	
	/// Receive next results from data base.
	REBOOL Next() const;
	
	/// Checks is has next row of results.
	REBOOL IsHasAnotherRow() const;
	
	/// Returns column index by it's name.
	/// Returns RENotFound if not found.
	REUInt32 GetColumnIndexFromName(const REString & columnName) const;
	
	/// Returns Boolean value for column name.
	REBOOL GetBoolForColumnName(const REString & columnName) const;
	
	/// Returns Boolean value for column index.
	REBOOL GetBoolForColumnIndex(const REUInt32 index) const;
	
	/// Returns Integer value for column name.
	REInt32 GetIntForColumnName(const REString & columnName) const;
	
	/// Returns Integer value for column index.
	REInt32 GetIntForColumnIndex(const REUInt32 index) const;
	
	/// Returns Float value for column name.
	REFloat64 GetFloatForColumnName(const REString & columnName) const;
	
	/// Returns Integer value for column index.
	REFloat64 GetFloatForColumnIndex(const REUInt32 index) const;
	
	/// Returns Long Integer value for column name.
	REInt64 GetLongIntForColumnName(const REString & columnName) const;
	
	/// Returns Long Integer value for column index.
	REInt64 GetLongIntForColumnIndex(const REUInt32 index) const;
	
	/// Returns String value for column name.
	REString GetStringForColumnName(const REString & columnName) const;
	
	/// Returns String value for column index.
	REString GetStringForColumnIndex(const REUInt32 index) const;
    
	/// Assign operator using another result set.
	RESQLiteDBResultSet & operator=(const RESQLiteDBResultSet & anotherSet);
	
	/// Constructs result set from another result sets object.
	RESQLiteDBResultSet(const RESQLiteDBResultSet & anotherSet);
	
	/// Constructs result set from statement struct.
	RESQLiteDBResultSet(RESQLiteDBResultSet::StatementStruct * statement = NULL);
	
	virtual ~RESQLiteDBResultSet();
};


#endif /* __RESQLITEDBRESULTSET_H__ */ 

