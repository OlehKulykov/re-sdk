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


#include "../../include/regui/RETextInputRespondersManager.h"

__RE_PUBLIC_CLASS_API__ RETextInputRespondersManager * RETextInputRespondersManager::_defaulTextInputRespondersManager = NULL;

void RETextInputRespondersManager::SetFirstState(RETextInputRespondersManager::Responder * resp, REBOOL isFirst)
{
	if (resp->isFirst != isFirst) 
	{
		if (resp->isFirst) 
		{
			if (_callBacks.EndTextInputCallBack) 
			{
				if (_callBacks.EndTextInputCallBack())
				{
					resp->object->OnTextInputResponderTextInputEnded();
				}
			}
		}
		else
		{
			if (_callBacks.StartUTF8TextInputCallBack)
			{
				if (_callBacks.StartUTF8TextInputCallBack(resp->object->GetTextInputResponderText().UTF8String()))
				{
					resp->object->OnTextInputResponderTextInputStarted();
				}
			}
			else if (_callBacks.StartWideTextInputCallBack)
			{
				REStringPresentation p(resp->object->GetTextInputResponderText());
				if (_callBacks.StartWideTextInputCallBack(p.WideString()))
				{
					resp->object->OnTextInputResponderTextInputStarted();
				}
			}
		}
		resp->isFirst = isFirst;
	}
}

/// Called when text changed. Wide string version.
/// Returns accepted wide string and length if defined pointer to 'acceptedLength' value.
REUInt32 RETextInputRespondersManager::AcceptNewWideText(const wchar_t * newText)
{
	REUInt32 r = 0;
	_mutex.Lock();
	if (_firstResponder) 
	{
		REUInt32 needLen = newText ? (REUInt32)wcslen(newText) : 0;
		if (_firstResponder->object->IsTextInputResponderHasMaximumTextLength()) 
		{
			const REUInt32 maxLen = _firstResponder->object->GetTextInputResponderMaximumTextLength();
			if (needLen > maxLen) { needLen = maxLen; }
		}
		
		REString t(newText, needLen);
		r = needLen;
		_firstResponder->object->OnTextInputResponderTextChanged(t);
	}
	_mutex.Unlock();
	return r;
}

/// Called when text changed. Plaine C string version.
/// Returns accepted wide string and length if defined pointer to 'acceptedLength' value.
REUInt32 RETextInputRespondersManager::AcceptNewUTF8Text(const char * newText)
{
	_mutex.Lock();
	REUInt32 r = 0;
	
	REStringPresentation newTextP(newText);
	r = this->AcceptNewWideText(newTextP.WideString());
	
	_mutex.Unlock();
	return r;
}

/// User side action. Called when text input ended/cancelled by user.
void RETextInputRespondersManager::OnTextInputEnded()
{
	_mutex.Lock();
	if (_firstResponder) 
	{
		_firstResponder->object->OnTextInputResponderTextInputEnded();
		_firstResponder->isFirst = false;
		_firstResponder = NULL;
	}
	_mutex.Unlock();
}

void RETextInputRespondersManager::SetStartWideTextInputCallBack(int (*StartWideTextInputCallBack)(const wchar_t *))
{
	_callBacks.StartWideTextInputCallBack = StartWideTextInputCallBack;
}

void RETextInputRespondersManager::SetStartUTF8TextInputCallBack(int (*StartUTF8TextInputCallBack)(const char *))
{
	_callBacks.StartUTF8TextInputCallBack = StartUTF8TextInputCallBack;
}

void RETextInputRespondersManager::SetEndTextInputCallBack(int (*EndTextInputCallBack)())
{
	_callBacks.EndTextInputCallBack = EndTextInputCallBack;
}

RETextInputRespondersManager::Responder * RETextInputRespondersManager::GetResponderForObjectIdentifier(const REUIdentifier respObjectId, REUInt32 * index)
{
	_mutex.Lock();
	for (REUInt32 i = 0; i < _responders.Count(); i++) 
	{
		RETextInputRespondersManager::Responder * resp = _responders[i];
		if (respObjectId == resp->object->GetTextInputResponderObjectIdentifier()) 
		{
			if (index) { (*index) = i; }
			_mutex.Unlock();
			return resp;
		}
	}
	_mutex.Unlock();
	return NULL;
}

void RETextInputRespondersManager::Clear()
{
	_mutex.Lock();
	for (REUInt32 i = 0; i < _responders.Count(); i++) 
	{
		RETextInputRespondersManager::Responder * resp = _responders[i];
		resp->Release();
	}
	_responders.Clear();
	_mutex.Unlock();
}

