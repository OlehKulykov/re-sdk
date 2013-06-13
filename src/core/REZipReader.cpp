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


#include "../../include/recore/REZipReader.h"
#include "../../include/recore/REMD5Generator.h"
#include "../../include/recore/REFileManager.h"
#include "../../include/recore/REFile.h"
#include "../../include/recore/REMem.h"

#include "unzip/crypt.h"
#include "unzip/unzip.h"

class REZipReaderPrivateCallBacks 
{
public:
	static voidpf Open(voidpf opaque, const char * filename, int mode);
	static uLong Read(voidpf opaque, voidpf stream, void * buf, uLong size);
	static uLong Write(voidpf opaque, voidpf stream, const void * buf, uLong size);
	static long Tell(voidpf opaque, voidpf stream);
	static long Seek(voidpf opaque, voidpf stream, uLong offset, int origin);
	static int Close(voidpf opaque, voidpf stream);
	static int Error(voidpf opaque, voidpf stream);
};

voidpf REZipReaderPrivateCallBacks::Open(voidpf opaque, const char * filename, int mode)
{
	// opened file on create callback
	return opaque;
}

uLong REZipReaderPrivateCallBacks::Read(voidpf opaque, voidpf stream, void * buf, uLong size)
{
	return (uLong)((REFile *)opaque)->FileRead(buf, (const REUInt32)size);
}

uLong REZipReaderPrivateCallBacks::Write(voidpf opaque, voidpf stream, const void * buf, uLong size)
{
	// no writing
	return 0;
}

long REZipReaderPrivateCallBacks::Tell(voidpf opaque, voidpf stream)
{
	return (long)((REFile *)opaque)->FileTell();
}

long REZipReaderPrivateCallBacks::Seek(voidpf opaque, voidpf stream, uLong offset, int origin)
{
	switch (origin)
    {
		case ZLIB_FILEFUNC_SEEK_CUR :
			return (long)((REFile *)opaque)->FileSeek((const REUInt32)offset, SEEK_CUR);
			break;
		case ZLIB_FILEFUNC_SEEK_END :
			return (long)((REFile *)opaque)->FileSeek((const REUInt32)offset, SEEK_END);
			break;
		case ZLIB_FILEFUNC_SEEK_SET :
			return (long)((REFile *)opaque)->FileSeek((const REUInt32)offset, SEEK_SET);
			break;
		default: return -1;
    }
	return -1;
}

int REZipReaderPrivateCallBacks::Close(voidpf opaque, voidpf stream)
{
	return (int)((REFile *)opaque)->FileClose();
}

int REZipReaderPrivateCallBacks::Error(voidpf opaque, voidpf stream)
{
	return (int)((REFile *)opaque)->FileFError();
}

class __RE_PUBLIC_CLASS_API__ REEditableZipEntry : public REZipEntry
{
protected:
	unzFile _unZipFile;
	REUInt32 _unzipFileOffset;
public:
	void SetCRC32(const REUInt32 newCRC32) { _crc32 = newCRC32; }
	void SetSize(const REUInt32 newSize) { _size = newSize; }
	void SetPath(const REString & newPath) { _path.Set(newPath); }
	void SetPath(const char * newPath) { _path.Set(newPath); }
	void SetUnZipFileOffset(const REUInt32 newOffset) { _unzipFileOffset = newOffset; }
	const REUInt32 GetUnZipFileOffset() const { return _unzipFileOffset; }
	
	virtual REBOOL Read(REBuffer * buff) const;
	
	REEditableZipEntry(unzFile unZipFile) : REZipEntry() ,
		_unZipFile(unZipFile),
		_unzipFileOffset(0) { }
	
	virtual ~REEditableZipEntry() { }
};


/// Reads to entries array all avaiable arcrchive entries.
/// Note: all archive entries will be deleted with archive. 
REBOOL REZipReader::ReadAvaiableEntries(REArray<REZipEntry*> * entries) const
{
	if (entries && _entries)  
	{
		entries->Clear();
		REArray<REEditableZipEntry*> * from = (REArray<REEditableZipEntry*> *)_entries;
		if (entries->SetCapacity(from->Count() + 1)) 
		{
			for (REUInt32 i = 0; i < from->Count(); i++) 
			{
				REEditableZipEntry * e = (*from)[i];
				entries->Add(e);
			}
			return true;
		}
	}
	return false;
}

