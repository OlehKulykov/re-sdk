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


#include "../../include/renetwork/private/REURLCURLCallbacksPrivate.h"


#if (defined(__RE_USING_ADITIONAL_CURL_LIBRARY__) || defined(__RE_USING_SYSTEM_CURL_LIBRARY__)) 


#include "../../include/recore/REBuffer.h"

size_t REURLCURLCallbacksPrivate::WriteFuncToREBuffer(char * ptr, size_t size, size_t nmemb, void * reBuffer)
{
	if (reBuffer) 
	{
		const size_t needWriteSize = size * nmemb;
		if (((REBuffer *)reBuffer)->Append(ptr, (REUInt32)needWriteSize))
		{
			return needWriteSize;
		}
	}
	return 0;
}

size_t REURLCURLCallbacksPrivate::WriteHeaders(void * buffer, size_t size, size_t nmemb, void * userp)
{
	const size_t needWriteSize = size * nmemb;
	if (userp) 
	{
		REURLResponseObjectPrivate * resp = (REURLResponseObjectPrivate *)userp;
		const char * s = (const char *)buffer;
		switch (*s) 
		{
			case 'H':
			{
				float f = -1.0f;
				int i = -1;
				if (sscanf(s, "HTTP/%f %i", &f, &i) == 2) 
				{
					if (i >= 0)
					{
						resp->SetStatusCode((REInt32)i);
					}
				}
			}
				break;
			case 'C':
			{
				char contType[128] = { 0 };
				unsigned long long contLen = 0;
				if (sscanf(s, "Content-Type: %s", contType) == 1) 
				{
					resp->SetMIMEType(contType);
				}
				else if (sscanf(s, "Content-Length: %ull", &contLen) == 1)
				{
					resp->SetExpectedContentSize((REUInt32)contLen);
					resp->SetIsExpectedContentSizeDetermined(true);
				}
			}
				break;
			default:
				break;
		}
		//printf("\n%s", s);
	}
	return needWriteSize;
}

#endif




