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


#ifndef __RENULLOBJECT_H__
#define __RENULLOBJECT_H__

#include "RECommonHeader.h"
#include "RENULL.h"
#include "REObject.h"

/// Class of NULL object. Just existen object that inform that no normal object detected.
class __RE_PUBLIC_CLASS_API__ RENULLObject : public RENULL, public REObject
{
protected:	
	RENULLObject();
	virtual ~RENULLObject();
public:
	/* REObject */
	virtual REBOOL isEqual(REObject * anotherObject);
	
	/// Getter of default NULL object.
	static RENULLObject * NULLObject();
};

#endif /* RENULLObject */

