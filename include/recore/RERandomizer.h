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


#ifndef __RERANDOMIZER_H__
#define __RERANDOMIZER_H__


#include "RECommonHeader.h"

/// Class using for generating random numbers.
class __RE_PUBLIC_CLASS_API__ RERandomizer
{
public:
	/// Return random signed integer number in range [0, 32767]
	REInt32 GetInt() const;
	
	/// Return random signed integer number in range ['lowRange', 'upRange']
	REInt32 GetIntInRange(const REInt32 lowRange, const REInt32 upRange) const;
	
	/// Return random 32-bit float number in range [0.0f, 0.9(9)f]
	REFloat32 GetFloat() const;
	
	/// Return random 32-bit float number in range ['lowRange', 'upRange']
	REFloat32 GetFloatInRange(const REFloat32 lowRange, const REFloat32 upRange) const;

	RERandomizer();
	~RERandomizer();
};

#endif /* __RERANDOMIZER_H__ */

