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


#ifndef __REUSERCALLBACKS_H__
#define __REUSERCALLBACKS_H__


#include "RECommonHeader.h"
#include "REBuffer.h"
#include "REString.h"
#include "REObject.h"
#include "REArray.h"

class __RE_PUBLIC_CLASS_API__ REUserCallBacks
{
private:
	typedef struct _userCallBacks
	{
		REInt32 (*CreateAudioPlayer)(const char * filePath);
		REInt32 (*SendAudioPlayerMessage)(REInt32 audioPlayerID, REInt32 message, REInt32 param);
	} UserCallBacks;
	static UserCallBacks _callBacks;
	 
public:
	void SetCreateAudioPlayer(REInt32 (*CreateAudioPlayer)(const char *));
	REBOOL IsCreateAudioPlayer();
	REInt32 CallCreateAudioPlayer(const char * filePath);
	
	void SetSendAudioPlayerMessage(REInt32 (*SendAudioPlayerMessage)(REInt32, REInt32, REInt32));
	REBOOL IsSendAudioPlayerMessage();
	REInt32 CallSendAudioPlayerMessage(REInt32 audioPlayerID, REInt32 message, REInt32 param);
	
	REUserCallBacks();
	~REUserCallBacks();
};

#endif /* __REUSERCALLBACKS_H__ */

