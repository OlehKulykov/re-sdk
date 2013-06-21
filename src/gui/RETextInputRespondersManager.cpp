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


#include "../../include/regui/RETextInputRespondersManager.h"

__RE_PUBLIC_CLASS_API__ RETextInputRespondersManager * RETextInputRespondersManager::_defaulTextInputRespondersManager = NULL;

void RETextInputRespondersManager::setFirstState(RETextInputRespondersManager::Responder * resp, REBOOL isFirst)
{
	if (resp->isFirst != isFirst) 
	{
		if (resp->isFirst) 
		{
			if (_callBacks.EndTextInputCallBack) 
			{
				if (_callBacks.EndTextInputCallBack())
				{
					resp->object->onTextInputResponderTextInputEnded();
				}
			}
		}
		else
		{
			if (_callBacks.StartUTF8TextInputCallBack)
			{
				if (_callBacks.StartUTF8TextInputCallBack(resp->object->getTextInputResponderText().UTF8String()))
				{
					resp->object->onTextInputResponderTextInputStarted();
				}
			}
			else if (_callBacks.StartWideTextInputCallBack)
			{
				REStringPresentation p(resp->object->getTextInputResponderText());
				if (_callBacks.StartWideTextInputCallBack(p.wideString()))
				{
					resp->object->onTextInputResponderTextInputStarted();
				}
			}
		}
		resp->isFirst = isFirst;
	}
}

/// Called when text changed. Wide string version.
/// Returns accepted wide string and length if defined pointer to 'acceptedLength' value.
REUInt32 RETextInputRespondersManager::acceptNewWideText(const wchar_t * newText)
{
	REUInt32 r = 0;
	_mutex.lock();
	if (_firstResponder) 
	{
		REUInt32 needLen = newText ? (REUInt32)wcslen(newText) : 0;
		if (_firstResponder->object->isTextInputResponderHasMaximumTextLength()) 
		{
			const REUInt32 maxLen = _firstResponder->object->getTextInputResponderMaximumTextLength();
			if (needLen > maxLen) { needLen = maxLen; }
		}
		
		REString t(newText, needLen);
		r = needLen;
		_firstResponder->object->onTextInputResponderTextChanged(t);
	}
	_mutex.unlock();
	return r;
}

/// Called when text changed. Plaine C string version.
/// Returns accepted wide string and length if defined pointer to 'acceptedLength' value.
REUInt32 RETextInputRespondersManager::acceptNewUTF8Text(const char * newText)
{
	_mutex.lock();
	REUInt32 r = 0;
	
	REStringPresentation newTextP(newText);
	r = this->acceptNewWideText(newTextP.wideString());
	
	_mutex.unlock();
	return r;
}

/// User side action. Called when text input ended/cancelled by user.
void RETextInputRespondersManager::onTextInputEnded()
{
	_mutex.lock();
	if (_firstResponder) 
	{
		_firstResponder->object->onTextInputResponderTextInputEnded();
		_firstResponder->isFirst = false;
		_firstResponder = NULL;
	}
	_mutex.unlock();
}

void RETextInputRespondersManager::setStartWideTextInputCallBack(int (*StartWideTextInputCallBack)(const wchar_t *))
{
	_callBacks.StartWideTextInputCallBack = StartWideTextInputCallBack;
}

void RETextInputRespondersManager::setStartUTF8TextInputCallBack(int (*StartUTF8TextInputCallBack)(const char *))
{
	_callBacks.StartUTF8TextInputCallBack = StartUTF8TextInputCallBack;
}

void RETextInputRespondersManager::setEndTextInputCallBack(int (*EndTextInputCallBack)())
{
	_callBacks.EndTextInputCallBack = EndTextInputCallBack;
}

RETextInputRespondersManager::Responder * RETextInputRespondersManager::getResponderForObjectIdentifier(const REUIdentifier respObjectId, REUInt32 * index)
{
	_mutex.lock();
	for (REUInt32 i = 0; i < _responders.count(); i++) 
	{
		RETextInputRespondersManager::Responder * resp = _responders[i];
		if (respObjectId == resp->object->getTextInputResponderObjectIdentifier()) 
		{
			if (index) { (*index) = i; }
			_mutex.unlock();
			return resp;
		}
	}
	_mutex.unlock();
	return NULL;
}

void RETextInputRespondersManager::clear()
{
	_mutex.lock();
	for (REUInt32 i = 0; i < _responders.count(); i++) 
	{
		RETextInputRespondersManager::Responder * resp = _responders[i];
		resp->release();
	}
	_responders.clear();
	_mutex.unlock();
}

