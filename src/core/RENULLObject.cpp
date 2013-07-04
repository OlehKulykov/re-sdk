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


#include "../../include/recore/RENULLObject.h"


__RE_PUBLIC_CLASS_API__ RENULLObject * RENULLObject::_defaultNull = NULL;

/* REObject */
const REUInt32 RENULLObject::getClassIdentifier() const
{
	return RENULLObject::classIdentifier();
}

const REUInt32 RENULLObject::classIdentifier()
{
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("RENULLObject");
	return clasIdentif;
}

REBOOL RENULLObject::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((RENULLObject::classIdentifier() == classIdentifier) || REObject::isImplementsClass(classIdentifier));
}

RENULLObject::RENULLObject() : RENULL(), REObject()
{
	
}

RENULLObject::~RENULLObject()
{
	if (this == _defaultNull) 
	{
		_defaultNull = NULL;
	}
}

RENULLObject * RENULLObject::NULLObject()
{
	if (_defaultNull) 
	{
		return _defaultNull;
	}
	else
	{
		_defaultNull = new RENULLObject();
		if (_defaultNull) 
		{
			_defaultNull->retain();
		}
	}
	return _defaultNull;
}




