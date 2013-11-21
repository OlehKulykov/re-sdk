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

/**
 @brief Class presented memory data.
 @detailed Based on REBuffer object with posibility to load from file or internal resources. Implements IREFileReadable interface for reading data file C FILE object.
 */
class __RE_PUBLIC_CLASS_API__ REData : public IREFileReadable
{
protected:
	/**
	 @brief Smart pointer of the REBuffer object for holding data memory buffer.
	 */	
	REPtr<REBuffer> _buffer;
	
	
	/**
	 @brief 32 bit file offset used for implementing IREFileReadable functionality
	 */	
	REUInt32 _fileOffset;
	
	
	/**
	 @brief Try init data object with the data from file path.
	 @param filePath The file path.
	 @param filePathLength The length of the file path or RENotFound for automat calculation.
	 @return True if initialized(file exists), othervice false.
	 */	
	REBOOL initializeFromFilePath(const char * filePath, const REUInt32 filePathLength);
	
	
	/**
	 @brief Try init data object with the data from internal resources.
	 @param filePath The file path.
	 @param filePathLength The length of the file path or RENotFound for automat calculation.
	 @return True if initialized(internal resource exists), othervice false.
	 */	
	REBOOL initializeFromResourcePath(const char * filePath, const REUInt32 filePathLength);
	
	
	/**
	 @brief Try init data object with the data from file or internal resources.
	 @param filePath The file path.
	 @param filePathLength The length of the file path or RENotFound for automat calculation.
	 @return True if initialized, othervice false.
	 */
	REBOOL initFromPath(const char * filePath, const REUInt32 filePathLength);
public:
	/* IREFileReadable */
	/**
	 @brief Reads data from object to the buffer pointer.
	 @detailed Buffer must be at least of 'dataSizeToRead'.
	 @param bufferForReading The buffer for writing data. 
	 @param dataSizeToRead Size in bytes for readed data.
	 @return Returns number of bytes readed.
	 */
	virtual REUInt32 fileRead(void * bufferForReading, const REUInt32 dataSizeToRead);
	
	
	/**
	 @brief Read position of the object.
	 @return Return readable position.
	 */
	virtual REUInt32 fileTell();
	
	
	/**
	 @brief Setting position in bytes of reading.
	 @detailed Apply read offset of the object.
	 @param fileOffset Offset in bytes.
	 @param origin Position used as reference for the fileOffset. SEEK_SET - Beginning of file. SEEK_CUR - Current position of the file pointer. SEEK_END - End of file.
	 @return If successful, the function returns a zero value, otherwise nonzero value.
	 */
	virtual REInt32 fileSeek(const REUInt32 fileOffset, int origin);
	
	
	/**
	 @brief Setting position in bytes of reading from end of the file.
	 @detailed Apply read offset of the object from end of the file using SEEK_END value.
	 @param fileOffset Offset in bytes.
	 @return If successful, the function returns a zero value, otherwise nonzero value.
	 */
	virtual REInt32 fileSeekFromEndFile(const REUInt32 fileOffset);
	
	
	/**
	 @brief Setting position in bytes of reading from begining of the file.
	 @detailed Apply read offset of the object from begining of the file using SEEK_SET value.
	 @param fileOffset Offset in bytes.
	 @return If successful, the function returns a zero value, otherwise nonzero value.
	 */
	virtual REInt32 fileSeekFromBeginFile(const REUInt32 fileOffset);
	
	
	/**
	 @brief Setting position in bytes of reading from current location of the file.
	 @detailed Apply read offset of the object from current location of the file using SEEK_CUR value.
	 @param fileOffset Offset in bytes.
	 @return If successful, the function returns a zero value, otherwise nonzero value.
	 */
	virtual REInt32 fileSeekFromCurrentFilePos(const REUInt32 fileOffset);
	
	
	/**
	 @brief Checks if the error indicator associated with stream is set.
	 @detailed This indicator is generaly set by a previous operation on the stream that failed.
	 @return Returning a value different from zero if it is.
	 */
	virtual REInt32 fileFError();
	
	
	/**
	 @brief Closes the file associated with the stream and disassociates it.
	 @return If the stream is successfully closed, a zero value is returned. On failure, 1 is returned.
	 */
	virtual REInt32 fileClose();
	
	
	/**
	 @brief Check if reading position in the end of file.
	 @return True - on the end of the file, othervice false.
	 */
	virtual REBOOL isEndOfFile();
	
	
	/**
	 @brief Initializes data from file path.
	 @detailed First try to init data from avaiable resources and than try to init directly from file.
	 @return True if successfully readed, othervice false.
	 */ 
	REBOOL initFromPath(const char * filePath);
	
	
	/**
	 @brief Initializes data from file path.
	 @detailed First try to init data from avaiable resources and than try to init directly from file.
	 @param filePath The file or resource path.
	 @return True if successfully readed/initialized, othervice false.
	 */  
	REBOOL initFromPath(const REString & filePath);
	
	
	/**
	 @brief Initializes data from buffer.
	 @detailed Buffer will be copyed.
	 @param buffer The buffer object.
	 @return True if successfully initialized, othervice false.
	 */  
	REBOOL initFromBuffer(const REBuffer & buffer);
	
	
	/**
	 @brief Basic assignment operator with another data object.
	 @param anotherData The data object.
	 @return Address of the object.
	 */  
	REData & operator=(const REData & anotherData);
	
	
	/**
	 @brief Basic assignment operator with another buffer object.
	 @param anotherData The buffer object.
	 @return Address of the object.
	 */  
	REData & operator=(const REBuffer & anotherBuffer);
	
	
	/**
	 @brief Basic assignment operator with autopointer to buffer object.
	 @param anotherData The buffer object.
	 @return Address of the object.
	 */  
	REData & operator=(const REPtr<REBuffer> & anotherBufferPtr);
	
	
	/**
	 @brief Initializing data object from URL String.
	 @detailed This functionality avaiable only with RENetwork and in config file must be defined __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__.
	 @param urlString The URL object.
	 @return True if successfully initialized, othervice false.
	 @example http://example.com/index.html for downloading from web.
	 @example file:///Volumes/Data/file.dat for reading from from file. See file url scemes.
	 */  
	REBOOL initFromURLString(const REString & urlString);
	
	
	/**
	 @brief Clears data object and set read offset to 0.
	 */ 
	void clear();
	
	
	/**
	 @brief Get data memory buffer.
	 @return Pointer to data memory buffer.
	 */  
	const REUByte * bytes() const;
	
	
	/**
	 @brief Get data size.
	 @return Size of the data object.
	 */  
	REUInt32 size() const;
	
	
	/**
	 @brief Get hash of the data.
	 @return CRC32 hash of the data object.
	 */ 
	REUInt32 hash() const;
	
	
	/**
	 @brief Checks is data empty.
	 @datailed Size of the data is 0.
	 @return True if empty, othervice false.
	 */ 
	REBOOL isEmpty() const;
	
	
	/**
	 @brief Constructs empty data.
	 */  
	REData();
	
	
	/**
	 @brief Constructs data object with buffer.
	 @detailed buffer will be retained and deleted after using. Do not delete 'buffer' manually.
	 @param buffer The pointer to the buffer object.
	 */  
	REData(REBuffer * buffer);
	
	
	/**
	 @brief Constructs data object with autopointer to buffer.
	 @detailed deleting of the buffer managing by autopointer.
	 @param buffer Autopointer to buffer.
	 */  
	REData(const REPtr<REBuffer> & buffer);
	
	
	/**
	 @brief Construct data from another data.
	 @detailed Data buffer not copyed, retained.
	 @param anotherData Address of the another data object.
	 */  
	REData(const REData & anotherData);
	
	
	/**
	 @brief Constructs data object with buffer.
	 @detailed Buffer will be copyed.
	 @param buffer Address to the buffer object.
	 */ 
	REData(const REBuffer & buffer);
	
	
	/**
	 @brief Destructor.
	 */  
	virtual ~REData();
};

#endif /* __REDATA_H__ */


