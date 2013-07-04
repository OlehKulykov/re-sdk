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


#include "../../../include/recore/REDB.h"

REString REDB::getDatabaseFullPath() const
{
	return REString();
}

REBOOL REDB::open()
{
	return false;
}

REBOOL REDB::isOpened() const
{
	return false;
}

REBOOL REDB::close()
{
	return false;
}

REInt64 REDB::getLastInsertedRowId() const
{
	return -1;
}

REPtr<REDBResultSet> REDB::executeQuery(const REString & queryString)
{
	return REPtr<REDBResultSet>(new REDBResultSet());
}

REBOOL REDB::beginTransaction()
{
	return false;
}

REBOOL REDB::executeUpdate(const REString & queryString)
{
	return false;
}

REBOOL REDB::commitTransaction()
{
	return false;
}

REBOOL REDB::rollBackTransaction()
{
	return false;
}

REDB::REDB()
{
	
}

REPtr<REDB> REDB::createSQLiteWithFilePath(const REString & dbFilePath)
{
	return REPtr<REDB>();
}

