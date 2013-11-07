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


#include "../../include/recore/REFile.h"
#include "../../include/recore/REWideString.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

#if defined(HAVE_SYS_STAT_H)
#include <sys/stat.h>
#endif

#if defined(HAVE_FCNTL_H)
#include <fcntl.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if defined(HAVE_ERRNO_H)
#include <errno.h>
#endif

#if defined(HAVE_SYS_ERRNO_H)
#include <sys/errno.h>
#endif

#if defined(HAVE_IO_H)
#include <io.h>
#endif

#if defined(HAVE_WCHAR_H)
#include <wchar.h>
#endif

#if defined(HAVE_DIRECT_H) 
#include <direct.h>
#endif

class REFilePrivate
{
public:
	static void initFileWithSizeFromPathAndMode(FILE ** file,
												REUInt32 * size,
												const REString & filePath,
												const char * openMode = NULL);
};

void REFilePrivate::initFileWithSizeFromPathAndMode(FILE ** file,
													REUInt32 * size,
													const REString & filePath,
													const char * openMode)
{
	FILE * f = REFile::fileOpen(filePath, openMode ? openMode : "rb");
	if (f)
	{
		if (fseek(f, 0, SEEK_END) != 0)
		{
			fclose(f);
		}
		else
		{
			const REUInt32 fsize = (REUInt32)ftell(f);
			if (fseek(f, 0, SEEK_SET) != 0)
			{
				fclose(f);
			}
			else
			{
				*file = f;
				*size = fsize;
			}
		}
	}
}


/* IREFileReadable */
REUInt32 REFile::fileRead(void * bufferForReading, const REUInt32 dataSizeToRead)
{
	if (_file && bufferForReading) 
	{
		return (REUInt32)fread(bufferForReading, 1, dataSizeToRead, _file);
	}
	return 0;
}

REUInt32 REFile::fileTell()
{
	return (_file) ? (REUInt32)ftell(_file) : 0;
}

REInt32 REFile::fileSeek(const REUInt32 fileOffset, int origin)
{
	return (_file) ? (REInt32)fseek(_file, fileOffset, origin) : 1; // 1 = error
}

REInt32 REFile::fileSeekFromEndFile(const REUInt32 fileOffset)
{
	return (_file) ? (REInt32)fseek(_file, fileOffset, SEEK_END) : 1; // 1 = error
}

REInt32 REFile::fileSeekFromBeginFile(const REUInt32 fileOffset)
{
	return (_file) ? (REInt32)fseek(_file, fileOffset, SEEK_SET) : 1; // 1 = error
}

REInt32 REFile::fileSeekFromCurrentFilePos(const REUInt32 fileOffset)
{
	return (_file) ? (REInt32)fseek(_file, fileOffset, SEEK_CUR) : 1; // 1 = error
}

REInt32 REFile::fileFError()
{
	return (_file) ? (REInt32)ferror(_file) : 0;
}

REInt32 REFile::fileClose()
{
	if (_file)
	{
		const int reslt = fclose(_file);
		_file = NULL;
		_fileSize = 0;
		return (REInt32)reslt;
	}
	return 1; //error
}

REBOOL REFile::isEndOfFile()
{
	return (_fileSize == this->fileTell());
}

const REUInt32 REFile::fileSize() const
{
	return _fileSize;
}

REFile::REFile(const REString & filePath, const char * openMode) : 
	_file(NULL),
	_fileSize(0)
{
	REFilePrivate::initFileWithSizeFromPathAndMode(&_file,
												   &_fileSize,
												   filePath,
												   openMode);
}

REFile::REFile(const REString & filePath) : 
	_file(NULL),
	_fileSize(0)
{
	REFilePrivate::initFileWithSizeFromPathAndMode(&_file,
												   &_fileSize,
												   filePath);
}

REFile::~REFile()
{
	if (_file) 
	{
		fclose(_file);
	}
}

FILE * REFile::fileOpen(const char * filePath, const char * openMode)
{
	if (filePath) 
	{
		openMode = (openMode) ? openMode : "rb";
#if defined(HAVE_FUNCTION_FOPEN_S)
		FILE * f = NULL;
		if (fopen_s(&f, filePath, mode) == 0) 
		{
			return f; 
		}
#else
		FILE * f = fopen(filePath, openMode);
		return f;
#endif		
	}
	return NULL;
}

FILE * REFile::fileOpen(const REString & filePath, const char * openMode)
{
	if (filePath.isEmpty()) 
	{
		return NULL; 
	}
	
#if defined(HAVE_FUNCTION__WFOPEN_S)	
	REWideString mode;
	if (openMode) { mode = openMode; } else { mode = L"rb"; }
	REWideString wideString(filePath);
	FILE * f = NULL;
	if (_wfopen_s(&f, wideString.wideChars(), mode.wideChars()) == 0)
	{
		return f; 
	}
#elif defined(HAVE_FUNCTION__WFOPEN)
	REWideString mode;
	if (openMode) { mode = openMode; } else { mode = L"rb"; }
	REWideString wideString(filePath);
	FILE * f = _wfopen(wideString.wideChars(), mode.wideChars());
	return f;
#else
	return REFile::fileOpen((const char *)filePath.UTF8String(), openMode);
#endif	
}


