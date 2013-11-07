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

/**
 @brief Base data base class.
 @detailed Class for operating with database connection.
 */
class __RE_PUBLIC_CLASS_API__ REDB
{
protected:
	REDB();
	
public:
	/**
	 @brief Database path getter.
	 @detailed Getting opened database path.
	 @return String with path or empty string.
	 */
	virtual REString databaseFullPath() const;
	
	/**
	 @brief Open database connection.
	 @return True if opened otherwice false.
	 */
	virtual REBOOL open();
	
	/**
	 @brief Check is database opened.
	 @return True if opened otherwice false.
	 */
	virtual REBOOL isOpened() const;
	
	/**
	 @brief Closes database.
	 @return True if successfully closed otherwice false.
	 */
	virtual REBOOL close();
	
	/**
	 @brief Getter for last inserted row identifier.
	 @return 64 bit signed integer of last inserted row identifier.
	 */
	virtual REInt64 lastInsertedRowId() const;
	
	/**
	 @brief Executes SQL query.
	 @param queryString string with SQL query.
	 @return Autopointer with result as REDBResultSet of execution.
	 */
	virtual REPtr<REDBResultSet> executeQuery(const REString & queryString);
	
	/**
	 @brief Begin transaction.
	 @return True if successfully started otherwice false.
	 */
	virtual REBOOL beginTransaction();
	
	/**
	 @brief Executes SQL update query.
	 @param queryString string with SQL query.
	 @return True if successfully started otherwice false.
	 */
	virtual REBOOL executeUpdate(const REString & queryString);
	
	/**
	 @brief Commiting transaction.
	 @detailed Commiting last opened transaction using method beginTransaction().
	 @code
	 REDB db;
	 ... open data base
	 db.beginTransaction();
	 ... batabase changes
	 db.commitTransaction();
	 @endcode
	 @return True if successfully commited otherwice false.
	 */
	virtual REBOOL commitTransaction();
	
	/**
	 @brief Rolls back transaction.
	 @detailed Rolls back last opened transaction opened using method beginTransaction().
	 @code
	 REDB db;
	 ... open data base
	 db.beginTransaction();
	 ... batabase changes
	 db.rollBackTransaction();
	 @endcode
	 @return True if successfully commited otherwice false.
	 */
	virtual REBOOL rollBackTransaction();
	
	virtual ~REDB();
	
	/**
	 @brief Create SQLite database.
	 @detailed Create opened SQLite database with file path.
	 @param dbFilePath string with path to SQLite file.
	 @return Autopointer with opened SQLite database object or with NULL object.
	 */
	static REPtr<REDB> createSQLiteWithFilePath(const REString & dbFilePath);
};



#endif /* __REDB_H__ */


