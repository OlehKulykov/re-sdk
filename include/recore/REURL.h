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


#ifndef __REURL_H__
#define __REURL_H__


#include "RECommonHeader.h"
#include "REString.h"
#include "RENumber.h"
#include "REPtr.h"


class REURLInternal;

/**
 @brief Class presented URL object.
 @detailed Description of the URL parts http://en.wikipedia.org/wiki/URI_scheme
 @example 
 foo://username:password@example.com:8042/over/there/index.dtb?type=animal&name=narwhal#nose
 \_/   \_______________/ \_________/ \__/            \___/ \_/ \______________________/ \__/
 |           |               |       |                |    |            |                |
 |       userinfo         hostname  port              |    |          query          fragment
 |    \________________________________/\_____________|____|/ \__/        \__/
 |                    |                          |    |    |    |          |
 |                    |                          |    |    |    |          |
 scheme              authority                    path   |    |    interpretable as keys
 name   \_______________________________________________|____|/       \____/     \_____/
 |                         |                          |    |          |           |
 |                 hierarchical part                  |    |    interpretable as values
 |                                                    |    |
 |            path               interpretable as filename |
 |   ___________|____________                              |
 / \ /                        \                             |
 urn:example:animal:ferret:nose               interpretable as extension
 
 path
 _________|________
 scheme /                  \
 name  userinfo  hostname       query
 _|__   ___|__   ____|____   _____|_____
 /    \ /      \ /         \ /           \
 mailto:username@example.com?subject=Topic

 */
class __RE_PUBLIC_CLASS_API__ REURL
{
private:
	void setURLString(const char * stringValue, const REUInt32 length = RENotFound);
	void setWithLen(const char * stringValue, const REUInt32 lenght);
	void parse();
	
protected:
	/**
	 @brief Autoppointer to internal URL implementation.
	 @detailed Usefull for copying URL object.
	 */
	REPtr<REURLInternal> _i;
	
public:
	/**
	 @brief URL scheme part.
	 @return Scheme string or empty string.
	 @example 'http' or 'file'
	 */
	REString schemeName() const;
	
	
	/**
	 @brief URL userinfo part.
	 @return Userinfo string or empty string.
	 @example: 'username:password'
	 */
	REString userInfo() const;
	
	
	/**
	 @brief URL hostname part.
	 @return Hostname string or empty string.
	 @example: 'example.com'
	 */
	REString hostName() const;
	
	
	/**
	 @brief URL port part.
	 @return Port number.
	 @example: '8042'
	 */
	RENumber port() const;
	
	
	/**
	 @brief URL query part.
	 @return Query string or empty string.
	 @example: 'type=animal&name=narwhal'
	 */
	REString query() const;
	
	
	/**
	 @brief URL fragment part.
	 @return Fragment string or empty string.
	 @example: 'nose'
	 */
	REString fragment() const;
	
	
	/**
	 @brief URL authority part.
	 @return Authority string or empty string.
	 @example: 'username:password@example.com:8042'
	 */
	REString authority() const;
	
	
	/**
	 @brief URL path part.
	 @return Path string or empty string.
	 @example: 'over/there/index.dtb'
	 */
	REString path() const;
	
	
	/**
	 @brief URL hierarchical part.
	 @return Hierarchical string or empty string.
	 @example: 'username:password@example.com:8042/over/there/index.dtb'
	 */
	REString hierarchicalPart() const;
	
	
	/**
	 @brief URL file name part.
	 @return File name string or empty string.
	 @example: 'index'
	 */
	REString fileName() const;
	
	
	/**
	 @brief URL extension part.
	 @return Extension string or empty string.
	 @example: 'dtb'
	 */
	REString extension() const;
	
	
	/**
	 @brief URL file name with extension part.
	 @return File name with extension string or empty string.
	 @example: 'index.dtb'
	 */
	REString fileNameWithExtension() const;
	
	
	/**
	 @brief URL user name part.
	 @return User name string or empty string.
	 @example: 'username'
	 */
	REString userName() const;
	
	
	/**
	 @brief URL password part.
	 @return Password string or empty string.
	 @example: 'password'
	 */
	REString password() const;
	
	
	/**
	 @brief URL C string.
	 @return Pointer to C string.
	 */
	const char * UTF8String() const;
	
	
	/**
	 @brief Length of the URL string.
	 */
	const REUInt32 length() const;
	
	
	/**
	 @brief Check is URL has scheme 'file'
	 @return True if has scheme 'file', othervice false.
	 */
	REBOOL isFileURL() const;
	
	
	/**
	 @brief Basic assignment operator with another URL object.
	 @param urlValue The URL object.
	 @return Address of the object.
	 */ 
	REURL & operator=(const REURL & urlValue);
	
	
	/**
	 @brief Basic assignment operator with string object.
	 @param stringValue The URL string object.
	 @return Address of the object.
	 */ 
	REURL & operator=(const REString & stringValue);
	
	
	/**
	 @brief Basic assignment operator with wide character string.
	 @param stringValue The URL wide character string pointer.
	 @return Address of the object.
	 */ 
	REURL & operator=(const wchar_t * stringValue);
	
	
	/**
	 @brief Basic assignment operator with C string.
	 @param stringValue The URL C string pointer.
	 @return Address of the object.
	 */ 
	REURL & operator=(const char * stringValue);
	
	
	/**
	 @brief Constructs URL object with another URL object.
	 @param urlValue The URL object.
	 */ 
	REURL(const REURL & urlValue);
	
	
	/**
	 @brief Constructs URL object with string object.
	 @param stringValue The URL string object.
	 */ 
	REURL(const REString & stringValue);
	
	
	/**
	 @brief Constructs URL object with wide character string.
	 @param wideString The URL wide character string pointer.
	 @param wideStringLength Length of the wide character string or RENotFound for automatic calculating length.
	 */ 
	REURL(const wchar_t * wideString, const REUInt32 wideStringLength = RENotFound);
	
	
	/**
	 @brief Constructs URL object with C string.
	 @param utf8String The URL C string pointer.
	 @param utf8StringLength Length of the C character string or RENotFound for automatic calculating length.
	 */ 
	REURL(const char * utf8String, const REUInt32 utf8StringLength = RENotFound);
	
	
	/**
	 @brief Constructs empty URL object.
	 */ 
	REURL();
	
	
	/**
	 @brief Destructor.
	 */  
	virtual ~REURL();
};


#endif /* __REURL_H__ */

