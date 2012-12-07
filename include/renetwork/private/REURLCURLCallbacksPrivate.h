/*
 *   Copyright 2012 Kulikov Oleg
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


#ifndef __REURLCURLCALLBACKSPRIVATE_H__
#define __REURLCURLCALLBACKSPRIVATE_H__

#include "../../recore/RECommonHeader.h"

#if (defined(__RE_USING_ADITIONAL_CURL_LIBRARY__) || defined(__RE_USING_SYSTEM_CURL_LIBRARY__)) 

#include "REURLResponseObjectPrivate.h"

class __RE_PUBLIC_CLASS_API__ REURLCURLCallbacksPrivate
{
public:	
	static size_t WriteFuncToREBuffer(char * ptr, size_t size, size_t nmemb, void * reBuffer);
	
	static size_t WriteHeaders(void * buffer, size_t size, size_t nmemb, void * userp);
};

#endif

#endif /* __REURLCURLCALLBACKSPRIVATE_H__ */

