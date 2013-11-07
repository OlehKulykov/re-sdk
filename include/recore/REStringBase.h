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


#ifndef __RESTRINGBASE_H__ 
#define __RESTRINGBASE_H__ 

#include "RECommonHeader.h"
#include "REPtr.h"
#include "REBuffer.h"

typedef enum _reStringType
{
	REStringTypeUTF8 = 0,
	REStringTypeWide = 1
} REStringType;

class __RE_PUBLIC_CLASS_API__ REStringBase
{
protected:
	REPtr<REBuffer> _p;
	
	void * stringBuffer() const;
	REUInt32 stringBufferSize() const;
public:
	
	REBOOL isEmpty() const;
	REBOOL isNotEmpty() const;
	void clear();
	
	void setFromUTF8String(const char * utf8String, 
						   const REUInt32 utf8StringLength = RENotFound,
						   const REStringType toType = REStringTypeUTF8);
	
	void setFromWideString(const wchar_t * wideString, 
						   const REUInt32 wideStringLength = RENotFound,
						   const REStringType toType = REStringTypeUTF8);
	
	REStringBase();
	
	REStringBase(const char * utf8String, 
				 const REUInt32 utf8StringLength = RENotFound,
				 const REStringType toType = REStringTypeUTF8);
	
	REStringBase(const wchar_t * wideString, 
				 const REUInt32 wideStringLength = RENotFound,
				 const REStringType toType = REStringTypeUTF8);
	
	REStringBase(const REPtr<REBuffer> & stringBuffer);
	
	virtual ~REStringBase();
};

#endif /* __RESTRINGBASE_H__  */

