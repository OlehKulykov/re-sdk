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
const REUInt32 RENULLObject::GetClassIdentifier() const
{
	return RENULLObject::ClassIdentifier();
}

const REUInt32 RENULLObject::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("RENULLObject");
	return clasIdentif;
}

REBOOL RENULLObject::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((RENULLObject::ClassIdentifier() == classIdentifier) || REObject::IsImplementsClass(classIdentifier));
}

RENULLObject::RENULLObject() : REObject()
{
	
}

RENULLObject::~RENULLObject()
{
	
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
			_defaultNull->Retain();
		}
	}
	return _defaultNull;
}




