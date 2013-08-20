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


#ifndef __REFILE_H__
#define __REFILE_H__

#include "RECommonHeader.h"
#include "IREFileReadable.h"
#include "REString.h"

/// Class of file object.
class __RE_PUBLIC_CLASS_API__ REFile : public IREFileReadable
{
protected:
	FILE * _file;	
	REUInt32 _fileSize;
public:
	/* IREFileReadable */
	virtual REUInt32 fileRead(void * bufferForReading, const REUInt32 dataSizeToRead);
	virtual REUInt32 fileTell();
	virtual REInt32 fileSeek(const REUInt32 fileOffset, int origin);
	virtual REInt32 fileSeekFromEndFile(const REUInt32 fileOffset);
	virtual REInt32 fileSeekFromBeginFile(const REUInt32 fileOffset);
	virtual REInt32 fileSeekFromCurrentFilePos(const REUInt32 fileOffset);
	virtual REInt32 fileFError();
	virtual REInt32 fileClose();
	virtual REBOOL isEndOfFile();
	
	/// Returns size of file.
	const REUInt32 fileSize() const;
	
	/// Construct file object with file path and open mode.
	REFile(const REString & filePath, const char * openMode);
	
	/// Construct file object with file name.
	REFile(const REString & filePath);
	
	/// Destructor.
	virtual ~REFile();
	
	/// Opens file with path and opne mode.
	static FILE * fileOpen(const REString & filePath, const char * openMode);
};


#endif /* __REFILE_H__ */