/// Register responder. Can call in contructor of IRETextInputResponder object.
/// If not it's will register when try to become first responder and not registered yet.
REBOOL RETextInputRespondersManager::RegisterResponder(IRETextInputResponder * responder)
{
	_mutex.Lock();
	if (responder) 
	{
		RETextInputRespondersManager::Responder * resp = this->GetResponderForObjectIdentifier(responder->GetTextInputResponderObjectIdentifier(), NULL);
		if (resp) 
		{
			_mutex.Unlock();
			return true;
		}
		
		resp = new RETextInputRespondersManager::Responder(responder);
		if (resp) 
		{
			REBOOL isAdded = false;
			if (_responders.Add(resp)) { isAdded = true; }
			else { delete resp; }
			_mutex.Unlock();
			return isAdded;
		}
	}
	_mutex.Unlock();
	return false;
}

/// Unregister responder. Can call in desctructor of IRETextInputResponder object.
REBOOL RETextInputRespondersManager::UnRegisterResponder(IRETextInputResponder * responder)
{
	_mutex.Lock();
	if (responder) 
	{
		REUInt32 index = 0;
		RETextInputRespondersManager::Responder * resp = this->GetResponderForObjectIdentifier(responder->GetTextInputResponderObjectIdentifier(), &index);
		if (resp) 
		{
			REBOOL isRemoved = false;
			if (_responders.RemoveAt(index)) 
			{
				resp->Release();
				isRemoved = true;
			}
			_mutex.Unlock();
			return isRemoved;
		}
	}
	_mutex.Unlock();
	return false;
}

/// Set object as first responder.
REBOOL RETextInputRespondersManager::SetFirstResponder(IRETextInputResponder * newFirstResponder)
{
	_mutex.Lock();
	if (this->IsFirstResponder(newFirstResponder)) 
	{
		_mutex.Unlock();
		return true;
	}
	
	if (newFirstResponder) 
	{
		_firstResponder = NULL;
		REBOOL isFound = false;
		REUInt32 foundIndex = 0;
		const REUIdentifier respObjectId = newFirstResponder->GetTextInputResponderObjectIdentifier();
		for (REUInt32 i = 0; i < _responders.Count(); i++) 
		{
			RETextInputRespondersManager::Responder * resp = _responders[i];
			this->SetFirstState(resp, false);
			if (respObjectId == resp->object->GetTextInputResponderObjectIdentifier()) 
			{
				isFound = true;
				foundIndex = i;
				_firstResponder = NULL;
			}
		}
		
		if (isFound) 
		{
			RETextInputRespondersManager::Responder * resp = _responders[foundIndex];
			this->SetFirstState(resp, true);
			_firstResponder = resp;
		}
		
		_mutex.Unlock();
		return isFound;
	}
	_mutex.Unlock();
	return false;
}

/// Remove object from first responders stack.
REBOOL RETextInputRespondersManager::RemoveFirstResponder(IRETextInputResponder * firstResponder)
{
	_mutex.Lock();
	if (firstResponder) 
	{
		if (this->IsFirstResponder(firstResponder)) 
		{
			const REUIdentifier respObjectId = firstResponder->GetTextInputResponderObjectIdentifier();
			for (REUInt32 i = 0; i < _responders.Count(); i++) 
			{
				RETextInputRespondersManager::Responder * resp = _responders[i];
				if (respObjectId == resp->object->GetTextInputResponderObjectIdentifier()) 
				{
					this->SetFirstState(resp, false);
					if (_firstResponder) 
					{
						if (respObjectId == _firstResponder->object->GetTextInputResponderObjectIdentifier()) 
						{
							_firstResponder = NULL;
						}
					}
					_mutex.Unlock();
					return true;
				}
			}
		}
		else 
		{
			_mutex.Unlock();
			return true;
		}
	}
	_mutex.Unlock();
	return false;
}

/// Check 'responder' is first.
REBOOL RETextInputRespondersManager::IsFirstResponder(IRETextInputResponder * responder)
{
	_mutex.Lock();
	if (responder && _firstResponder) 
	{
		if (_firstResponder->object->GetTextInputResponderObjectIdentifier() ==
			responder->GetTextInputResponderObjectIdentifier()) 
		{
			_mutex.Unlock();
			return true;
		}
	}
	_mutex.Unlock();
	return false;
}

RETextInputRespondersManager::RETextInputRespondersManager() :
	_firstResponder(NULL)
{
	_mutex.Init(REMutexTypeRecursive);
	memset(&_callBacks, 0, sizeof(RETextInputRespondersManager::CallBacksStruct));
}

RETextInputRespondersManager::~RETextInputRespondersManager()
{
	this->Clear();
}

RETextInputRespondersManager * RETextInputRespondersManager::GetDefaultManager()
{
	if (_defaulTextInputRespondersManager == NULL) 
	{
		_defaulTextInputRespondersManager = new RETextInputRespondersManager();
	}
	return _defaulTextInputRespondersManager;
}

void RETextInputRespondersManager::ReleaseDefaultManager()
{
	if (_defaulTextInputRespondersManager) 
	{
		delete _defaulTextInputRespondersManager;
		_defaulTextInputRespondersManager = NULL;
	}
}



