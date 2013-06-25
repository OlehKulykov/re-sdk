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


#include "../../include/recore/REFileManager.h"
#include "../../include/recore/REFile.h"
#include "../../include/recore/REMem.h"

#include <sys/stat.h>
#include <fcntl.h>

#ifndef __RE_OS_WINDOWS__
#include <unistd.h>
#endif


#if ( defined(__RE_OS_MACOSX__) || defined(__RE_OS_IPHONE__) )
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef __RE_OS_WINDOWS__
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#endif

REBOOL REFileManager::isReadableFileAtPath(const char * path) const
{
	if (path) 
	{
#ifndef __RE_OS_WINDOWS__
		if (access(path, R_OK) == 0)
		{
			return true;
		}
#endif /* NOT WIN */

#ifdef __RE_OS_WINDOWS__
		const DWORD dwAttrs = GetFileAttributesA(path);
		if (dwAttrs == INVALID_FILE_ATTRIBUTES) 
		{
			return false;
		}
		return true; 
#endif /* WIN */
	}
	return false;

}

REBOOL REFileManager::isReadableFileAtPath(const REString & path) const
{
	if ( path.isEmpty() ) 
	{
		return false;
	}

#ifndef __RE_OS_WINDOWS__	
	return this->isReadableFileAtPath(path.getChars());
#endif /* NOT WIN */
	
#ifdef __RE_OS_WINDOWS__
	REStringPresentation p(path.UTF8String());
	const wchar_t * widePath = p.WideString();
	if (p.GetWideLength() && widePath) 
	{
		const DWORD dwAttrs = GetFileAttributesW(widePath);
		if (dwAttrs == INVALID_FILE_ATTRIBUTES) 
		{
			return false;
		}
		return true;
	}
	return false;
#endif /* WIN */
}

REBOOL REFileManager::isWritableFileAtPath(const char * path) const
{
	if (path) 
	{
#ifndef __RE_OS_WINDOWS__
		if (access(path, W_OK) == 0)
		{
			return true;
		}
#endif /* NOT WIN */

#ifdef __RE_OS_WINDOWS__
		const DWORD dwAttrs = GetFileAttributesA(path);
		if (dwAttrs == INVALID_FILE_ATTRIBUTES) 
		{
			return false;
		}

		if (!(dwAttrs & FILE_ATTRIBUTE_READONLY)) 
		{ 
			return true; 
		} 
#endif /* WIN */
	}
	return false;

}

REBOOL REFileManager::isWritableFileAtPath(const REString & path) const
{
	if ( path.isEmpty() ) 
	{
		return false;
	}

#ifndef __RE_OS_WINDOWS__	
	return this->isWritableFileAtPath(path.getChars());
#endif /* NOT WIN */
	
#ifdef __RE_OS_WINDOWS__
	REStringPresentation p(path.UTF8String());
	const wchar_t * widePath = p.WideString();
	if (p.GetWideLength() && widePath) 
	{
		const DWORD dwAttrs = GetFileAttributesW(widePath);
		if (dwAttrs == INVALID_FILE_ATTRIBUTES) 
		{
			return false;
		}
		
		if (!(dwAttrs & FILE_ATTRIBUTE_READONLY)) 
		{ 
			return true; 
		} 
	}
	return false;
#endif /* WIN */
}

REBOOL REFileManager::createFileAtPath(const char * path, REData * withFileData) const
{
	if (path) 
	{
		FILE * f = REFile::fileOpen(REString(path), "wb+");
		if (f)
		{
			size_t dwBytesToWrite = 0;
			size_t dwBytesWritten = 0;
			if (withFileData)
			{
				if ( !withFileData->isEmpty() )
				{
					dwBytesToWrite = withFileData->getSize();
					dwBytesWritten = fwrite(withFileData->getBytes(), 1, dwBytesToWrite, f);
				}
			}
			fclose(f);
			return (dwBytesToWrite == dwBytesWritten);
		}
	}
	return false;
}

REBOOL REFileManager::createFileAtPath(const REString & path, REData * withFileData) const
{
	if ( path.isEmpty() ) 
	{
		return false;
	}

#ifndef __RE_OS_WINDOWS__	
	return this->createFileAtPath(path.getChars(), withFileData);
#endif /* NOT WIN */
	
#ifdef __RE_OS_WINDOWS__
	FILE * f = REFile::fileOpen(path, "wb+");
	if (f)
	{
		size_t dwBytesToWrite = 0;
		size_t dwBytesWritten = 0;
		if (withFileData)
		{
			if ( !withFileData->IsEmpty() )
			{
				dwBytesToWrite = withFileData->GetSize();
				dwBytesWritten = fwrite(withFileData->GetBytes(), 1, dwBytesToWrite, f);
			}
		}
		fclose(f);
		return (dwBytesToWrite == dwBytesWritten);
	}
	return false;
#endif	/* WIN */ 
}

