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


#include "../../include/recore/REData.h"
#include "../../include/recore/REMD5Generator.h"
#include "../../include/recore/RECRC32Generator.h"
#include "../../include/recore/REResourcesStorage.h"
#include "../../include/recore/REFile.h"
#include "../../include/recore/REMem.h"
#include "../../include/recore/REStaticString.h"


#ifdef __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
#include "../../include/renetwork.h"
#endif

REUInt32 REData::fileRead(void * bufferForReading, const REUInt32 dataSizeToRead)
{
	if (bufferForReading && dataSizeToRead)
	{
		REPtr<REBuffer> b(_buffer);
		if (b.isNotEmpty()) 
		{
			REUByte * buff = (REUByte *)b->buffer();
			if (buff) 
			{
				REUInt32 sizeToRead = dataSizeToRead;
				REUInt32 avaiableSize = this->size() - _fileOffset;
				if (avaiableSize < sizeToRead)
				{
					sizeToRead = avaiableSize;
				}
				
				memcpy(bufferForReading, &buff[_fileOffset], sizeToRead);
				_fileOffset += sizeToRead;
				return sizeToRead;
			}
		}
	}
	
	return 0;
}

REUInt32 REData::fileTell()
{
	return _fileOffset;
}

REInt32 REData::fileSeek(const REUInt32 fileOffset, int origin)
{
	REUInt32 resultOffset = fileOffset;
	if (origin == SEEK_CUR)
	{
		resultOffset += _fileOffset;
	}
	else if (origin == SEEK_END)
	{
		if (this->size() < fileOffset)
		{
			return 2; // out of bounds
		}
		resultOffset = (this->size() - fileOffset);
	}

	if (resultOffset > this->size()) // common for SEEK_CUR && SEEK_SET
	{
		return 3; // out of bounds
	}

	_fileOffset = resultOffset;
	return 0;
}

REInt32 REData::fileSeekFromEndFile(const REUInt32 fileOffset)
{
	return this->fileSeek(fileOffset, SEEK_END);
}

REInt32 REData::fileSeekFromBeginFile(const REUInt32 fileOffset)
{
	return this->fileSeek(fileOffset, SEEK_SET);
}

REInt32 REData::fileSeekFromCurrentFilePos(const REUInt32 fileOffset)
{
	return this->fileSeek(fileOffset, SEEK_CUR);
}

REInt32 REData::fileFError()
{
	return 0;
}

REInt32 REData::fileClose()
{
	return 0;	
}

REBOOL REData::isEndOfFile()
{
	return (this->size() == _fileOffset);
}

void REData::clear()
{
	_buffer.release();
	_fileOffset = 0;
}

REData & REData::operator=(const REData & anotherData)
{
	_buffer = anotherData._buffer;
	_fileOffset = 0;
	
	return (*this);
}

/// Basic assignment operator.
REData & REData::operator=(const REBuffer & anotherBuffer)
{
	_buffer.release();
	_fileOffset = 0;
	
	const REUInt32 s = anotherBuffer.size();
	if (s > 0) 
	{
		REPtr<REBuffer> b(new REBuffer(anotherBuffer.buffer(), s));
		if (b.isNotEmpty()) 
		{
			if (b->size() == s) 
			{
				_buffer = b;
			}
		}
	}
	
	return (*this);
}

/// Basic assignment operator.
REData & REData::operator=(const REPtr<REBuffer> & anotherBufferPtr)
{
	_buffer = anotherBufferPtr;
	_fileOffset = 0;
	return (*this);
}

const REUByte * REData::bytes() const
{
	return _buffer.isNotEmpty() ? (const REUByte *)_buffer->buffer() : NULL;
}

REUInt32 REData::size() const
{
	return _buffer.isNotEmpty() ? _buffer->size() : 0;
}

REUInt32 REData::hash() const
{
	return RECRC32Generator::generateFromData(this->bytes(), this->size());
}

REBOOL REData::isEmpty() const 
{
	return (this->size() == 0);
}

/// Constructs data from buffer.
/// buffer will be copyed.
REData::REData(const REBuffer & buffer) : 
	_fileOffset(0)
{
	const REUInt32 s = buffer.size();
	if (s > 0) 
	{
		REPtr<REBuffer> b(new REBuffer(buffer.buffer(), s));
		if (b.isNotEmpty()) 
		{
			if (b->size() == s) 
			{
				_buffer = b;
			}
		}
	}
}

