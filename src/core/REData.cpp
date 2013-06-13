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
#include "../../include/recore/REResourcesStorage.h"
#include "../../include/recore/REFile.h"
#include "../../include/recore/REMem.h"

#ifdef __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
#include "../../include/renetwork.h"
#endif

REUInt32 REData::FileRead(void * bufferForReading, const REUInt32 dataSizeToRead)
{
	if (bufferForReading && dataSizeToRead )
	{
		REUByte * buff = (REUByte *)this->GetBuffer();
		if (buff) 
		{
			REUInt32 sizeToRead = dataSizeToRead;
			REUInt32 avaiableSize = this->GetSize() - _fileOffset;
			if (avaiableSize < sizeToRead)
			{
				sizeToRead = avaiableSize;
			}
			
			REMem::Memcpy(bufferForReading, &buff[_fileOffset], sizeToRead);
			_fileOffset += sizeToRead;
			return sizeToRead;
		}
	}
	
	return 0;
}

REUInt32 REData::FileTell()
{
	return _fileOffset;
}

REInt32 REData::FileSeek(const REUInt32 fileOffset, int origin)
{
	REUInt32 resultOffset = fileOffset;
	if (origin == SEEK_CUR)
	{
		resultOffset += _fileOffset;
	}
	else if (origin == SEEK_END)
	{
		if (this->GetSize() < fileOffset)
		{
			return 2; // out of bounds
		}
		resultOffset = (this->GetSize() - fileOffset);
	}

	if (resultOffset > this->GetSize()) // common for SEEK_CUR && SEEK_SET
	{
		return 3; // out of bounds
	}

	_fileOffset = resultOffset;
	return 0;
}

REInt32 REData::FileSeekFromEndFile(const REUInt32 fileOffset)
{
	return this->FileSeek(fileOffset, SEEK_END);
}

REInt32 REData::FileSeekFromBeginFile(const REUInt32 fileOffset)
{
	return this->FileSeek(fileOffset, SEEK_SET);
}

REInt32 REData::FileSeekFromCurrentFilePos(const REUInt32 fileOffset)
{
	return this->FileSeek(fileOffset, SEEK_CUR);
}

REInt32 REData::FileFError()
{
	return 0;
}

REInt32 REData::FileClose()
{
	return 0;	
}

REBOOL REData::IsEndOfFile()
{
	return (this->GetSize() == _fileOffset);
}

REData & REData::operator=(const REData & anotherData)
{
	this->Clear();
	
	this->Set(anotherData.GetBuffer(), anotherData.GetSize());
		
	return (*this);
}

const REUByte * REData::GetBytes() const
{
	return (const REUByte *)this->GetBuffer();
}

REUInt32 REData::GetHash() const
{
	REMD5Generator g;
	return g.GetFromData(this->GetBuffer(), this->GetSize());
}

REBOOL REData::IsEmpty() const 
{
	return (this->GetSize() == 0);
}

REData::REData(const REBuffer & buffer) : REBuffer(buffer),
	_fileOffset(0)
{
	
}

REData::REData(const REData & anotherData) : REBuffer(anotherData),
	_fileOffset(0)
{
	
}

REData::REData() : REBuffer(),
	_fileOffset(0)
{

}

REData::~REData()
{
	
}

REBOOL REData::InitializeFromFilePath(const char * filePath, const REUInt32 filePathLength)
{
	this->Clear();
	
	FILE * fileHandle = REFile::FOpen(REString(filePath), "rb");
	if ( fileHandle == NULL ) 
	{
		return false;
	}
	
	if ( fseek(fileHandle, 0, SEEK_END) != 0 )
	{
		fclose(fileHandle);
		return false;
	}
	
	long fileSize = ftell(fileHandle);
	if ( fileSize <= 0 ) 
	{
		fclose(fileHandle);
		return false;
	}
	
	if ( fseek(fileHandle, 0, SEEK_SET) != 0 )
	{
		fclose(fileHandle);
		return false;
	}
	
	if (this->Resize((REUInt32)fileSize, false))
	{
		size_t readedSize = fread(this->GetBuffer(), 1, this->GetSize(), fileHandle);
		if (readedSize == this->GetSize())
		{
			fclose(fileHandle);
			return true;
		}
		else
		{
			this->Clear();
		}
	}
	
	fclose(fileHandle);
	return false;
}

REBOOL REData::InitializeFromResourcePath(const char * filePath, const REUInt32 filePathLength)
{
	this->Clear();
	
	REResourcesStorage r;
	return r.ReadToBuffer(REString(filePath), this);
}

REBOOL REData::InitFromPath(const char * filePath, const REUInt32 filePathLength)
{
	this->Clear();
	
	if ( this->InitializeFromResourcePath(filePath, filePathLength) ) { return true; }
	else if ( this->InitializeFromFilePath(filePath, filePathLength) ) { return true; }
	return false;
}

REBOOL REData::InitFromPath( const REString & filePath )
{
	this->Clear();
	
	if ( filePath.IsEmpty() ) 
	{
		return false;
	}
	
	const char * s = filePath.UTF8String();
	const REUInt32 l = filePath.Length();
	return this->InitFromPath(s, l);
}

REBOOL REData::InitFromPath( const char * filePath )
{
	this->Clear();	
	
	if (filePath) 
	{
		const REUInt32 filePathLength = (REUInt32)strlen(filePath);
		return this->InitFromPath(filePath, filePathLength);
	}
	return false;
}

REBOOL REData::InitFromBuffer(const REBuffer & buffer)
{
	this->Clear();
	
	(*this) = buffer;

	return true;
}

/// __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
REBOOL REData::InitFromURLString(const REString & urlString)
{
	this->Clear();
	
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
			request->Release();
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




