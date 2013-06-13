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


#ifndef __IREFILEREADABLE_H__
#define __IREFILEREADABLE_H__

#include "RECommonHeader.h"

/// Interface for classes that can be used for reading some data.
/// Members is simular to 'c FILE' functions.
class __RE_PUBLIC_CLASS_API__ IREFileReadable
{
public:
	/// Reads 'dataSizeToRead' bytes to 'bufferForReading'. Buffer must be al least of 'dataSizeToRead'.
	/// Returns number of bytes readed.
	virtual REUInt32 FileRead(void * bufferForReading, const REUInt32 dataSizeToRead) = 0;
	
	/// Return readable position.
	virtual REUInt32 FileTell() = 0;
	
	/// Setting position in bytes of reading.
	/// If successful, the function returns a zero value.
	/// Otherwise, it returns nonzero value.
	virtual REInt32 FileSeek(const REUInt32 fileOffset, int origin) = 0;
	
	/// Set reading position in bytes from end of file.
	/// If successful, the function returns a zero value.
	/// Otherwise, it returns nonzero value.
	virtual REInt32 FileSeekFromEndFile(const REUInt32 fileOffset) = 0;
	
	/// Set reading position in bytes from begining of file.
	/// If successful, the function returns a zero value.
	/// Otherwise, it returns nonzero value.
	virtual REInt32 FileSeekFromBeginFile(const REUInt32 fileOffset) = 0;
	
	/// Set reading position in bytes from current location.
	/// If successful, the function returns a zero value.
	/// Otherwise, it returns nonzero value.
	virtual REInt32 FileSeekFromCurrentFilePos(const REUInt32 fileOffset) = 0;
	
	/// Checks if the error indicator associated with stream is set, returning a value different from zero if it is.
	/// This indicator is generaly set by a previous operation on the stream that failed.
	virtual REInt32 FileFError() = 0;
	
	/// Closes the file associated with the stream and disassociates it.
	/// If the stream is successfully closed, a zero value is returned.
	/// On failure, 1 is returned.
	virtual REInt32 FileClose() = 0;
	
	/// Check if reading position in the end of file.
	virtual REBOOL IsEndOfFile() = 0;
	
	virtual ~IREFileReadable() { }
	
};


#endif /* __IREFILEREADABLE_H__ */


