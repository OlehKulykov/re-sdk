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


#include "../../include/renetwork/REURLResponseObject.h"
#include "../../include/recore/REMD5Generator.h"


/* REObject */
const REUInt32 REURLResponseObject::GetClassIdentifier() const
{
	return REURLResponseObject::ClassIdentifier();
}

const REUInt32 REURLResponseObject::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REMD5Generator::GenerateFromString("REURLResponseObject");
	return clasIdentif;
}

REURLObject * REURLResponseObject::GetURLObject() const
{
	return _urlObject;
}

const REUInt32 REURLResponseObject::GetExpectedContentSize() const
{
	return _expectedContentSize;
}

REBOOL REURLResponseObject::IsExpectedContentSizeDetermined() const
{
	return _isExpectedContentSizeDetermined;
}

const REString & REURLResponseObject::GetMIMEType() const
{
	return _MIMEType;
}

const REString & REURLResponseObject::GetSuggestedFileName() const
{
	return _suggestedFileName;
}

const REInt32 REURLResponseObject::GetStatusCode() const
{
	return _statusCode;
}

REURLResponseObject::REURLResponseObject() : REObject(),
	_urlObject(NULL),
	_expectedContentSize(0),
	_statusCode(-1),
	_isExpectedContentSizeDetermined(false)
{
	
}

REURLResponseObject::~REURLResponseObject()
{
	if (_urlObject)
	{
		_urlObject->Release();
	}
}

