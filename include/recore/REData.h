/*
 *   Copyright 2012 Kulykov Oleh
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


#ifndef __REDATA_H__
#define __REDATA_H__

#include "RECommonHeader.h"


#include "REString.h"
#include "REArray.h"
#include "IREFileReadable.h"
#include "REBufferObject.h"

/// Class of data based on buffer with file read functionality.
class __RE_PUBLIC_CLASS_API__ REData : public REBuffer, public IREFileReadable
{
protected:
	REUInt32 _fileOffset;
	
	REBOOL InitializeFromFilePath(const char * filePath, const REUInt32 filePathLength);
	REBOOL InitializeFromResourcePath(const char * filePath, const REUInt32 filePathLength);
	REBOOL InitFromPath(const char * filePath, const REUInt32 filePathLength);
public:
	/// Constructs empty data.
	REData();
	
	/// Construct data from another data.
	REData(const REData & anotherData);
	
	/// Constructs data from buffer.
	REData(const REBuffer & buffer);
	
	/// Destructor.
	virtual ~REData();
	
	/* IREFileReadable */
	virtual REUInt32 FileRead(void * bufferForReading, const REUInt32 dataSizeToRead);
	virtual REUInt32 FileTell();
	virtual REInt32 FileSeek(const REUInt32 fileOffset, int origin);
	virtual REInt32 FileSeekFromEndFile(const REUInt32 fileOffset);
	virtual REInt32 FileSeekFromBeginFile(const REUInt32 fileOffset);
	virtual REInt32 FileSeekFromCurrentFilePos(const REUInt32 fileOffset);
	virtual REInt32 FileFError();
	virtual REInt32 FileClose();
	virtual REBOOL IsEndOfFile();
	
	/// Initializes data from file path.
	/// First wtry to init data from avaiable resources
	/// and than try to init directly from file.
	REBOOL InitFromPath(const char * filePath);
	
	/// Initializes data from file path.
	/// First wtry to init data from avaiable resources
	/// and than try to init directly from file.
	REBOOL InitFromPath(const REString & filePath);
	
	/// Initializes data from buffer.
	REBOOL InitFromBuffer(const REBuffer & buffer);
	
	/// Basic assignment operator.
	REData & operator=(const REData & anotherData);
	
	/// This functionality avaiable only with RENetwork and
	/// in config file must be defined __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__ 
	/// Initializing data object from URL String.
	/// Example: http://example.com/index.html for downloading from web.
	/// Example: file:///Volumes/Data/file.dat for reading from from file. See file url scemes.
	REBOOL InitFromURLString(const REString & urlString);
	
	/// Returns pointer to data memory buffer.
	const REUByte * GetBytes() const;
	
	/// Returns hash of data.
	REUInt32 GetHash() const;
	
	/// Checks is data empty.
	REBOOL IsEmpty() const;
};

#endif /* __REDATA_H__ */


