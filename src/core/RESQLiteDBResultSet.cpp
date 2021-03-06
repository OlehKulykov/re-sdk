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


#include "../../include/recore/private/RESQLiteDB.h"
#include "../../include/recore/private/RESQLiteDBResultSet.h"


#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

#if !defined(__RE_CORE_NO_SQLITE_DATABASE_SUPPORT__) && defined(HAVE_SQLITE3_H)

#include <sqlite3.h>
#include "../../include/recore/REThread.h"
#ifndef IS_RE_USING_SQLITE
#define IS_RE_USING_SQLITE 1
#endif

#endif /* !defined(__RE_CORE_NO_SQLITE_DATABASE_SUPPORT__) && defined(HAVE_SQLITE3_H) */


REBOOL RESQLiteDBResultSet::isOK() const 
{
	return (!this->isNull()); 
}

REUInt32 RESQLiteDBResultSet::columnIndexFromName(const REString & columnName) const
{
#if (IS_RE_USING_SQLITE)
	if (_st)
	{
		REArray<REString *>::Iterator i = _st->columnsNames->getIterator();
		while (i.next())
		{
			if (i.object()->isEqual(columnName))
			{
				return i.index();
			}
		}
	}
#endif /* IS_RE_USING_SQLITE */
	return RENotFound;
}

REBOOL RESQLiteDBResultSet::boolForColumnName(const REString & columnName) const
{
#if (IS_RE_USING_SQLITE)
	return ( this->intForColumnName(columnName) != 0 );
#else
	return false;
#endif
}

REBOOL RESQLiteDBResultSet::boolForColumnIndex(const REUInt32 index) const
{
#if (IS_RE_USING_SQLITE)
	return ( this->intForColumnIndex(index) != 0 );
#else
	return false;
#endif	
}

REInt32 RESQLiteDBResultSet::intForColumnName(const REString & columnName) const
{
#if (IS_RE_USING_SQLITE)
	REUInt32 index = this->columnIndexFromName(columnName);
	return this->intForColumnIndex(index);
#else
	return 0;
#endif	
}

REInt32 RESQLiteDBResultSet::intForColumnIndex(const REUInt32 index) const
{
#if (IS_RE_USING_SQLITE)
	if ( _st ) 
	{
		if ( index < _st->columnsNames->count() ) 
		{
			return sqlite3_column_int((sqlite3_stmt*)_st->statement, (int)index);
		}
	}
#endif	
	return 0;
}

REFloat64 RESQLiteDBResultSet::floatForColumnName(const REString & columnName) const
{
#if (IS_RE_USING_SQLITE)
	REUInt32 index = this->columnIndexFromName(columnName);
	return this->floatForColumnIndex(index);
#else
	return 0.0;
#endif	
}

REFloat64 RESQLiteDBResultSet::floatForColumnIndex(const REUInt32 index) const
{
#if (IS_RE_USING_SQLITE)
	if ( _st ) 
	{
		if ( index < _st->columnsNames->count() ) 
		{
			return sqlite3_column_double((sqlite3_stmt*)_st->statement, (int)index);
		}
	}
#endif	
	return 0.0;
}


REInt64 RESQLiteDBResultSet::longIntForColumnName(const REString & columnName) const
{
#if (IS_RE_USING_SQLITE)
	REUInt32 index = this->columnIndexFromName(columnName);
	return this->longIntForColumnIndex(index);
#else
	return 0;
#endif	
}

REInt64 RESQLiteDBResultSet::longIntForColumnIndex(const REUInt32 index) const
{
#if (IS_RE_USING_SQLITE)
	if ( _st ) 
	{
		if ( index < _st->columnsNames->count() ) 
		{
			return sqlite3_column_int64((sqlite3_stmt*)_st->statement, (int)index);
		}
	}
#endif	
	return 0;
}


