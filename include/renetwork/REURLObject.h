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


#ifndef __REURLOBJECT_H__
#define __REURLOBJECT_H__


#include "REURL.h"
#include "../recore/REObject.h"

/// http://en.wikipedia.org/wiki/URI_scheme

class __RE_PUBLIC_CLASS_API__ REURLObject : public REObject, public REURL
{
protected:
	REURLObject(const REURL & urlValue);
	REURLObject(const REString & stringValue);
	REURLObject(const wchar_t * stringValue);
	REURLObject(const char * stringValue);
public:
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual REBOOL IsEqual(REObject * anotherObject);
	
	virtual ~REURLObject();
	
	static REURLObject * CreateWithURL(const REURL & urlValue);
	static REURLObject * CreateWithString(const REString & stringValue);
	static REURLObject * CreateWithUTF8String(const char * stringValue);
	static REURLObject * CreateWithWideString(const wchar_t * stringValue);
};


#endif /* __REURLOBJECT_H__ */

