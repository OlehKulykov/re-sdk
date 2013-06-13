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


#include "../../include/regui/RETextField.h"


#define RE_TEXTFIELD_XML_CURSOR_BLINK_TIME_KEY_STRING "cursorblinktime"
#define RE_TEXTFIELD_XML_CURSOR_BLINK_TIME_FORMAT_STRING "%f"
#define RE_TEXTFIELD_XML_MAX_TEXT_LEN_KEY_STRING "maxtextlen"
#define RE_TEXTFIELD_XML_MAX_TEXT_LEN_FORMAT_STRING "%i"
#define RE_TEXTFIELD_XML_HAS_MAX_TEXT_LEN_KEY_STRING "hasmaxtextlen"
#define RE_TEXTFIELD_XML_HAS_MAX_TEXT_LEN_FORMAT_STRING "%i"
#define RE_TEXTFIELD_XML_CLEAR_PREV_TEXT_KEY_STRING "clearprevtext"
#define RE_TEXTFIELD_XML_CLEAR_PREV_TEXT_FORMAT_STRING "%i"
#define RE_TEXTFIELD_XML_SHOW_CURSOR_KEY_STRING "showcursor"
#define RE_TEXTFIELD_XML_SHOW_CURSOR_FORMAT_STRING "%i"

REBOOL RETextField::AcceptTextFieldStringParameter(RETextField * textField, const char * key, const char * value)
{
    if (strcmp(key, RE_TEXTFIELD_XML_CURSOR_BLINK_TIME_KEY_STRING) == 0)
    {
        float v = 0.0f;
        if (sscanf(value, RE_TEXTFIELD_XML_CURSOR_BLINK_TIME_FORMAT_STRING, &v) == 1)
        {
            textField->SetCursorBlinkTime((v > 0.0f) ? (RETimeInterval)v : 0.0);
            return true;
        }
    }
    else if (strcmp(key, RE_TEXTFIELD_XML_MAX_TEXT_LEN_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_TEXTFIELD_XML_MAX_TEXT_LEN_FORMAT_STRING, &v) == 1)
        {
            textField->SetMaximumInputedTextLength((v >= 0) ? (REUInt32)v : 0);
            return true;
        }
    }
    else if (strcmp(key, RE_TEXTFIELD_XML_HAS_MAX_TEXT_LEN_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_TEXTFIELD_XML_HAS_MAX_TEXT_LEN_FORMAT_STRING, &v) == 1)
        {
            textField->SetHasMaximumInputedTextLength((v != 0));
            return true;
        }
    }
    else if (strcmp(key, RE_TEXTFIELD_XML_CLEAR_PREV_TEXT_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_TEXTFIELD_XML_CLEAR_PREV_TEXT_FORMAT_STRING, &v) == 1)
        {
            textField->SetClearPreviousInputedText((v != 0));
            return true;
        }
    }
    else if (strcmp(key, RE_TEXTFIELD_XML_SHOW_CURSOR_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_TEXTFIELD_XML_SHOW_CURSOR_FORMAT_STRING, &v) == 1)
        {
            textField->SetShowCursor((v != 0));
            return true;
        }
    }
    return false;
}

REBOOL RETextField::AcceptStringParameter(const char * key, const char * value)
{
    if (RELabel::AcceptStringParameter(key, value))
    {
        return true;
    }

    if (key && value)
    {
        return RETextField::AcceptTextFieldStringParameter(this, key, value);
    }

    return false;
}

void RETextField::UserActionClickDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
										const REFloat32 currentCoordX, const REFloat32 currentCoordY)
{
	RERect frame(this->GetScreenFrame());
	if (frame.IsPointInRect(startCoordX, startCoordY) && frame.IsPointInRect(currentCoordX, currentCoordY)) 
	{
		RETextInputRespondersManager * m = RETextInputRespondersManager::GetDefaultManager();
		if (m) 
		{
			if (!m->IsFirstResponder(this)) 
			{
				this->BecomeFirstTextInputResponder();
				if (_isShowCursor) 
				{
					_isActualShowCursor = m->IsFirstResponder(this);
					_lastBlinkTime = 0.0f;
				}
			}
		}
	}
}