/// Register responder. Can call in contructor of IRETextInputResponder object.
/// If not it's will register when try to become first responder and not registered yet.
REBOOL RETextInputRespondersManager::registerResponder(IRETextInputResponder * responder)
{
	_mutex.lock();
	if (responder) 
	{
		RETextInputRespondersManager::Responder * resp = this->getResponderForObjectIdentifier(responder->getTextInputResponderObjectIdentifier(), NULL);
		if (resp) 
		{
			_mutex.unlock();
			return true;
		}
		
		resp = new RETextInputRespondersManager::Responder(responder);
		if (resp) 
		{
			REBOOL isAdded = false;
			if (_responders.add(resp)) { isAdded = true; }
			else { delete resp; }
			_mutex.unlock();
			return isAdded;
		}
	}
	_mutex.unlock();
	return false;
}

/// Unregister responder. Can call in desctructor of IRETextInputResponder object.
REBOOL RETextInputRespondersManager::unRegisterResponder(IRETextInputResponder * responder)
{
	_mutex.lock();
	if (responder) 
	{
		REUInt32 index = 0;
		RETextInputRespondersManager::Responder * resp = this->getResponderForObjectIdentifier(responder->getTextInputResponderObjectIdentifier(), &index);
		if (resp) 
		{
			REBOOL isRemoved = false;
			if (_responders.removeAt(index)) 
			{
				resp->release();
				isRemoved = true;
			}
			_mutex.unlock();
			return isRemoved;
		}
	}
	_mutex.unlock();
	return false;
}

/// Set object as first responder.
REBOOL RETextInputRespondersManager::setFirstResponder(IRETextInputResponder * newFirstResponder)
{
	_mutex.lock();
	if (this->isFirstResponder(newFirstResponder)) 
	{
		_mutex.unlock();
		return true;
	}
	
	if (newFirstResponder) 
	{
		_firstResponder = NULL;
		REBOOL isFound = false;
		REUInt32 foundIndex = 0;
		const REUIdentifier respObjectId = newFirstResponder->getTextInputResponderObjectIdentifier();
		for (REUInt32 i = 0; i < _responders.count(); i++) 
		{
			RETextInputRespondersManager::Responder * resp = _responders[i];
			this->setFirstState(resp, false);
			if (respObjectId == resp->object->getTextInputResponderObjectIdentifier()) 
			{
				isFound = true;
				foundIndex = i;
				_firstResponder = NULL;
			}
		}
		
		if (isFound) 
		{
			RETextInputRespondersManager::Responder * resp = _responders[foundIndex];
			this->setFirstState(resp, true);
			_firstResponder = resp;
		}
		
		_mutex.unlock();
		return isFound;
	}
	_mutex.unlock();
	return false;
}

/// Remove object from first responders stack.
REBOOL RETextInputRespondersManager::removeFirstResponder(IRETextInputResponder * firstResponder)
{
	_mutex.lock();
	if (firstResponder) 
	{
		if (this->isFirstResponder(firstResponder)) 
		{
			const REUIdentifier respObjectId = firstResponder->getTextInputResponderObjectIdentifier();
			for (REUInt32 i = 0; i < _responders.count(); i++) 
			{
				RETextInputRespondersManager::Responder * resp = _responders[i];
				if (respObjectId == resp->object->getTextInputResponderObjectIdentifier()) 
				{
					this->setFirstState(resp, false);
					if (_firstResponder) 
					{
						if (respObjectId == _firstResponder->object->getTextInputResponderObjectIdentifier()) 
						{
							_firstResponder = NULL;
						}
					}
					_mutex.unlock();
					return true;
				}
			}
		}
		else 
		{
			_mutex.unlock();
			return true;
		}
	}
	_mutex.unlock();
	return false;
}

/// Check 'responder' is first.
REBOOL RETextInputRespondersManager::isFirstResponder(IRETextInputResponder * responder)
{
	_mutex.lock();
	if (responder && _firstResponder) 
	{
		if (_firstResponder->object->getTextInputResponderObjectIdentifier() ==
			responder->getTextInputResponderObjectIdentifier()) 
		{
			_mutex.unlock();
			return true;
		}
	}
	_mutex.unlock();
	return false;
}

RETextInputRespondersManager::RETextInputRespondersManager() :
	_firstResponder(NULL)
{
	_mutex.init(REMutexTypeRecursive);
	memset(&_callBacks, 0, sizeof(RETextInputRespondersManager::CallBacksStruct));
}

RETextInputRespondersManager::~RETextInputRespondersManager()
{
	this->clear();
}

RETextInputRespondersManager * RETextInputRespondersManager::getDefaultManager()
{
	if (_defaulTextInputRespondersManager == NULL) 
	{
		_defaulTextInputRespondersManager = new RETextInputRespondersManager();
	}
	return _defaulTextInputRespondersManager;
}

void RETextInputRespondersManager::releaseDefaultManager()
{
	RE_SAFE_DELETE(_defaulTextInputRespondersManager);
}



