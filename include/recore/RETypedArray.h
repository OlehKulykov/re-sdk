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


#ifndef __RETYPEDARRAY_H__
#define __RETYPEDARRAY_H__

#include "RECommonHeader.h"
#include "REArray.h"

class RETypedPtr;

class __RE_PUBLIC_CLASS_API__ RETypedArray : public REArray<RETypedPtr>
{
protected:
	void clearTyped();

public:
	virtual void clear();
	
	RETypedArray & operator=(const RETypedArray & anotherArray);
	RETypedArray & operator=(const REArray<RETypedPtr> & anotherArray);
	
	RETypedArray(REUInt32 newCapacity = REARRAY_CAPACITY_INCREMENT);
	RETypedArray(const RETypedArray & anotherArray);
	RETypedArray(const REArray<RETypedPtr> & anotherArray);
	
	virtual ~RETypedArray();
};


#endif