void RETextField::Update(const RETimeInterval currentTime)
{
	if ((currentTime - _lastBlinkTime) >= _cursorBlinkTime) 
	{
		if (_cursorView)
		{
			if (REAnimation::Setup(NULL))
			{
				REAnimation::SetTime((_cursorBlinkTime / 8.0));				
				if (_cursorView->GetAlpha() < 0.1f) 
				{
					_cursorView->SetAlphaAnimated(1.0f);
				}
				else 
				{
					_cursorView->SetAlphaAnimated(0.0f);
				}
				REAnimation::Execute();
			}
		}
		_lastBlinkTime = currentTime;
	}
}

void RETextField::Render()
{
	if (_cursorView) 
	{
		_cursorView->SetVisible(_isActualShowCursor);
		if (_cursorView->IsVisible()) 
		{
			RERect textFrame(this->GetTextFrame());
			RERect frame(_cursorView->GetFrame());
			frame.x = textFrame.x + textFrame.width;
			_cursorView->SetFrame(frame);
		}
	}
	
	RELabel::Render();
}

void RETextField::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	if (_cursorView) 
	{
		_cursorView->SetVisible(_isActualShowCursor);
		if (_cursorView->IsVisible()) 
		{
			RERect textFrame(this->GetTextFrame());
			RERect frame(_cursorView->GetFrame());
			frame.x = textFrame.x + textFrame.width;
			
			_cursorView->SetFrame(frame);
		}
	}
	
	RELabel::RenderWithOffset(offsetX, offsetY);
}

/// Setting rectangular frame of view.
void RETextField::SetFrame(const RERect & newViewFrame)
{
	RELabel::SetFrame(newViewFrame);
	
	if (_cursorView) 
	{
		const REFloat32 w2px = 2.0f * RERenderDevice::GetDefaultDevice()->GetScreenToRenderSizeRatio().width;
		_cursorView->SetFrame(RERect(0.0f, 0.0f, w2px, newViewFrame.height));
	}
}

void RETextField::OnTextInputResponderTextInputStarted()
{
	
}

void RETextField::OnTextInputResponderTextInputEnded()
{
	_isActualShowCursor = false;
}

void RETextField::OnTextInputResponderTextChanged(const REString & newTextString)
{
	this->SetText(newTextString);
}

REBOOL RETextField::BecomeFirstTextInputResponder()
{
	RETextInputRespondersManager * m = RETextInputRespondersManager::GetDefaultManager();
	if (m) 
	{
		return m->SetFirstResponder(this);
	}
	return false;
}

REBOOL RETextField::ResignFirstTextInputResponder()
{
	RETextInputRespondersManager * m = RETextInputRespondersManager::GetDefaultManager();
	if (m) 
	{
		return m->RemoveFirstResponder(this);
	}
	return false;
}

const REUInt32 RETextField::GetTextInputResponderMaximumTextLength() const
{
	return _maximumInputedTextLength;
}

REBOOL RETextField::IsTextInputResponderHasMaximumTextLength() const
{
	return _isHasMaximumInputedTextLength;
}

REBOOL RETextField::IsClearPreviousInputedText() const
{
	return _isClearPreviousTextInputText;
}

const REString & RETextField::GetTextInputResponderText() const
{
	return _text;
}

void RETextField::SetMaximumInputedTextLength(const REUInt32 newMaxInputLen)
{
	_maximumInputedTextLength = newMaxInputLen;
}

void RETextField::SetClearPreviousInputedText(REBOOL isClear)
{
	_isClearPreviousTextInputText = isClear;
}

void RETextField::SetHasMaximumInputedTextLength(REBOOL isHasMaxTextLen)
{
	_isHasMaximumInputedTextLength = isHasMaxTextLen;
}

/// Is text input cursor showing
REBOOL RETextField::IsShowCursor() const
{
	return _isShowCursor;
}

/// Set is need to show text input cursor.
void RETextField::SetShowCursor(REBOOL isShow)
{
	if (_isShowCursor != isShow) 
	{
		_lastBlinkTime = 0.0;
		if (isShow) 
		{
			this->AddToMainLoop();
		}
		else
		{
			this->RemoveFromMainLoop();
		}
	}
	_isShowCursor = isShow;
	_isActualShowCursor = false;
	if (isShow) 
	{
		RETextInputRespondersManager * m = RETextInputRespondersManager::GetDefaultManager();
		if (m) 
		{
			_isActualShowCursor = m->IsFirstResponder(this);
			_lastBlinkTime = 0.0;
		}
	}
}

