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


#ifndef __REDB_H__
#define __REDB_H__


#include "RECommonHeader.h"
#include "REPtr.h"
#include "REString.h"
#include "REDBResultSet.h"

/// Base class of data base.
class __RE_PUBLIC_CLASS_API__ REDB
{
protected:
	REDB();
	
public:
	virtual REString getDatabaseFullPath() const;
	
	/// Opens data base.
	virtual REBOOL open();
	
	/// Check is database opened.
	virtual REBOOL isOpened() const;
	
	/// Closes data base.
	virtual REBOOL close();
	
	/// Returns last inserted row id.
	virtual REInt64 getLastInsertedRowId() const;
	
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
	
	virtual ~REDB();
	
	static REPtr<REDB> createSQLiteWithFilePath(const REString & dbFilePath);
};



#endif /* __REDB_H__ */


