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


#ifndef __REBASE64_H__
#define __REBASE64_H__

#include "RECommonHeader.h"
#include "REBuffer.h"
#include "REString.h"

/// Class using for encoding/decoding data buffer to Base64 string.
class __RE_PUBLIC_CLASS_API__ REBase64 
{
public:
	/// Generates Base64 string from 'buff' data buffer.
	/// Returns true if string pointer 'base64String' is not NULL and string generated, otherwice returns false.
	REBOOL BufferToBase64String(const REBuffer & buff, REString * base64String);
	
	/// Generates 'buff' data buffer from Base64 string.
	/// Returns true if buffer pointer 'buff' is not NULL and buffer generated, otherwice returns false.
	REBOOL Base64StringToBuffer(const REString & base64String, REBuffer * buff);
	
	/// Default constructor.
	REBase64();
	
	/// Destructor.
	~REBase64();
};


#endif /* __REBASE64_H__ */

