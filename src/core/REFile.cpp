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
		if ( fseek(f, 0, SEEK_END) != 0 )
		{
			fclose(f);
		}
		else
		{
			const REUInt32 fsize = (REUInt32)ftell(f);
			if ( fseek(f, 0, SEEK_SET) != 0 )
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
	if (_file) 
	{
		return (REUInt32)ftell(_file);
	}
	return 0;
}

REInt32 REFile::fileSeek(const REUInt32 fileOffset, int origin)
{
	if (_file) 
	{	
		return (REInt32)fseek(_file, fileOffset, origin);
	}
	return 1; //error
}

REInt32 REFile::fileSeekFromEndFile(const REUInt32 fileOffset)
{
	if (_file) 
	{	
		return (REInt32)fseek(_file, fileOffset, SEEK_END);
	}
	return 1; //error
}

REInt32 REFile::fileSeekFromBeginFile(const REUInt32 fileOffset)
{
	if (_file) 
	{	
		return (REInt32)fseek(_file, fileOffset, SEEK_SET);
	}
	return 1; //error
}

REInt32 REFile::fileSeekFromCurrentFilePos(const REUInt32 fileOffset)
{
	if (_file) 
	{	
		return (REInt32)fseek(_file, fileOffset, SEEK_CUR);
	}
	return 1; //error
}

REInt32 REFile::fileFError()
{
	if (_file)
	{
		return (REInt32)ferror(_file);
	}
	return 0;
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

const REUInt32 REFile::getFileSize() const
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


FILE * REFile::fileOpen(const REString & filePath, const char * openMode)
{
	if ( filePath.isEmpty() ) { return NULL; }
	
#ifndef __RE_OS_WINDOWS__
	// NOT WIN
	const char * mode = NULL;
	if (openMode) { mode = openMode; }
	else { mode = "rb"; }
	
	FILE * f = fopen(filePath.getChars(), mode);
	return f;
#endif	
	
#ifdef __RE_OS_WINDOWS__
	// WIN
	if ( filePath.IsNonASCIICharsPresent() ) 
	{
		REStringPresentation p(filePath.UTF8String());
		FILE * f = NULL;
		if (openMode) 
		{
			REStringPresentation modePresent(openMode);
#ifdef _MSC_VER
            if (_wfopen_s(&f, p.WideString(), modePresent.WideString()) == 0) { return f; }
#else
            f = _wfopen( p.WideString(), modePresent.WideString());
            return f;
#endif

		}
        else
        {
#ifdef _MSC_VER
            if (_wfopen_s(&f, p.WideString(), L"rb") == 0) { return f; }
#else
            f = _wfopen(p.WideString(), L"rb");
            return f;
#endif
        }
	}
	else
	{
		FILE * f = NULL;
		const char * mode = NULL;
		if (openMode) { mode = openMode; }
		else { mode = "rb"; }
#ifdef _MSC_VER
		if (fopen_s(&f, filePath.UTF8String(), mode) == 0) { return f; }
#else
        f = fopen(filePath.UTF8String(), mode);
        return f;
#endif
	}
#endif 	
	
	return NULL;
}