/// Construct data from another data.
/// buffer not copyed, retained.
REData::REData(const REData & anotherData) : 
	_buffer(anotherData._buffer),
	_fileOffset(0)
{
	
}

/// Constructs empty data.
/// 'buffer' will be retained and deleted after using.
REData::REData(REBuffer * buffer) :
	_fileOffset(0)
{
	if (buffer) 
	{
		_buffer = REPtr<REBuffer>(buffer);
	}
}

/// Constructs empty data.
/// 'buffer' pointer will be retained and released after using.
REData::REData(const REPtr<REBuffer> & buffer) :
	_buffer(buffer),
	_fileOffset(0)
{
	
}

REData::REData() : 
	_fileOffset(0)
{

}

REData::~REData()
{
	
}

REBOOL REData::initializeFromFilePath(const char * filePath, const REUInt32 filePathLength)
{
	this->clear();
	
	FILE * fileHandle = REFile::fileOpen(filePath, "rb");
	if (fileHandle == NULL) 
	{
		return false;
	}
	
	if (fseek(fileHandle, 0, SEEK_END) != 0)
	{
		fclose(fileHandle);
		return false;
	}
	
	long fileSize = ftell(fileHandle);
	if (fileSize <= 0) 
	{
		fclose(fileHandle);
		return false;
	}
	
	if (fseek(fileHandle, 0, SEEK_SET) != 0)
	{
		fclose(fileHandle);
		return false;
	}
	
	REPtr<REBuffer> b(new REBuffer((REUInt32)fileSize));
	if (b.isNotEmpty()) 
	{
		if (b->size() == fileSize) 
		{
			const size_t readedSize = fread(b->buffer(), 1, b->size(), fileHandle);
			if (readedSize == b->size())
			{
				fclose(fileHandle);
				_buffer = b;
				return true;
			}
		}
	}
	
	fclose(fileHandle);
	return false;
}

REBOOL REData::initializeFromResourcePath(const char * filePath, const REUInt32 filePathLength)
{
	this->clear();
	
	REBuffer * newBuffer = new REBuffer();
	REPtr<REBuffer> b(newBuffer);
	if (b.isNotEmpty()) 
	{
		REResourcesStorage r;
		if (r.readToBuffer(REStaticString(filePath, filePathLength), newBuffer))
		{
			_buffer = b;
			return true;
		}
	}
	
	return false;
}

REBOOL REData::initFromPath(const char * filePath, const REUInt32 filePathLength)
{
	this->clear();
	
	if ( this->initializeFromResourcePath(filePath, filePathLength) ) { return true; }
	else if ( this->initializeFromFilePath(filePath, filePathLength) ) { return true; }
	return false;
}

REBOOL REData::initFromPath( const REString & filePath )
{
	this->clear();
	
	if (filePath.isEmpty()) 
	{
		return false;
	}
	
	const char * s = filePath.UTF8String();
	const REUInt32 l = filePath.length();
	return this->initFromPath(s, l);
}

REBOOL REData::initFromPath( const char * filePath )
{
	this->clear();	
	
	if (filePath) 
	{
		const REUInt32 filePathLength = (REUInt32)strlen(filePath);
		return this->initFromPath(filePath, filePathLength);
	}
	return false;
}

REBOOL REData::initFromBuffer(const REBuffer & buffer)
{
	this->clear();
	
	(*this) = buffer;

	return true;
}

/// __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
REBOOL REData::initFromURLString(const REString & urlString)
{
	this->clear();
	
#ifdef __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
	REURL url(urlString);
	if (url.IsFileURL()) 
	{
		return this->InitFromPath(url.GetHierarchicalPart());
	}
	else
	{
		REURLRequestObject * request = REURLRequestObject::CreateWithURL(url);
		if (request) 
		{
			REBuffer downlBuff;
			const REBOOL isSended = REURLConnectionObject::SendRequest(request, &downlBuff, NULL);
			request->release();
			if (isSended)
			{
				if (downlBuff.GetSize()) 
				{
					return this->InitFromBuffer(downlBuff);
				}
				return true;
			}
		}
	}
#endif	
	return false;
}




