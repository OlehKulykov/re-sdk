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
class __RE_PUBLIC_CLASS_API__ RETextField : public RELabel, public IRETextInputResponder, public REMainLoopUpdatable
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
private:
    static REBOOL AcceptTextFieldStringParameter(RETextField * textField, const char * key, const char * value);
public:
    /* REGUIObject */
    /// Acepting string parameter and value string presentation from XML document.
    /// Using when view controller loading from XML. This method can be invoked from non-main thread if view controller loading in background.
    virtual REBOOL AcceptStringParameter(const char * key, const char * value);

	/* IREUserActionResponder */
	virtual void UserActionClickDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									   const REFloat32 currentCoordX, const REFloat32 currentCoordY);
	
	/* REMainLoopUpdatable */
	/// Called from main thread with current time in seconds.
	virtual void Update(const RETimeInterval currentTime);
	
	/// Must return unique identifier of object. Usialy returns 'REObject::GetObjectIdentifier()'.
	virtual const REUIdentifier GetMainLoopUpdatableIdentifier() const { return this->GetObjectIdentifier(); }
	
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	/* IRERenderable */
	virtual void Render();
	virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
	/* IRETextInputResponder */
	virtual const REUIdentifier GetTextInputResponderObjectIdentifier() const { return this->GetObjectIdentifier(); }
	virtual void OnTextInputResponderTextInputStarted();
	virtual void OnTextInputResponderTextInputEnded();
	virtual void OnTextInputResponderTextChanged(const REString & newTextString);
	virtual REBOOL BecomeFirstTextInputResponder();
	virtual REBOOL ResignFirstTextInputResponder();
	virtual const REUInt32 GetTextInputResponderMaximumTextLength() const;
	virtual REBOOL IsTextInputResponderHasMaximumTextLength() const;
	virtual REBOOL IsClearPreviousInputedText() const;
	virtual const REString & GetTextInputResponderText() const;
	
	void SetMaximumInputedTextLength(const REUInt32 newMaxInputLen);
	void SetClearPreviousInputedText(REBOOL isClear);
	void SetHasMaximumInputedTextLength(REBOOL isHasMaxTextLen);
	
	/// Returns cursor view.
	REView * GetCursorView() const;
	
	/// Setting rectangular frame of view.
	virtual void SetFrame(const RERect & newViewFrame);
	
	/// Is text input cursor showing
	REBOOL IsShowCursor() const;
	
	/// Set is need to show text input cursor.
	void SetShowCursor(REBOOL isShow);
	
	/// Returns time in seconds of cursor blinking.
	const RETimeInterval GetCursorBlinkTime() const;
	
	/// Setting time in seconds of cursor blinking. 
	void SetCursorBlinkTime(const RETimeInterval newBlinkTime);
	
	/// Creates and return new text field object.
	static RETextField * Create();

	/// Returns XML key string for cursor blink time.
    static const char * GetXMLCursorBlinkTimeKeyString();
	
	/// Returns XML format string for cursor blink time.
    static const char * GetXMLCursorBlinkTimeFormatString();
	
	/// Returns XML key string for maximum text length.
    static const char * GetXMLMaximumTextLengthKeyString();
	
	/// Returns XML format string for maximum text length.
    static const char * GetXMLMaximumTextLengthFormatString();
	
	/// Returns XML key string for flag has maximum text.
    static const char * GetXMLHasMaximumTextLengthKeyString();
	
	/// Returns XML format string for flag has maximum text.
    static const char * GetXMLHasMaximumTextLengthFormatString();
	
	/// Returns XML key string for flag clear previous text.
    static const char * GetXMLClearPreviousTextKeyString();
	
	/// Returns XML format string for flag clear previous text.
    static const char * GetXMLClearPreviousTextFormatString();
	
	/// Returns XML key string for flag show cursor.
    static const char * GetXMLShowCursorKeyString();
	
	/// Returns XML format string for flag show cursor.
    static const char * GetXMLShowCursorFormatString();
};



#endif /* __RETEXTFIELD_H__ */


