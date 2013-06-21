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


#include "../../include/recore/REUserCallBacks.h"

__RE_PUBLIC_CLASS_API__ REUserCallBacks::UserCallBacks REUserCallBacks::_callBacks = { 0 };

void REUserCallBacks::setCreateAudioPlayer(REInt32 (*CreateAudioPlayer)(const char *))
{
	_callBacks.CreateAudioPlayer = CreateAudioPlayer;
}
REBOOL REUserCallBacks::isCreateAudioPlayer()
{
	if (_callBacks.CreateAudioPlayer) { return true; }
	return false;
}
REInt32 REUserCallBacks::callCreateAudioPlayer(const char * filePath)
{
	if (_callBacks.CreateAudioPlayer && filePath) 
	{
		return _callBacks.CreateAudioPlayer(filePath);
	}
	return -1;
}

void REUserCallBacks::setSendAudioPlayerMessage(REInt32 (*SendAudioPlayerMessage)(REInt32, REInt32, REInt32))
{
	_callBacks.SendAudioPlayerMessage = SendAudioPlayerMessage;
}
REBOOL REUserCallBacks::isSendAudioPlayerMessage()
{
	if (_callBacks.SendAudioPlayerMessage) { return true; }
	return false;
}
REInt32 REUserCallBacks::callSendAudioPlayerMessage(REInt32 audioPlayerID, REInt32 message, REInt32 param)
{
	if (_callBacks.SendAudioPlayerMessage && (audioPlayerID >= 0) ) 
	{
		return _callBacks.SendAudioPlayerMessage(audioPlayerID, message, param);
	}
	return -1;
}

REUserCallBacks::REUserCallBacks()
{
	
}

REUserCallBacks::~REUserCallBacks()
{
	
}

