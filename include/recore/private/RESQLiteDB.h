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


#ifndef __RESQLITEDB_H__
#define __RESQLITEDB_H__


#include "../RECommonHeader.h"
#include "../REDB.h"
#include "RESQLiteDBResultSet.h"

#if (defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) || defined(__USING_RECORE_DYNAMIC_LIBRARY__))
/// Warning message 4251: Class 'REArray<T>' needs to have dll-interface to be used by clients of class.
/// Why disable 4251: Class 'REArray<T>' defined as private field and no clents can access it.
/// To access must use public methods.
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif
#endif


/// Class of Sqlite data base.
class RESQLiteDB : public REDB
{
private:
	REString _databasePath;
	void * _db;
	REArray<RESQLiteDBResultSet::StatementStruct *> _statements;

	void removeUnUsedStates();
	
	static REBOOL setColumnsNames(RESQLiteDBResultSet::StatementStruct * st);
	
	REBOOL closeSQLiteDB();
	
public:
	/// Returns path to database.
	virtual REString databaseFullPath() const;
	
	/// Opens data base.
	virtual REBOOL open();
	
	/// Check is database opened.
	virtual REBOOL isOpened() const;
	
	/// Closes data base.
	virtual REBOOL close();
	
	/// Returns last inserted row id.
	virtual REInt64 lastInsertedRowId() const;
	
	/// Executes sql query string.
	virtual REPtr<REDBResultSet> executeQuery(const REString & queryString);
	
	/// Begins transaction.
	virtual REBOOL beginTransaction();
	
	/// Executes sql update query strind.
	virtual REBOOL executeUpdate(const REString & queryString);
	
	/// Commiting last transaction.
	virtual REBOOL commitTransaction();
	
	/// Rolls back last transaction.
	virtual REBOOL rollBackTransaction();
	
	/// Constructs bata base with file path.
	RESQLiteDB(const REString & dbFilePath);
	
	/// Destructor.
	virtual ~RESQLiteDB();
};



#endif /* __RESQLITEDB_H__ */


