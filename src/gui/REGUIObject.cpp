/*
 *   Copyright 2012 Kulykov Oleh
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


#include "../../include/regui/REGUIObject.h"

const REUInt32 REGUIObject::GetClassIdentifier() const
{
	return REGUIObject::ClassIdentifier();
}

const REUInt32 REGUIObject::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REGUIObject");
	return clasIdentif;
}

REBOOL REGUIObject::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REGUIObject::ClassIdentifier() == classIdentifier) || REObject::IsImplementsClass(classIdentifier));
}

void REGUIObject::OnPrepareGUIObjectForSetuping()
{
	
}

void REGUIObject::OnSetupingGUIObjectFinished(const REBOOL isAcceptedByParent)
{
	
}

REBOOL REGUIObject::AcceptStringParameter(const char * key, const char * value)
{
	return false;
}

REBOOL REGUIObject::AcceptObjectParameter(const char * className, const char * key, REGUIObject * value)
{
	return false;
}

IREObjectProperty * REGUIObject::GetPropertyForKey(const char * key)
{
	return NULL;
}

REGUIObject::REGUIObject() : REObject()
{
	
}

REGUIObject::~REGUIObject()
{
	
}


