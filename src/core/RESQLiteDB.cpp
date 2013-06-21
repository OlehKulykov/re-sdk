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


#include "../../include/recore/RESQLiteDB.h"

#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	

#include "../../include/recore/REThread.h"

#ifdef __RE_USING_ADITIONAL_SQLITE_LIBRARY__
#include "../addlibs/sqlite.h"
#endif


#ifdef __RE_USING_SYSTEM_SQLITE_LIBRARY__
#include <sqlite3.h>
#endif

#endif


void RESQLiteDB::removeUnUsedStates()
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	REUInt32 i = 0;
	while (i < _statements.count()) 
	{
		RESQLiteDBResultSet::StatementStruct * st = _statements[i];
		if ( st->usingCount == 0 ) 
		{
			if ( _statements.removeAt(i) )
			{
                if (st->columnsNames) 
                {
                    for (REUInt32 i = 0; i < st->columnsNames->count(); i++) 
                    {
                        REString * colName = (*st->columnsNames)[i];
                        delete colName;
                    }
                    delete st->columnsNames;
                }
				sqlite3_finalize((sqlite3_stmt *)st->statement);
				free(st);
			}
			else 
			{
				i++;
			}
		}
		else 
		{
			i++;
		}
	}
#endif	
}

REBOOL RESQLiteDB::setColumnsNames(RESQLiteDBResultSet::StatementStruct * st)
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
	if ( st ) 
	{
		sqlite3_stmt * statement = (sqlite3_stmt *)st->statement;
		const int columnsCount = sqlite3_column_count(statement);
		if ( columnsCount < 0 ) 
		{
			return false;
		}
		
		REArray<REString *> * arr = new REArray<REString *>(columnsCount);
		if ( arr == NULL ) 
		{
			return false;
		}
		
		for (int index = 0; index < columnsCount; index++) 
		{
			const char * name = sqlite3_column_name(statement, index);
			REString * nameStr = new REString(name);
			if ( nameStr ) 
			{
				nameStr->toLower();
				arr->add(nameStr);
			}
		}
		st->columnsNames = arr;
		return true;
	}
#endif	
	return false;
}


REBOOL RESQLiteDB::beginTransaction()
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
    if ( this->executeUpdate(REString("BEGIN EXCLUSIVE TRANSACTION;")) ) 
	{
		return true;
    }
#endif	
    return false;
}

REBOOL RESQLiteDB::executeUpdate(const REString & queryString)
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
	if ( this->isOpened() ) 
	{
		/*
		REString normQueryStr(queryString);
		if ( normQueryStr.IsWideString() ) 
		{
			normQueryStr.ToCString();
		}
		*/
		sqlite3_stmt * pStmt = NULL;
		const char * utf8QueryString = queryString.UTF8String();
		int rc = sqlite3_prepare_v2((sqlite3 *)_db, utf8QueryString, -1, &pStmt, 0);
		if (rc != SQLITE_OK) 
		{
			sqlite3_finalize(pStmt);
			return false;
		}
			
		const int queryCount = sqlite3_bind_parameter_count(pStmt);
		if ( queryCount != 0 ) 
		{
			sqlite3_finalize(pStmt);
			return false;
		}
		
		rc = sqlite3_step(pStmt);
		if ( (rc != SQLITE_DONE) && (rc != SQLITE_ROW) ) 
		{
			sqlite3_finalize(pStmt);
			return false;
		}
		
		rc = sqlite3_finalize(pStmt);
		return (rc == SQLITE_OK);
	}
#endif
	
	return false;
}

REBOOL RESQLiteDB::commitTransaction()
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
	if ( this->executeUpdate(REString("COMMIT TRANSACTION;")) ) 
	{
		return true;
    }
#endif
    return false;
}

REBOOL RESQLiteDB::rollBackTransaction()
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
	if ( this->executeUpdate(REString("ROLLBACK TRANSACTION;")) ) 
	{
		return true;
    }
#endif	
    return false;
}

const REString & RESQLiteDB::getDatabasePath() const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
	return _databasePath;
#else
	static REString emptyString;
	return emptyString;
#endif
}

REBOOL RESQLiteDB::close()
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
	if ( _db ) 
	{
		this->removeUnUsedStates();
        
		if ( !_statements.isEmpty() ) { this->removeUnUsedStates(); }
		
		if ( _statements.isEmpty() ) 
		{
            int numberOfRetries = 10;
            int rc = SQLITE_ERROR;
            do
            {
                sqlite3 * db = (sqlite3 *)_db;
                rc = sqlite3_close(db);
                if (rc == SQLITE_BUSY) 
                {
					REThread::uSleep(20);
                    numberOfRetries--;
                    if (numberOfRetries == 0) { return false; }
                }
            }
            while (rc != SQLITE_OK);
            
            _db = NULL;
            return true;
		}
	}
#endif	
	return false;
}

REInt64 RESQLiteDB::getLastInsertedRowId() const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
	if ( _db ) 
	{
		sqlite_int64 ret = sqlite3_last_insert_rowid((sqlite3 *)_db);
		return ret;
	}
#endif	
    return 0;
}

REBOOL RESQLiteDB::open()
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
	this->close();
	
	if ( _databasePath.isEmpty() ) 
	{
		return false;
	}
	
	sqlite3 * sqDB = NULL;
	if(sqlite3_open(_databasePath.UTF8String(), &sqDB) == SQLITE_OK) 
	{
		_db = sqDB;
		return true;
	}
#endif    
	return false;
}

REBOOL RESQLiteDB::isOpened() const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
	return ( _db != NULL );
#else
	return false;
#endif
}

RESQLiteDBResultSet RESQLiteDB::executeQuery(const REString & queryString)
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
	if ( this->isOpened() ) 
	{
		/*
		REString normQueryStr(queryString);
		if ( normQueryStr.IsWideString() ) 
		{
			normQueryStr.ToCString();
		}
		*/
		sqlite3_stmt * pStmt = NULL;
		const int rc = sqlite3_prepare_v2((sqlite3 *)_db, queryString.UTF8String(), -1, &pStmt, 0);
		if (rc == SQLITE_OK) 
		{
			const int queryCount = sqlite3_bind_parameter_count(pStmt);
			if ( queryCount == 0 ) 
			{
				RESQLiteDBResultSet::StatementStruct * newState = (RESQLiteDBResultSet::StatementStruct *)malloc(sizeof(RESQLiteDBResultSet::StatementStruct));
				if ( newState ) 
				{
					newState->db = _db;
					newState->statement = pStmt;
					newState->usingCount = 0;
					if ( RESQLiteDB::setColumnsNames(newState) ) 
					{
						_statements.add(newState);
						return RESQLiteDBResultSet(newState);
					}
				}
			}
		}
		sqlite3_finalize(pStmt);
	}
#endif	
	return RESQLiteDBResultSet();
}

RESQLiteDB::RESQLiteDB(const REString & dbFilePath) 
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__
: _db(NULL)
#endif
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
	if ( !dbFilePath.isEmpty() ) 
	{
		_databasePath.set(dbFilePath);
		/*
		if ( _databasePath.IsWideString() ) 
		{
			_databasePath.ToCString();
		}
		*/
	}
#endif	
}

RESQLiteDB::~RESQLiteDB()
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__		
	this->close();
#endif	
}


