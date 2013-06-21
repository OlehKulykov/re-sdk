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


#ifndef __REZIPREADER_H__
#define __REZIPREADER_H__

#include "RECommonHeader.h"
#include "REArray.h"
#include "REBuffer.h"
#include "IREFileReadable.h"
#include "REData.h"

/// Entry of zip file. Using with REZipReader.
class __RE_PUBLIC_CLASS_API__ REZipEntry
{	
protected:
	REString _path;
	REUInt32 _size;
	REUInt32 _crc32;
	
public:
	/// Return path as presented in zip file
	const REString & getPath() const;
	
	/// Return size in bytes of uncompressed entry
	const REUInt32 getSize() const;
	
	/// Return crc32 value of entry
	const REUInt32 getCRC32() const;
	
	/// Reads uncompressed bytes of entry to buffer
	virtual REBOOL read(REBuffer * buff) const = 0;
	
	
	REZipEntry();
	virtual ~REZipEntry();
};


/// Reads zip archivers in 32 bit mode up to 4GB.
/// Note: all archive entries will be deleted with archive.
class __RE_PUBLIC_CLASS_API__ REZipReader 
{
private:
	void * _archiveFile;
	void * _entries;
	void * _callBacks;
	IREFileReadable * _source;
	
	void initEntries();
	REBOOL openArchiveFile();
	REBOOL openPath(const REString & zipFilePath);
	REBOOL openData(const REData & zipFileData);
	
public:
	/// Reads to entries array all avaiable arcrchive entries.
	/// Note: all archive entries will be deleted with archive. 
	REBOOL readAvaiableEntries(REArray<REZipEntry*> * entries) const;
	
	/// Return entry for path of NULL if not found.
	REZipEntry * getEntry(const REString & entryPath) const;
	
	/// Return count of zip entries
	const REUInt32 getEntriesCount() const;
	
	/// Check is archive have no entries e.g. is empty of file not found
	REBOOL isEmpty() const;
	
	/// Initialized zip reader with zip file data 
	REZipReader(const REData & zipFileData);
	
	/// Initialized zip reader with path to zip file
	REZipReader(const REString & zipFilePath);
	virtual ~REZipReader();
};


#endif /* __REZIPREADER_H__ */

