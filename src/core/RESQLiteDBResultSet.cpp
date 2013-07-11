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



#if !defined(__RE_CORE_NO_SQLITE_DATABASE_SUPPORT__)

#include "../../include/recore/REThread.h"

#if defined(HAVE_SQLITE3_H)
#include <sqlite3.h>
#else
#include "../addlibs/sqlite.h"
#endif

#endif


REUInt32 RESQLiteDBResultSet::getColumnIndexFromName(const REString & columnName) const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
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
#endif	
	return RENotFound;
}

REBOOL RESQLiteDBResultSet::getBoolForColumnName(const REString & columnName) const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	return ( this->getIntForColumnName(columnName) != 0 );
#else
	return false;
#endif
}

REBOOL RESQLiteDBResultSet::getBoolForColumnIndex(const REUInt32 index) const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	return ( this->getIntForColumnIndex(index) != 0 );
#else
	return false;
#endif	
}

REInt32 RESQLiteDBResultSet::getIntForColumnName(const REString & columnName) const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	REUInt32 index = this->getColumnIndexFromName(columnName);
	return this->getIntForColumnIndex(index);
#else
	return 0;
#endif	
}

REInt32 RESQLiteDBResultSet::getIntForColumnIndex(const REUInt32 index) const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
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

REFloat64 RESQLiteDBResultSet::getFloatForColumnName(const REString & columnName) const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	REUInt32 index = this->getColumnIndexFromName(columnName);
	return this->getFloatForColumnIndex(index);
#else
	return 0.0;
#endif	
}

REFloat64 RESQLiteDBResultSet::getFloatForColumnIndex(const REUInt32 index) const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
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


REInt64 RESQLiteDBResultSet::getLongIntForColumnName(const REString & columnName) const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	REUInt32 index = this->getColumnIndexFromName(columnName);
	return this->getLongIntForColumnIndex(index);
#else
	return 0;
#endif	
}

REInt64 RESQLiteDBResultSet::getLongIntForColumnIndex(const REUInt32 index) const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
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


REString RESQLiteDBResultSet::getStringForColumnName(const REString & columnName) const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	REUInt32 index = this->getColumnIndexFromName(columnName);
	return this->getStringForColumnIndex(index);
#else
	return REString("");
#endif	
}

REString RESQLiteDBResultSet::getStringForColumnIndex(const REUInt32 index) const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
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
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	if ( _st ) 
	{
		_st->usingCount--;
		_st = NULL;
	}
#endif	
}

REBOOL RESQLiteDBResultSet::next() const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
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
#endif	
	return false;
}

REBOOL RESQLiteDBResultSet::isHasAnotherRow() const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	if ( _st ) 
	{
		return (sqlite3_errcode((sqlite3 *)_st->db) == SQLITE_ROW);
	}
#endif	
	return false;
}

REBOOL RESQLiteDBResultSet::isNull() const
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	return ( _st == NULL );
#else
	return true;
#endif	
}

RESQLiteDBResultSet & RESQLiteDBResultSet::operator=(const RESQLiteDBResultSet & anotherSet)
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
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
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__
	,_st(anotherSet._st)
#endif
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	if ( _st ) 
	{
		_st->usingCount++;
	}
#endif	
}

RESQLiteDBResultSet::RESQLiteDBResultSet(RESQLiteDBResultSet::StatementStruct * statement) : REDBResultSet()
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__
	,_st(statement)
#endif
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	if ( _st ) 
	{
		_st->usingCount++;
	}
#endif	
}

RESQLiteDBResultSet::~RESQLiteDBResultSet()
{
#ifndef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__	
	this->close();
#endif	
}

