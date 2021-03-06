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

void RETextField::userActionClickDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
										const REFloat32 currentCoordX, const REFloat32 currentCoordY)
{
	RERect frame(this->screenFrame());
	if (frame.isPointInRect(startCoordX, startCoordY) && frame.isPointInRect(currentCoordX, currentCoordY)) 
	{
		RETextInputRespondersManager * m = RETextInputRespondersManager::getDefaultManager();
		if (m) 
		{
			if (!m->isFirstResponder(this)) 
			{
				this->becomeFirstTextInputResponder();
				if (_isShowCursor) 
				{
					_isActualShowCursor = m->isFirstResponder(this);
					_lastBlinkTime = 0.0f;
				}
			}
		}
	}
}

void RETextField::update(const RETimeInterval currentTime)
{
	if ((currentTime - _lastBlinkTime) >= _cursorBlinkTime) 
	{
		if (_cursorView)
		{
			if (REAnimation::setup(NULL))
			{
				REAnimation::setTime((_cursorBlinkTime / 8.0));				
				if (_cursorView->alpha() < 0.1f) 
				{
					_cursorView->setAlphaAnimated(1.0f);
				}
				else 
				{
					_cursorView->setAlphaAnimated(0.0f);
				}
				REAnimation::execute();
			}
		}
		_lastBlinkTime = currentTime;
	}
}

void RETextField::render()
{
	if (_cursorView) 
	{
		_cursorView->setVisible(_isActualShowCursor);
		if (_cursorView->isVisible()) 
		{
			RERect textFrame(this->getTextFrame());
			RERect frame(_cursorView->frame());
			frame.x = textFrame.x + textFrame.width;
			_cursorView->setFrame(frame);
		}
	}
	
	RELabel::render();
}

void RETextField::renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	if (_cursorView) 
	{
		_cursorView->setVisible(_isActualShowCursor);
		if (_cursorView->isVisible()) 
		{
			RERect textFrame(this->getTextFrame());
			RERect frame(_cursorView->frame());
			frame.x = textFrame.x + textFrame.width;
			
			_cursorView->setFrame(frame);
		}
	}
	
	RELabel::renderWithOffset(offsetX, offsetY);
}

/// Setting rectangular frame of view.
void RETextField::SetFrame(const RERect & newViewFrame)
{
	RELabel::setFrame(newViewFrame);
	
	if (_cursorView) 
	{
		const REFloat32 w2px = 2.0f * RERenderDevice::defaultDevice()->screenToRenderSizeRatio().width;
		_cursorView->setFrame(RERect(0.0f, 0.0f, w2px, newViewFrame.height));
	}
}

void RETextField::onTextInputResponderTextInputStarted()
{
	
}

void RETextField::onTextInputResponderTextInputEnded()
{
	_isActualShowCursor = false;
}

void RETextField::onTextInputResponderTextChanged(const REString & newTextString)
{
	this->setText(newTextString);
}

REBOOL RETextField::becomeFirstTextInputResponder()
{
	RETextInputRespondersManager * m = RETextInputRespondersManager::getDefaultManager();
	if (m) 
	{
		return m->setFirstResponder(this);
	}
	return false;
}

REBOOL RETextField::resignFirstTextInputResponder()
{
	RETextInputRespondersManager * m = RETextInputRespondersManager::getDefaultManager();
	if (m) 
	{
		return m->removeFirstResponder(this);
	}
	return false;
}

const REUInt32 RETextField::getTextInputResponderMaximumTextLength() const
{
	return _maximumInputedTextLength;
}

REBOOL RETextField::isTextInputResponderHasMaximumTextLength() const
{
	return _isHasMaximumInputedTextLength;
}

REBOOL RETextField::isClearPreviousInputedText() const
{
	return _isClearPreviousTextInputText;
}

const REString & RETextField::getTextInputResponderText() const
{
	return _text;
}

void RETextField::setMaximumInputedTextLength(const REUInt32 newMaxInputLen)
{
	_maximumInputedTextLength = newMaxInputLen;
}

void RETextField::setClearPreviousInputedText(REBOOL isClear)
{
	_isClearPreviousTextInputText = isClear;
}

void RETextField::setHasMaximumInputedTextLength(REBOOL isHasMaxTextLen)
{
	_isHasMaximumInputedTextLength = isHasMaxTextLen;
}

/// Is text input cursor showing
REBOOL RETextField::isShowCursor() const
{
	return _isShowCursor;
}

/// Set is need to show text input cursor.
void RETextField::setShowCursor(REBOOL isShow)
{
	if (_isShowCursor != isShow) 
	{
		_lastBlinkTime = 0.0;
		if (isShow) 
		{
			this->addToMainLoop();
		}
		else
		{
			this->removeFromMainLoop();
		}
	}
	_isShowCursor = isShow;
	_isActualShowCursor = false;
	if (isShow) 
	{
		RETextInputRespondersManager * m = RETextInputRespondersManager::getDefaultManager();
		if (m) 
		{
			_isActualShowCursor = m->isFirstResponder(this);
			_lastBlinkTime = 0.0;
		}
	}
}

/// Returns time in seconds of cursor blinking.
const RETimeInterval RETextField::getCursorBlinkTime() const
{
	return _cursorBlinkTime;
}

/// Setting time in seconds of cursor blinking. 
void RETextField::setCursorBlinkTime(const RETimeInterval newBlinkTime)
{
	_cursorBlinkTime = newBlinkTime;
}

REView * RETextField::getCursorView() const
{
	return _cursorView;
}

/* REObject */
void RETextField::onReleased()
{
	this->removeFromMainLoop();
	
	RETextInputRespondersManager * m = RETextInputRespondersManager::getDefaultManager();
	if (m) 
	{
		m->unRegisterResponder(this);
	}
	
	if (_cursorView) 
	{
		if (_cursorView->isAnimating()) 
		{
			_cursorView->stopAnimation(REAnimationStopTypeImmediately, true, false);
		}
		_cursorView->release();
		_cursorView = NULL;
	}
	
	RELabel::onReleased();
}

RETextField::RETextField() : RELabel(), REMainLoopUpdatable(),
	_cursorView(NULL),
	_cursorBlinkTime(0.6),
	_lastBlinkTime(0.0),
	_maximumInputedTextLength(0),
	_isHasMaximumInputedTextLength(false),
	_isClearPreviousTextInputText(false),
	_isShowCursor(false),
	_isActualShowCursor(false)
{
    REView * cursorView = REView::create();
	if (cursorView) 
	{
		if (this->addSubview(cursorView)) 
		{
			_cursorView = cursorView;
            _cursorView->setColor(REColor(0.0f, 0.0f, 0.0f, 1.0f));
		}
		else 
		{
            REObject::deleteObject(cursorView);
		}
	}

	RETextInputRespondersManager * m = RETextInputRespondersManager::getDefaultManager();
	if (m) 
	{
		m->registerResponder(this);
	}
	
	this->setShowCursor(true);
	this->setRespondsForUserAction(true);
}

RETextField::~RETextField()
{

}

RETextField * RETextField::create()
{
	RETextField * tf = new RETextField();
	return tf;
}

