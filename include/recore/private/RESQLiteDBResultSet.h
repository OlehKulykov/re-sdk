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


#include "../RECommonHeader.h"
#include "../REArray.h"
#include "../REString.h"
#include "../REDBResultSet.h"

/// Class of Sqlite data base result sets.
class RESQLiteDBResultSet : public REDBResultSet
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
	virtual REBOOL isOK() const { return (!this->isNull()); }
	
	/// Checks is result set null(empty).
	REBOOL isNull() const;
	
	/// Closes result set.
	void close();
	
	/// Receive next results from data base.
	REBOOL next() const;
	
	/// Checks is has next row of results.
	REBOOL isHasAnotherRow() const;
	
	/// Returns column index by it's name.
	/// Returns RENotFound if not found.
	REUInt32 columnIndexFromName(const REString & columnName) const;
	
	/// Returns Boolean value for column name.
	REBOOL boolForColumnName(const REString & columnName) const;
	
	/// Returns Boolean value for column index.
	REBOOL boolForColumnIndex(const REUInt32 index) const;
	
	/// Returns Integer value for column name.
	REInt32 intForColumnName(const REString & columnName) const;
	
	/// Returns Integer value for column index.
	REInt32 intForColumnIndex(const REUInt32 index) const;
	
	/// Returns Float value for column name.
	REFloat64 floatForColumnName(const REString & columnName) const;
	
	/// Returns Integer value for column index.
	REFloat64 floatForColumnIndex(const REUInt32 index) const;
	
	/// Returns Long Integer value for column name.
	REInt64 longIntForColumnName(const REString & columnName) const;
	
	/// Returns Long Integer value for column index.
	REInt64 longIntForColumnIndex(const REUInt32 index) const;
	
	/// Returns String value for column name.
	REString stringForColumnName(const REString & columnName) const;
	
	/// Returns String value for column index.
	REString stringForColumnIndex(const REUInt32 index) const;
    
	/// Assign operator using another result set.
	RESQLiteDBResultSet & operator=(const RESQLiteDBResultSet & anotherSet);
	
	/// Constructs result set from another result sets object.
	RESQLiteDBResultSet(const RESQLiteDBResultSet & anotherSet);
	
	/// Constructs result set from statement struct.
	RESQLiteDBResultSet(RESQLiteDBResultSet::StatementStruct * statement = NULL);
	
	virtual ~RESQLiteDBResultSet();
};


#endif /* __RESQLITEDBRESULTSET_H__ */ 

