/*
 *   Copyright 2012 - 2013 Kulikov Oleg
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


#ifndef __REAUDIOOPENAL_H__
#define __REAUDIOOPENAL_H__

#include "../recore/RECommonHeader.h"

#ifdef __RE_USING_OPENAL__

#ifdef __RE_OS_IPHONE__ 
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif /* __RE_OS_IPHONE__ */

#ifdef __RE_OS_MACOSX__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif /* __RE_OS_MACOSX__ */


class __RE_PUBLIC_CLASS_API__ REAudioOpenAL
{
private:
	static ALCdevice * openAllDevices();
	static ALCdevice * openDefaultDevice();
	static ALCdevice * openDevice();
	static REBOOL initDevice();
	
protected:	
	static ALCdevice * _alDevice;
	static ALCcontext * _alContext;
	
public:
	REAudioOpenAL();
	virtual ~REAudioOpenAL();
};




#endif /* __RE_USING_OPENAL__ */

#endif /* __REAUDIOOPENAL_H__ */

