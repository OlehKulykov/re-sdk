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


#ifndef __REBUFFERNOCOPY_H__
#define __REBUFFERNOCOPY_H__

#include "REBuffer.h"

/// Class of memory buffer.
class __RE_PUBLIC_CLASS_API__ REBufferNoCopy : public REBuffer
{
public:
	typedef void(*FreeOriginalBuff)(void * mem);
	
private:
	REBufferNoCopy::FreeOriginalBuff _freeOriginalBuff;
	REBOOL _isNeedToFreeOriginalBuff;
	
protected:
	virtual void freeMemory(void * mem);
	
public:
	REBufferNoCopy(const void * originalBuff, const REUInt32 buffSize, REBufferNoCopy::FreeOriginalBuff freeOriginalBuff = REBuffer::defaultFree);
	
	virtual ~REBufferNoCopy();
};

#endif /* __REBUFFERNOCOPY_H__ */

