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


#ifndef __RETEXTFIELD_H__
#define __RETEXTFIELD_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REMainLoopUpdatable.h"
#include "RELabel.h"
#include "IRETextInputResponder.h"
#include "RETextInputRespondersManager.h"

/// Class using for displaying text.
class __RE_PUBLIC_CLASS_API__ RETextField : public RELabel, public REMainLoopUpdatable, public IRETextInputResponder
{
protected:
	REView * _cursorView;
	RETimeInterval _cursorBlinkTime;
	RETimeInterval _lastBlinkTime;
	REUInt32 _maximumInputedTextLength;
	
	REBOOL _isHasMaximumInputedTextLength;
	REBOOL _isClearPreviousTextInputText;
	REBOOL _isShowCursor;
	REBOOL _isActualShowCursor;
	
	RETextField();
	virtual ~RETextField();
public:
	/* IREUserActionResponder */
	virtual void userActionClickDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									   const REFloat32 currentCoordX, const REFloat32 currentCoordY);
	
	/* REMainLoopUpdatable */
	/// Called from main thread with current time in seconds.
	virtual void update(const RETimeInterval currentTime);
	
	/* REObject */
	virtual void onReleased();
	
	/* IRERenderable */
	virtual void render();
	virtual void renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
	/* IRETextInputResponder */
	virtual const REUIdentifier getTextInputResponderObjectIdentifier() const { return this->objectIdentifier(); }
	virtual void onTextInputResponderTextInputStarted();
	virtual void onTextInputResponderTextInputEnded();
	virtual void onTextInputResponderTextChanged(const REString & newTextString);
	virtual REBOOL becomeFirstTextInputResponder();
	virtual REBOOL resignFirstTextInputResponder();
	virtual const REUInt32 getTextInputResponderMaximumTextLength() const;
	virtual REBOOL isTextInputResponderHasMaximumTextLength() const;
	virtual REBOOL isClearPreviousInputedText() const;
	virtual const REString & getTextInputResponderText() const;
	
	void setMaximumInputedTextLength(const REUInt32 newMaxInputLen);
	void setClearPreviousInputedText(REBOOL isClear);
	void setHasMaximumInputedTextLength(REBOOL isHasMaxTextLen);
	
	/// Returns cursor view.
	REView * getCursorView() const;
	
	/// Setting rectangular frame of view.
	virtual void SetFrame(const RERect & newViewFrame);
	
	/// Is text input cursor showing
	REBOOL isShowCursor() const;
	
	/// Set is need to show text input cursor.
	void setShowCursor(REBOOL isShow);
	
	/// Returns time in seconds of cursor blinking.
	const RETimeInterval getCursorBlinkTime() const;
	
	/// Setting time in seconds of cursor blinking. 
	void setCursorBlinkTime(const RETimeInterval newBlinkTime);
	
	/// Creates and return new text field object.
	static RETextField * create();
};



#endif /* __RETEXTFIELD_H__ */