REZipEntry * REZipReader::GetEntry(const REString & entryPath) const
{
	if (entryPath.IsEmpty())
	{
		return NULL;
	}
	
	if (_entries)
	{
		REArray<REEditableZipEntry*> * entries = (REArray<REEditableZipEntry*> *)_entries;
		for (REUInt32 i = 0; i < entries->Count(); i++)
		{
			if ( (*entries)[i]->GetPath().IsEqual(entryPath) ) 
			{
				return (*entries)[i];
			}
		}
	}
	
	return NULL;
}

void REZipReader::InitEntries()
{
	unzFile unZipFile = (unzFile)_archiveFile;
	int result = unzGoToFirstFile(unZipFile);
	if (result != UNZ_OK) { return; }
	const uLong extraFieldBufferSize = 1025;
	char extraField[extraFieldBufferSize];
	const uLong commentBufferSize = 1025;
	char szComment[commentBufferSize];
	REArray<REEditableZipEntry*> * entries = (REArray<REEditableZipEntry*> *)_entries;
	bool isCanReadNext = true;
	while (isCanReadNext) 
	{
		int openedForReadingFile = unzOpenCurrentFile(unZipFile);
		if (openedForReadingFile != UNZ_OK) { return; }
		
		const uLong fileNameBufferSize = 1025;
		char szFileName[fileNameBufferSize] = { 0 };
		
		unz_file_info pfile_info;
		memset(&pfile_info, 0, sizeof(pfile_info));
		
		const REUInt32 offsetInFile = (REUInt32)unzGetOffset(unZipFile);
		unzGetCurrentFileInfo(unZipFile,
							  &pfile_info,
							  &szFileName[0],
							  fileNameBufferSize,
							  extraField,
							  extraFieldBufferSize,
							  &szComment[0],
							  commentBufferSize);
		unzCloseCurrentFile(unZipFile);
		
		if (pfile_info.uncompressed_size > 0)
		{
			REEditableZipEntry * newEntry = new REEditableZipEntry(unZipFile);
			if (newEntry)
			{
				if (entries->Add(newEntry))
				{
					newEntry->SetCRC32((REUInt32)pfile_info.crc);
					newEntry->SetPath(szFileName);
					newEntry->SetSize((REUInt32)pfile_info.uncompressed_size);
					newEntry->SetUnZipFileOffset(offsetInFile);
				}
				else
				{
					delete newEntry;
				}
			}
		} 
		isCanReadNext = (unzGoToNextFile(unZipFile) == UNZ_OK);
	}
}

const REUInt32 REZipReader::GetEntriesCount() const
{
	if (_entries)
	{
		REArray<REEditableZipEntry*> * entries = (REArray<REEditableZipEntry*> *)_entries;
		return entries->Count();
	}
	return 0;
}

REBOOL REZipReader::IsEmpty() const
{
	return (this->GetEntriesCount() == 0);
}

REBOOL REZipReader::OpenArchiveFile()
{
	zlib_filefunc_def * callBacks = (zlib_filefunc_def *)REMem::Malloc(sizeof(zlib_filefunc_def));
	if (callBacks)
	{
		callBacks->opaque = _source;
		callBacks->zopen_file = REZipReaderPrivateCallBacks::Open;
		callBacks->zread_file = REZipReaderPrivateCallBacks::Read;
		callBacks->zwrite_file = REZipReaderPrivateCallBacks::Write;
		callBacks->ztell_file = REZipReaderPrivateCallBacks::Tell;
		callBacks->zseek_file = REZipReaderPrivateCallBacks::Seek;
		callBacks->zclose_file = REZipReaderPrivateCallBacks::Close;
		callBacks->zerror_file = REZipReaderPrivateCallBacks::Error;
		
		unzFile unZipFile = unzOpen2("", callBacks);
		if (unZipFile)
		{
			_callBacks = callBacks;
			_archiveFile = unZipFile;
			return true;
		}
		
		REMem::Free(callBacks);
	}

	return false;
}

