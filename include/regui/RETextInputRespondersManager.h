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


#ifndef __RETEXTINPUTRESPONDERSMANAGER_H__
#define __RETEXTINPUTRESPONDERSMANAGER_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REMutex.h"
#include "../recore/REArray.h"
#include "../recore/REObject.h"
#include "IRETextInputResponder.h"


/// Class interface for classes that can responce user text input.
class __RE_PUBLIC_CLASS_API__ RETextInputRespondersManager
{
private:
	class Responder : public REObject
	{
	public:
		IRETextInputResponder * object;
		REBOOL isFirst;
		Responder(IRETextInputResponder * r) : REObject(), object(r), isFirst(false) { }
		virtual ~Responder() { }
	};
	
	typedef struct _callBacksStruct
	{
		int (*StartWideTextInputCallBack)(const wchar_t * startUpText);
		int (*StartUTF8TextInputCallBack)(const char * startUpText);
		int (*EndTextInputCallBack)();
	} 
	CallBacksStruct;
	RETextInputRespondersManager::CallBacksStruct _callBacks;
	REArray<RETextInputRespondersManager::Responder *> _responders;
	REMutex _mutex;
	RETextInputRespondersManager::Responder * _firstResponder;
	void SetFirstState(RETextInputRespondersManager::Responder * resp, REBOOL isFirst);
	void Clear();
	RETextInputRespondersManager::Responder * GetResponderForObjectIdentifier(const REUIdentifier respObjectId, REUInt32 * index);
	
	static RETextInputRespondersManager * _defaulTextInputRespondersManager;
	
	RETextInputRespondersManager();
	~RETextInputRespondersManager();
public:
	/// Check 'responder' is first.
	REBOOL IsFirstResponder(IRETextInputResponder * responder);
	
	/// Register responder. Can call in contructor of IRETextInputResponder object.
	REBOOL RegisterResponder(IRETextInputResponder * responder);
	
	/// Unregister responder. Can call in desctructor of IRETextInputResponder object.
	REBOOL UnRegisterResponder(IRETextInputResponder * responder);
	
	/// Set object as first responder.
	REBOOL SetFirstResponder(IRETextInputResponder * newFirstResponder);
	
	/// Remove object from first responders stack.
	REBOOL RemoveFirstResponder(IRETextInputResponder * firstResponder);
	
	/// User side action. Called when text changed. Wide string version.
	/// Returns count of accepted characters count.
	REUInt32 AcceptNewWideText(const wchar_t * newText);
	
	/// User side action. Called when text changed. Plaine C string version.
	/// Returns count of accepted characters count. Can be different than strlen(...);
	REUInt32 AcceptNewUTF8Text(const char * newText);
	
	/// User side action. Called when text input ended/cancelled by user.
	void OnTextInputEnded();
	
	/// Setting callback for starting input text.
	/// First param is start up text, and second flag for clearing prev inputed text.
	/// Return 0 on error, otherwice non zero value.
	void SetStartWideTextInputCallBack(int (*StartWideTextInputCallBack)(const wchar_t *));
	
	/// Setting callback for starting input text.
	/// First param is start up text, and second flag for clearing prev inputed text.
	/// Return 0 on error, otherwice non zero value.
	void SetStartUTF8TextInputCallBack(int (*StartUTF8TextInputCallBack)(const char *));
	
	/// Setting callback for ending input text.
	/// Return 0 on error, otherwice non zero value.
	void SetEndTextInputCallBack(int (*EndTextInputCallBack)());
	
	static RETextInputRespondersManager * GetDefaultManager();
	static void ReleaseDefaultManager();
};


#endif /* __RETEXTINPUTRESPONDERSMANAGER_H__ */


