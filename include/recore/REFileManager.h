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


#ifndef __REFILEMANAGER_H__
#define __REFILEMANAGER_H__

#include "RECommonHeader.h"
#include "REData.h"
#include "REString.h"

/// Class used for creating, checking files and directories
class __RE_PUBLIC_CLASS_API__ REFileManager
{
protected:
	/// Create last path component dir. No check for NULL.
	/// If created or exists return true.
	/// New directory permision is 777.
	REBOOL createDir(const char * path);
	
#ifdef __RE_OS_WINDOWS__ 
	/// Wide char string version of CreateDir.
	REBOOL createDir(const wchar_t * path);
#endif /* WIN */	
public:
	/// Checks is path can be readable.
	/// Returns true if can read othervice false.
	REBOOL isReadableFileAtPath(const char * path) const;
	
	/// Checks is path can be readable.
	/// Returns true if can read othervice false.
	REBOOL isReadableFileAtPath(const REString & path) const;
	
	/// Checks is path can be writable.
	/// Returns true if can write othervice false.
	REBOOL isWritableFileAtPath(const char * path) const;
	
	/// Checks is path can be writable.
	/// Returns true if can write othervice false.
	REBOOL isWritableFileAtPath(const REString & path) const;
	
	/// Creates file at specific path with some data or empty.
	/// Returns true if file created and writed othervice false.
	REBOOL createFileAtPath(const char * path, REData * withFileData = NULL) const;
	
	/// Creates file at specific path with some data or empty.
	/// Returns true if file created and writed othervice false.
	REBOOL createFileAtPath(const REString & path, REData * withFileData = NULL) const;
	
	/// Creates directory at specific path with intermediates
	/// directories if needed.
	/// Returns true if directory created othervice false.
	REBOOL createDirectoryAtPath(const char * path, REBOOL isCreateIntermediates);
	
	/// Creates directory at specific path with intermediates
	/// directories if needed.
	/// Returns true if directory created othervice false.
	REBOOL createDirectoryAtPath(const REString & path, REBOOL isCreateIntermediates);
	
	/// Checks is file or directory exists.
	/// Returns true if exists othervice false.
	/// Also can returns flag is path directory or not.
	REBOOL isFileExistsAtPath(const char * path, REBOOL * isDirectory = NULL) const;
	
	/// Checks is file or directory exists.
	/// Returns true if exists othervice false.
	/// Also can returns flag is path directory or not.
	REBOOL isFileExistsAtPath(const REString & path, REBOOL * isDirectory = NULL) const;
	
	/// Constructs file manager.
	REFileManager();
	
	/// Destructor.
	~REFileManager();
};

#endif /* __REFILEMANAGER_H__ */