REBOOL REFileManager::isFileExistsAtPath(const char * path, REBOOL * isDirectory) const
{
    if (isDirectory)
    {
        *isDirectory = false;
    }
    if ( path == NULL )
    {
        return false;
    }

#ifdef __RE_OS_WINDOWS__
    struct _stat statbuf;
    if (_stat(path, &statbuf) != 0)
    {
        return false;
    }
#endif /* WIN */

#ifndef __RE_OS_WINDOWS__
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
	{
		return false;
	}
#endif /* NOT WIN */ 
	
    if (isDirectory)
    {
        if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
        {
            *isDirectory = true;
        }
    }

	return true;
}

REBOOL REFileManager::isFileExistsAtPath(const REString & path, REBOOL * isDirectory) const
{
	if ( path.isEmpty() ) 
	{
		if ( isDirectory ) 
		{
			*isDirectory = false;
		}
		return false;
	}
	
#ifndef __RE_OS_WINDOWS__	
	return this->isFileExistsAtPath(path.getChars(), isDirectory);
#endif /* NOT WIN */	
	
#ifdef __RE_OS_WINDOWS__
	REStringPresentation p(path.UTF8String());
	const wchar_t * widePath = p.WideString();
	if (p.GetWideLength() && widePath) 
	{
		if (isDirectory) 
		{
			*isDirectory = false;
		}
		
        struct _stat statbuf;
        if (_wstat(widePath, &statbuf) != 0)
        {
            return false;
        }

        if (isDirectory)
        {
            if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
            {
                *isDirectory = true;
            }
        }

        return true;
	}
	return false;
#endif /* WIN */	
}

REBOOL REFileManager::createDir(const char * path)
{
#ifndef __RE_OS_WINDOWS__	
	if (mkdir(path, 777) == 0) //created
	{
		return true;
	}
#endif /* NOT WIN */
	
#ifdef __RE_OS_WINDOWS__	
	if (_mkdir(path) == 0) //created
	{
		return true;
	}
#endif /* WIN */
	
	const int mkResult = errno;
	if (mkResult == EEXIST) //allready exists - return true
	{
		return true;
	}
	
	return false;
}

#ifdef __RE_OS_WINDOWS__ 
/// Wide char string version of CreateDir.
REBOOL REFileManager::CreateDir(const wchar_t * path)
{
	if (_wmkdir(path) == 0) //created
	{
		return true;
	}	
	const int mkResult = errno;
	if (mkResult == EEXIST) //allready exists - return true
	{
		return true;
	}
	
	return false;
}
#endif /* WIN */	

REBOOL REFileManager::createDirectoryAtPath(const char * path, REBOOL isCreateIntermediates)
{
	if (path == NULL) 
	{
		return false;
	}
	
	if (isCreateIntermediates) 
	{
		const REUInt32 len = strlen(path);
		if (len == 0) { return false; }
		REBuffer buf(len + 1);
		if (buf.getSize() == (len + 1)) 
		{
			REMem::Memcpy(buf.getBuffer(), path, len);
			char * p = (char *)buf.getBuffer();
			p[len] = 0;
			while ((*p == '/') || (*p == '\\')) { p++; } // skip first '/'
			while (*p) 
			{
				if ((*p == '/') || (*p == '\\')) 
				{
					*p = 0;
					if ( !this->createDir((const char *)buf.getBuffer()) )
					{
						return false;
					}
					*p = '/';
				}
				p++;
			}
			return this->createDir((const char *)buf.getBuffer());
		}
	}
	else
	{
		return this->createDir(path);
	}
	
	return false;
}

REBOOL REFileManager::createDirectoryAtPath(const REString & path, REBOOL isCreateIntermediates)
{
	if ( path.isEmpty() ) 
	{
		return false;
	}
	
#ifndef __RE_OS_WINDOWS__	
	return this->createDirectoryAtPath(path.getChars(), isCreateIntermediates);
#endif /* NOT WIN */	

#ifdef __RE_OS_WINDOWS__
	REStringPresentation presentationOfPath(path.UTF8String());
	const wchar_t * widePath = presentationOfPath.WideString();
	if (presentationOfPath.GetWideLength() && widePath) 
	{
		if (isCreateIntermediates) 
		{
			const REUInt32 len = presentationOfPath.GetWideLength();
			wchar_t * buff = const_cast<wchar_t*>(widePath);
			if (buff == NULL) { return false; }
			wchar_t * p = buff;
			p[len] = 0;
			while ((*p == L'/') || (*p == L'\\')) { p++; } // skip first '/'
			while (*p) 
			{
				if ((*p == L'/') || (*p == L'\\')) 
				{
					*p = 0;
					if ( !this->CreateDir((const wchar_t*)buff) )
					{
						return false;
					}
					*p = L'/';
				}
				p++;
			}
			return this->createDir((const wchar_t*)buff);
		}
		else
		{
			return this->createDir(widePath);
		}
	}
	return false;
#endif /* WIN */
}

REFileManager::REFileManager()
{
	
}

REFileManager::~REFileManager()
{
	
}

/*
const char REFileManager::GetPathSeparator()
{
#ifdef __RE_OS_WINDOWS__
	return '\\';
#endif 
	
#if ( defined(__RE_OS_MACOSX__) || defined(__RE_OS_LINUX__) || defined(__RE_OS_IPHONE__) )	
	return '/';	
#endif
	
	return '/';
}
*/