REString RESQLiteDBResultSet::stringForColumnName(const REString & columnName) const
{
#if (IS_RE_USING_SQLITE)
	REUInt32 index = this->columnIndexFromName(columnName);
	return this->stringForColumnIndex(index);
#else
	return REString();
#endif	
}

REString RESQLiteDBResultSet::stringForColumnIndex(const REUInt32 index) const
{
#if (IS_RE_USING_SQLITE)
	if ( _st ) 
	{
		if ( index < _st->columnsNames->count() ) 
		{
			if (sqlite3_column_type((sqlite3_stmt*)_st->statement, (int)index) == SQLITE_NULL) 
			{
				return REString();
			}
			
			const char * cStr = (const char *)sqlite3_column_text((sqlite3_stmt*)_st->statement, (int)index);
			if ( cStr ) 
			{
				REString rStr(cStr);
				/*
				if ( rStr.IsContainesWideChars() )
				{
					rStr.ToWideString();
				}
				*/
				return rStr;
			}
		}
	}
#endif	
	return REString();
}

void RESQLiteDBResultSet::close()
{
#if (IS_RE_USING_SQLITE)
	if ( _st ) 
	{
		_st->usingCount--;
		_st = NULL;
	}
#endif	
}

REBOOL RESQLiteDBResultSet::next() const
{
#if (IS_RE_USING_SQLITE)
	if ( _st ) 
	{
		int numberOfRetries = 10;
        int rc = SQLITE_ERROR;
        do 
        {
            sqlite3_stmt * statement = (sqlite3_stmt*)_st->statement;
            rc = sqlite3_step(statement);
            if (rc == SQLITE_BUSY) 
            {
                // this will happen if the db is locked, like if we are doing an update or insert.
                // in that case, retry the step... and maybe wait just 10 milliseconds.
				REThread::uSleep(20);
                numberOfRetries--;
                if (numberOfRetries == 0) { return false; }
            }
            else if ( (rc == SQLITE_ERROR) || (rc == SQLITE_MISUSE) ) 
            {
                return false;
            }
        } 
        while ( (rc != SQLITE_DONE) && (rc != SQLITE_ROW) );
        
        return (rc == SQLITE_ROW);
	}
#endif /* IS_RE_USING_SQLITE */
	return false;
}

REBOOL RESQLiteDBResultSet::isHasAnotherRow() const
{
#if (IS_RE_USING_SQLITE)
	if ( _st ) 
	{
		return (sqlite3_errcode((sqlite3 *)_st->db) == SQLITE_ROW);
	}
#endif	
	return false;
}

REBOOL RESQLiteDBResultSet::isNull() const
{
#if (IS_RE_USING_SQLITE)
	return ( _st == NULL );
#else
	return true;
#endif	
}

RESQLiteDBResultSet & RESQLiteDBResultSet::operator=(const RESQLiteDBResultSet & anotherSet)
{
#if (IS_RE_USING_SQLITE)
	this->close();
	
	if ( anotherSet._st ) 
	{
		_st = anotherSet._st;
		_st->usingCount++;
	}
#endif	
	return (*this);
}

RESQLiteDBResultSet::RESQLiteDBResultSet(const RESQLiteDBResultSet & anotherSet) : REDBResultSet()
#if (IS_RE_USING_SQLITE)
	,_st(anotherSet._st)
#endif
{
#if (IS_RE_USING_SQLITE)
	if ( _st ) 
	{
		_st->usingCount++;
	}
#endif	
}

RESQLiteDBResultSet::RESQLiteDBResultSet(RESQLiteDBResultSet::StatementStruct * statement) : REDBResultSet()
#if (IS_RE_USING_SQLITE)
	,_st(statement)
#endif
{
#if (IS_RE_USING_SQLITE)
	if ( _st ) 
	{
		_st->usingCount++;
	}
#endif	
}

RESQLiteDBResultSet::~RESQLiteDBResultSet()
{
#if (IS_RE_USING_SQLITE)
	this->close();
#endif	
}

