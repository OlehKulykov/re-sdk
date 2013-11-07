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


#ifndef __RESTATICSTRING_H__ 
#define __RESTATICSTRING_H__ 

#include "RECommonHeader.h"
#include "REString.h"


class __RE_PUBLIC_CLASS_API__ REStaticString : public REString
{
private:
	static void freeNonCopyBuffMem(void * m);
public:
	REStaticString(const char * utf8String, const REUInt32 utf8StringLength = RENotFound);
	REStaticString(const wchar_t * wideString, const REUInt32 wideStringLength = RENotFound);
	
	virtual ~REStaticString();
};

#endif /* __RESTATICSTRING_H__  */