/// Returns time in seconds of cursor blinking.
const RETimeInterval RETextField::GetCursorBlinkTime() const
{
	return _cursorBlinkTime;
}

/// Setting time in seconds of cursor blinking. 
void RETextField::SetCursorBlinkTime(const RETimeInterval newBlinkTime)
{
	_cursorBlinkTime = newBlinkTime;
}

REView * RETextField::GetCursorView() const
{
	return _cursorView;
}

/* REObject */
const REUInt32 RETextField::GetClassIdentifier() const
{
	return RETextField::ClassIdentifier();
}

const REUInt32 RETextField::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("RETextField");
	return clasIdentif;
}

REBOOL RETextField::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((RETextField::ClassIdentifier() == classIdentifier) ||
			(REObject::GenerateClassIdentifierFromClassName("IRETextInputResponder") == classIdentifier) ||
			(REObject::GenerateClassIdentifierFromClassName("REMainLoopUpdatable") == classIdentifier) ||
			RELabel::IsImplementsClass(classIdentifier));
}

void RETextField::OnReleased()
{
	this->RemoveFromMainLoop();
	
	RETextInputRespondersManager * m = RETextInputRespondersManager::GetDefaultManager();
	if (m) 
	{
		m->UnRegisterResponder(this);
	}
	
	if (_cursorView) 
	{
		if (_cursorView->IsAnimating()) 
		{
			_cursorView->StopAnimation(REAnimationStopTypeImmediately,true, false);
		}
		_cursorView->Release();
		_cursorView = NULL;
	}
	
	RELabel::OnReleased();
}

RETextField::RETextField() : RELabel(),
	_cursorView(NULL),
	_cursorBlinkTime(0.6),
	_lastBlinkTime(0.0),
	_maximumInputedTextLength(0),
	_isHasMaximumInputedTextLength(false),
	_isClearPreviousTextInputText(false),
	_isShowCursor(false),
	_isActualShowCursor(false)
{
    REView * cursorView = REView::Create();
	if (cursorView) 
	{
		if (this->AddSubView(cursorView)) 
		{
			_cursorView = cursorView;
            _cursorView->SetColor(REColor(0.0f, 0.0f, 0.0f, 1.0f));
		}
		else 
		{
            REObject::Delete(cursorView);
		}
	}

	RETextInputRespondersManager * m = RETextInputRespondersManager::GetDefaultManager();
	if (m) 
	{
		m->RegisterResponder(this);
	}
	
	this->SetShowCursor(true);
	this->SetRespondsForUserAction(true);
}

RETextField::~RETextField()
{

}

RETextField * RETextField::Create()
{
	RETextField * tf = new RETextField();
	return tf;
}

const char * RETextField::GetXMLCursorBlinkTimeKeyString() { return RE_TEXTFIELD_XML_CURSOR_BLINK_TIME_KEY_STRING; }
const char * RETextField::GetXMLCursorBlinkTimeFormatString() { return RE_TEXTFIELD_XML_CURSOR_BLINK_TIME_FORMAT_STRING; }
const char * RETextField::GetXMLMaximumTextLengthKeyString() { return RE_TEXTFIELD_XML_MAX_TEXT_LEN_KEY_STRING; }
const char * RETextField::GetXMLMaximumTextLengthFormatString() { return RE_TEXTFIELD_XML_MAX_TEXT_LEN_FORMAT_STRING; }
const char * RETextField::GetXMLHasMaximumTextLengthKeyString() { return RE_TEXTFIELD_XML_HAS_MAX_TEXT_LEN_KEY_STRING; }
const char * RETextField::GetXMLHasMaximumTextLengthFormatString() { return RE_TEXTFIELD_XML_HAS_MAX_TEXT_LEN_FORMAT_STRING; }
const char * RETextField::GetXMLClearPreviousTextKeyString() { return RE_TEXTFIELD_XML_CLEAR_PREV_TEXT_KEY_STRING; }
const char * RETextField::GetXMLClearPreviousTextFormatString() { return RE_TEXTFIELD_XML_CLEAR_PREV_TEXT_FORMAT_STRING; }
const char * RETextField::GetXMLShowCursorKeyString() { return RE_TEXTFIELD_XML_SHOW_CURSOR_KEY_STRING; }
const char * RETextField::GetXMLShowCursorFormatString() { return RE_TEXTFIELD_XML_SHOW_CURSOR_FORMAT_STRING; }