REBOOL REZipReader::OpenPath(const REString & zipFilePath)
{
	REFileManager manager;
	if (manager.IsFileExistsAtPath(zipFilePath))
	{
		REFile * zipFile = new REFile(zipFilePath);
		if (zipFile)
		{
			if (zipFile->GetFileSize())
			{
				_source = zipFile;
				if (this->OpenArchiveFile()) 
				{
					unz_global_info globalInfo;
					globalInfo.number_entry = 0;
					globalInfo.size_comment = 0;
					unzFile unZipFile = (unzFile)_archiveFile;
					unzGetGlobalInfo(unZipFile, &globalInfo);
					if ( globalInfo.number_entry > 0 )
					{
						REArray<REEditableZipEntry*> * entries = new REArray<REEditableZipEntry *>((REUInt32)globalInfo.number_entry);
						if (entries)
						{
							_entries = entries;
							this->InitEntries();
							return true;
						}
						delete entries;
					}
				}
				_source = NULL;
			}
			delete zipFile;
		}
	}
	return false;
}

REBOOL REZipReader::OpenData(const REData & zipFileData)
{
	REData * zipData = new REData(zipFileData);
	if (zipData) 
	{
		if (zipData->GetSize()) 
		{
			_source = zipData;
			if (this->OpenArchiveFile()) 
			{
				unz_global_info globalInfo;
				globalInfo.number_entry = 0;
				globalInfo.size_comment = 0;
				unzFile unZipFile = (unzFile)_archiveFile;
				unzGetGlobalInfo(unZipFile, &globalInfo);
				if ( globalInfo.number_entry > 0 )
				{
					REArray<REEditableZipEntry*> * entries = new REArray<REEditableZipEntry *>((REUInt32)globalInfo.number_entry);
					if (entries)
					{
						_entries = entries;
						this->InitEntries();
						return true;
					}
					delete entries;
				}
			}
			_source = NULL;	
		}
		
		delete zipData;
	}
	return false;
}

/// Initialized zip reader with zip file data 
REZipReader::REZipReader(const REData & zipFileData) :
	_archiveFile(NULL),
	_entries(NULL),
	_callBacks(NULL),
	_source(NULL)
{
	this->OpenData(zipFileData);
}

REZipReader::REZipReader(const REString & zipFilePath) : 
	_archiveFile(NULL),
	_entries(NULL),
	_callBacks(NULL),
	_source(NULL)
{
	this->OpenPath(zipFilePath);
}

REZipReader::~REZipReader()
{
	if (_archiveFile)
	{
		unzFile unZipFile = (unzFile)_archiveFile;
		unzClose(unZipFile);
	}
	
	if (_callBacks)
	{
		zlib_filefunc_def * callBacks = (zlib_filefunc_def *)_callBacks;
		REMem::Free(callBacks);
	}

	if (_source) 
	{
		delete _source;
	}
	
	if (_entries)
	{
		REArray<REEditableZipEntry*> * entries = (REArray<REEditableZipEntry*> *)_entries;
		for (REUInt32 i = 0; i < entries->Count(); i++) 
		{
			REEditableZipEntry * en = (*entries)[i];
			delete en;
		}
		entries->Clear();
		delete entries;
	}
	
}



const REString & REZipEntry::GetPath() const
{
	return _path;
}

const REUInt32 REZipEntry::GetSize() const
{
	return _size;
}

const REUInt32 REZipEntry::GetCRC32() const
{
	return _crc32;
}

REZipEntry::REZipEntry() : 
	_size(0),
	_crc32(0)
{
	
}

REZipEntry::~REZipEntry()
{
	
}

REBOOL REEditableZipEntry::Read(REBuffer * buff) const
{
	if (buff && _unzipFileOffset && _unZipFile)
	{
		if (unzSetOffset(_unZipFile, (uLong)_unzipFileOffset) != UNZ_OK) { return false; }
		if (unzOpenCurrentFile(_unZipFile) != UNZ_OK) { return false; }
		REBOOL isReaded = false;
		if (buff->Resize(_size, false))
		{
			const REUInt32 readedSize = (REUInt32)unzReadCurrentFile(_unZipFile, buff->GetBuffer(), buff->GetSize());
			isReaded = (readedSize == _size);
		}
		unzCloseCurrentFile(_unZipFile);
		return isReaded;
	}
	return false;
}


