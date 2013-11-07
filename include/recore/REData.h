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


#ifndef __REDATA_H__
#define __REDATA_H__

#include "RECommonHeader.h"


#include "REString.h"
#include "REArray.h"
#include "IREFileReadable.h"
#include "REPtr.h"

/// Class of data based on buffer with file read functionality.
class __RE_PUBLIC_CLASS_API__ REData : public IREFileReadable
{
protected:
	REPtr<REBuffer> _buffer;
	REUInt32 _fileOffset;
	
	REBOOL initializeFromFilePath(const char * filePath, const REUInt32 filePathLength);
	REBOOL initializeFromResourcePath(const char * filePath, const REUInt32 filePathLength);
	REBOOL initFromPath(const char * filePath, const REUInt32 filePathLength);
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
	
	/// Initializes data from file path.
	/// First try to init data from avaiable resources
	/// and than try to init directly from file.
	REBOOL initFromPath(const char * filePath);
	
	/// Initializes data from file path.
	/// First try to init data from avaiable resources
	/// and than try to init directly from file.
	REBOOL initFromPath(const REString & filePath);
	
	/// Initializes data from buffer.
	REBOOL initFromBuffer(const REBuffer & buffer);
	
	/// Basic assignment operator.
	REData & operator=(const REData & anotherData);
	
	/// Basic assignment operator.
	REData & operator=(const REBuffer & anotherBuffer);
	
	/// Basic assignment operator.
	REData & operator=(const REPtr<REBuffer> & anotherBufferPtr);
	
	/// This functionality avaiable only with RENetwork and
	/// in config file must be defined __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__ 
	/// Initializing data object from URL String.
	/// Example: http://example.com/index.html for downloading from web.
	/// Example: file:///Volumes/Data/file.dat for reading from from file. See file url scemes.
	REBOOL initFromURLString(const REString & urlString);
	
	void clear();
	
	/// Returns pointer to data memory buffer.
	const REUByte * bytes() const;
	
	REUInt32 size() const;
	
	/// Returns hash of data.
	/// Using CRC32 hash generator.
	REUInt32 hash() const;
	
	/// Checks is data empty.
	REBOOL isEmpty() const;
	
	/// Constructs empty data.
	REData();
	
	/// Constructs empty data.
	/// 'buffer' will be retained and deleted after using.
	/// Do not delete 'buffer' manually.
	REData(REBuffer * buffer);
	
	/// Constructs empty data.
	/// 'buffer' pointer will be retained and released after using.
	REData(const REPtr<REBuffer> & buffer);
	
	/// Construct data from another data.
	/// buffer not copyed, retained.
	REData(const REData & anotherData);
	
	/// Constructs data from buffer.
	/// buffer will be copyed.
	REData(const REBuffer & buffer);
	
	/// Destructor.
	virtual ~REData();
	
#if (defined(__RE_OS_IPHONE__) || defined(__RE_OS_MACOSX__)) && (defined(FOUNDATION_STATIC_INLINE) || defined(FOUNDATION_EXTERN_INLINE))
	NSData * getNSData() const
	{
		return this->isEmpty() ? nil : [NSData dataWithBytes:this->bytes() length:this->size()];
	}
	
	void setNSData(NSData * data)
	{
		if (data) 
		{
			const void * bytes = [data bytes];
			const NSUInteger len = [data length];
			if (bytes && len > 0)
			{
				_buffer = REPtr<REBuffer>(new REBuffer(bytes, (REUInt32)len));
			}
		}
	}
#endif	
	
};

#endif /* __REDATA_H__ */


