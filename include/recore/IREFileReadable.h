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

/**
 @brief Interface for classes that can be used for reading some data.
 @detailed Members is simular to 'c FILE' functions.
 */
class __RE_PUBLIC_CLASS_API__ IREFileReadable
{
public:
	/**
	 @brief Reads data from object to the buffer pointer.
	 @detailed Buffer must be at least of 'dataSizeToRead'.
	 @param bufferForReading The buffer for writing data. 
	 @param dataSizeToRead Size in bytes for readed data.
	 @result Returns number of bytes readed.
	 */
	virtual REUInt32 fileRead(void * bufferForReading, const REUInt32 dataSizeToRead) = 0;
	
	
	/**
	 @brief Read position of the object.
	 @result Return readable position.
	 */
	virtual REUInt32 fileTell() = 0;
	
	
	/**
	 @brief Setting position in bytes of reading.
	 @detailed Apply read offset of the object.
	 @param fileOffset Offset in bytes.
	 @param origin Position used as reference for the fileOffset. SEEK_SET - Beginning of file. SEEK_CUR - Current position of the file pointer. SEEK_END - End of file.
	 @return If successful, the function returns a zero value, otherwise nonzero value.
	 */
	virtual REInt32 fileSeek(const REUInt32 fileOffset, int origin) = 0;
	
	
	/**
	 @brief Setting position in bytes of reading from end of the file.
	 @detailed Apply read offset of the object from end of the file using SEEK_END value.
	 @param fileOffset Offset in bytes.
	 @return If successful, the function returns a zero value, otherwise nonzero value.
	 */
	virtual REInt32 fileSeekFromEndFile(const REUInt32 fileOffset) = 0;
	
	
	/**
	 @brief Setting position in bytes of reading from begining of the file.
	 @detailed Apply read offset of the object from begining of the file using SEEK_SET value.
	 @param fileOffset Offset in bytes.
	 @return If successful, the function returns a zero value, otherwise nonzero value.
	 */
	virtual REInt32 fileSeekFromBeginFile(const REUInt32 fileOffset) = 0;
	
	
	/**
	 @brief Setting position in bytes of reading from current location of the file.
	 @detailed Apply read offset of the object from current location of the file using SEEK_CUR value.
	 @param fileOffset Offset in bytes.
	 @return If successful, the function returns a zero value, otherwise nonzero value.
	 */
	virtual REInt32 fileSeekFromCurrentFilePos(const REUInt32 fileOffset) = 0;
	
	
	/**
	 @brief Checks if the error indicator associated with stream is set.
	 @detailed This indicator is generaly set by a previous operation on the stream that failed.
	 @return Returning a value different from zero if it is.
	 */
	virtual REInt32 fileFError() = 0;
	
	
	/**
	 @brief Closes the file associated with the stream and disassociates it.
	 @return If the stream is successfully closed, a zero value is returned. On failure, 1 is returned.
	 */
	virtual REInt32 fileClose() = 0;
	
	
	/**
	 @brief Check if reading position in the end of file.
	 @return Treu - on the end of the file, othervice false.
	 */
	virtual REBOOL isEndOfFile() = 0;
	
	
	/**
	 @brief Default destructor.
	 */
	virtual ~IREFileReadable() { }
};


#endif /* __IREFILEREADABLE_H__ */


