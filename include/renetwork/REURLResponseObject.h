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


#ifndef __REURLRESPONCEOBJECT_H__
#define __REURLRESPONCEOBJECT_H__


#include "REURLObject.h"
#include "../recore/REObject.h"
#include "../recore/REString.h"


class __RE_PUBLIC_CLASS_API__ REURLResponseObject : public REObject
{
private:
	
protected:
	REString _suggestedFileName;
	REString _MIMEType;
	REURLObject * _urlObject;
	REUInt32 _expectedContentSize;
	REInt32 _statusCode;
	REBOOL _isExpectedContentSizeDetermined;

	REURLResponseObject();
public:
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	
	REURLObject * GetURLObject() const;
	const REUInt32 GetExpectedContentSize() const;
	REBOOL IsExpectedContentSizeDetermined() const;
	const REInt32 GetStatusCode() const;
	const REString & GetMIMEType() const;
	const REString & GetSuggestedFileName() const;
	
	virtual ~REURLResponseObject();
};


#endif /* __REURLRESPONCEOBJECT_H__ */

