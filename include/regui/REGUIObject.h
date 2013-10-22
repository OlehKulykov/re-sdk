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


#ifndef __REGUIOBJECT_H__
#define __REGUIOBJECT_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REObject.h"
#include "../recore/REObjectProperty.h"

/// Base class for gui objects.
class __RE_PUBLIC_CLASS_API__ REGUIObject : public REObject
{
protected:
	REGUIObject();
	virtual ~REGUIObject();

public:	
	/// Informs object about begining of setuping. 
	virtual void onPrepareGUIObjectForSetuping();
	
	/// Informs object that setuping is done and tell that the object was accepted by parent 'GUIObject' object.
	/// If parent object not accept this object than this object will be deleted after receiving this info.
	virtual void onSetupingGUIObjectFinished(const REBOOL isAcceptedByParent);
	
	/// Acepting string parameter and value string presentation from XML document. 
	/// Using when view controller loading from XML. This method can be invoked from non-main thread if view controller loading in background.
	virtual REBOOL acceptStringParameter(const char * key, const char * value);
	
	/// Acepting object value for key from XML document with object's class name . 
	/// Using when view controller loading from XML. This method can be invoked from non-main thread if view controller loading in background.
	virtual REBOOL acceptObjectParameter(const char * className, const char * key, REGUIObject * value);
	
	/// If object containes property than it's will be askes for pointer to property by key.
	/// If property not fount than return NULL.
	virtual IREObjectProperty * getPropertyForKey(const char * key);
};


#endif /* __REGUIOBJECT_H__ */

