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


#ifndef __IRETEXTINPUTRESPONDER_H__
#define __IRETEXTINPUTRESPONDER_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REString.h"

/// Class interface for classes that can responce user text input.
class __RE_PUBLIC_CLASS_API__ IRETextInputResponder
{
public:
	/// Inform object that text input started.
	virtual void onTextInputResponderTextInputStarted() = 0;
	
	/// Inform object that text input ended.
	virtual void onTextInputResponderTextInputEnded() = 0;
	
	/// Called when text of responder changed.
	virtual void onTextInputResponderTextChanged(const REString & newTextString) = 0;
	
	/// Returns object identifier.
	virtual const REUIdentifier getTextInputResponderObjectIdentifier() const = 0;
	
	/// Tell object to become first responder.
	virtual REBOOL becomeFirstTextInputResponder() = 0;
	
	/// Tell object to become first responder.
	virtual REBOOL resignFirstTextInputResponder() = 0;
	
	/// Returns maximum accepted text length.
	virtual const REUInt32 getTextInputResponderMaximumTextLength() const = 0;
	
	/// Ask responder for using text input limit for maximum text length.
	virtual REBOOL isTextInputResponderHasMaximumTextLength() const = 0;
	
	/// Ask responder is need to clear previous inputed text after becom first responder.
	virtual REBOOL isClearPreviousInputedText() const = 0;
	
	/// Ask responder for inputed text.
	virtual const REString & getTextInputResponderText() const = 0;
	
	/// Destructor.
	virtual ~IRETextInputResponder() { }
};


#endif /* __IRETEXTINPUTRESPONDER_H__ */


