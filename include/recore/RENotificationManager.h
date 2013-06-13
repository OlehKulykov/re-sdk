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


#ifndef __RENOTIFICATIONMANAGER_H__
#define __RENOTIFICATIONMANAGER_H__

#include "RECommonHeader.h"
#include "REObject.h"
#include "REClassMethod.h"
#include "REString.h"

/// Class of notification manager.
class __RE_PUBLIC_CLASS_API__ RENotificationManager
{
public:
	/// Adds observer object for notification name using observer object class method.
	static REBOOL AddObserverForNotificationName(REObject * observerObject, const REString & notificationName, REClassMethod * observerMethod);
	
	/// Removes observer object.
	static REBOOL RemoveObserver(REObject * observerObject);
	
	/// Removes observer object for notification name.
	static REBOOL RemoveObserverForNotificationName(REObject * observerObject, const REString & notificationName);
	
	/// Posts notification with name.
	static REBOOL PostNotificationName(const REString & notificationName);
	
	/// Posts notification with name and object.
	static REBOOL PostNotificationNameWithObject(const REString & notificationName, REObject * object);
};


#endif /* __RENOTIFICATIONMANAGER_H__ */
