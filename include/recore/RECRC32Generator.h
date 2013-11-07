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


#ifndef __RECRC32GENERATOR_H__
#define __RECRC32GENERATOR_H__

#include "RECommonHeader.h"

/// Class generator of CRC32 values.
class __RE_PUBLIC_CLASS_API__ RECRC32Generator 
{
public:
	static const REUInt32 generateFromData(const void * data, const REUInt32 dataLength);
	
	static const REUInt32 generateFromString(const char * someString, const REUInt32 stringLenght = RENotFound);
};


#endif /* __RECRC32GENERATOR_H__ */


