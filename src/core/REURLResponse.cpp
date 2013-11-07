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


#include "../../include/recore/REURLResponse.h"
#include "../../include/recore/REMD5Generator.h"

const REURL & REURLResponse::url() const
{
	return _url;
}

const REUInt32 REURLResponse::expectedContentSize() const
{
	return _expectedContentSize;
}

REBOOL REURLResponse::isExpectedContentSizeDetermined() const
{
	return _isExpectedContentSizeDetermined;
}

const REInt32 REURLResponse::statusCode() const
{
	return _statusCode;
}

const REFloat32 REURLResponse::httpVersion() const
{
	return _httpVersion;
}

const REString & REURLResponse::MIMEType() const
{
	return _MIMEType;
}

const REString & REURLResponse::suggestedFileName() const
{
	return _suggestedFileName;
}

REURLResponse::REURLResponse() :
	_expectedContentSize(0),
	_statusCode(-1),
	_httpVersion(-1.0f),
	_isExpectedContentSizeDetermined(false)
{
	
}

REURLResponse::~REURLResponse()
{
	
}


