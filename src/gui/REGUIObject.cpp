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


#include "../../include/regui/REGUIObject.h"

void REGUIObject::onPrepareGUIObjectForSetuping()
{
	
}

void REGUIObject::onSetupingGUIObjectFinished(const REBOOL isAcceptedByParent)
{
	
}

REBOOL REGUIObject::acceptStringParameter(const char * key, const char * value)
{
	return false;
}

REBOOL REGUIObject::acceptObjectParameter(const char * className, const char * key, REGUIObject * value)
{
	return false;
}

IREObjectProperty * REGUIObject::getPropertyForKey(const char * key)
{
	return NULL;
}

REGUIObject::REGUIObject() : REObject()
{
	
}

REGUIObject::~REGUIObject()
{
	
}


