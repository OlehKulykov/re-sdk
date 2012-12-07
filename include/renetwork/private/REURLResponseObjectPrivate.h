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


#ifndef __REURLRESPONCEOBJECTPRIVATE_H__
#define __REURLRESPONCEOBJECTPRIVATE_H__


#include "../REURLResponseObject.h"


class __RE_PUBLIC_CLASS_API__ REURLResponseObjectPrivate : public REURLResponseObject
{
public:	
	void SetURL(REURLObject * url)
	{
		RE_SAFE_RETAIN(_urlObject, url);
	}
	
	void SetExpectedContentSize(const REUInt32 expSize) 
	{
		_expectedContentSize = expSize; 
	}
	
	void SetIsExpectedContentSizeDetermined(REBOOL isExpSizeDetermined)
	{
		_isExpectedContentSizeDetermined = isExpSizeDetermined;
	}
	
	void SetMIMEType(const char * mmt) 
	{
		_MIMEType.Set(mmt);
	}
	
	void SetSuggestedFileName(const char * sfn)
	{
		_suggestedFileName.Set(sfn);
	}
	
	void SetSuggestedFileName(const REString & sfn)
	{
		_suggestedFileName.Set(sfn);
	}
	
	void SetStatusCode(const REInt32 stCode)
	{
		_statusCode = stCode;
	}
	
	REURLResponseObjectPrivate() : REURLResponseObject() { }
	virtual ~REURLResponseObjectPrivate() { }
};


#endif /* __REURLRESPONCEOBJECTPRIVATE_H__ */

