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


#include "../../include/regui/REButton.h"

#define RE_BUTTON_XML_PRESSED_TEXTURE_OBJECT_KEY_STRING "pressedtexture"
#define RE_BUTTON_XML_DISABLED_TEXTURE_OBJECT_KEY_STRING "disabledtexture"

void REButton::render()
{
	RETextureObject * originalTexture = _texture;
	switch (_state) 
	{
		case REButton::StatePressed:
			_texture = _pressedTexture;
			break;
		case REButton::StateDisabled:
			_texture = _disabledTexture;
			break;
		default:
			break;
	}
	
	REView::render();
	
	_texture = originalTexture;
}

void REButton::renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	RETextureObject * originalTexture = _texture;
	switch (_state) 
	{
		case REButton::StatePressed:
			_texture = _pressedTexture;
			break;
		case REButton::StateDisabled:
			_texture = _disabledTexture;
			break;
		default:
			break;
	}
	
	REView::renderWithOffset(offsetX, offsetY);
	
	_texture = originalTexture;
}

RETextureObject * REButton::getTextureForState(REButton::StateType state)
{
	switch (state) 
	{
		case REButton::StateDefault:
			return _texture;
			break;
		case REButton::StatePressed:
			return _pressedTexture;
			break;
		case REButton::StateDisabled:
			return _disabledTexture;
			break;
		default:
			break;
	}
	return NULL;
}

void REButton::setTextureForState(RETextureObject * newTexture, REButton::StateType state)
{
	switch (state) 
	{
		case REButton::StateDefault:
			this->setTexture(newTexture);
			break;
		case REButton::StatePressed:
		{
			RE_SAFE_RELEASE(_pressedTexture);

			if (newTexture) 
			{
				_pressedTexture = newTexture;
				_pressedTexture->retain();
			}
		}
			break;
		case REButton::StateDisabled:
		{
			RE_SAFE_RELEASE(_disabledTexture);

			if (newTexture) 
			{
				_disabledTexture = newTexture;
				_disabledTexture->retain();
			}
		}
			break;
		default:
			break;
	}
}

void REButton::setButtonDownTargetMethod(REClassMethod * buttonDownTargetMethod)
{
	if (_buttonDownTargetMethod) 
	{
		delete _buttonDownTargetMethod;
		_buttonDownTargetMethod = NULL;
	}
	if (buttonDownTargetMethod) 
	{
		_buttonDownTargetMethod = buttonDownTargetMethod;
	}
}

REBOOL REButton::isDisabled() const
{
	return (_state == REButton::StateDisabled);
}

void REButton::setDisabled(const REBOOL isDisabled)
{
	if (isDisabled) 
	{
		_state = REButton::StateDisabled;
	}
	else
	{
		_state = REButton::StateDefault;
	}
}

/* IREUserActionResponder */
REBOOL REButton::isRespondsForUserAction() const 
{
	if (_state != REButton::StateDisabled) 
	{
		return _isRespondsForUserAction;
	}
	return false; 
}

#ifdef __RE_OS_DESKTOP__
void REButton::userCursorMoved(const REFloat32 coordX, const REFloat32 coordY)
{
	
}
REUInt32 REButton::getUserCursorIdentifier() const
{
	return 0;
}
#endif	
void REButton::userActionClickDidStart(const REFloat32 coordX, const REFloat32 coordY)
{
	if (_state != REButton::StateDisabled) 
	{
		if (this->getScreenFrame().isPointInRect(coordX, coordY)) 
		{
			_state = REButton::StatePressed;
		}
	}
}

void REButton::userActionClickDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
								   const REFloat32 currentCoordX, const REFloat32 currentCoordY)
{
	if (_state == REButton::StatePressed) 
	{
		if ( this->getScreenFrame().isPointInRect(currentCoordX, currentCoordY) ) 
		{
			if (_buttonDownTargetMethod) 
			{
				_buttonDownTargetMethod->invokeWithObject(this);
			}
		}
		
		if (_state == REButton::StatePressed) 
		{
			_state = REButton::StateDefault;
		}
	}
}

void REButton::userActionClickMoving(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									 const REFloat32 currentCoordX, const REFloat32 currentCoordY)
{
	if (_state != REButton::StateDisabled) 
	{
		REBOOL isInside = false;
		RERect screenFrame( this->getScreenFrame() );
		if ( screenFrame.isPointInRect(startCoordX, startCoordY) ) 
		{
			isInside = screenFrame.isPointInRect(currentCoordX, currentCoordY);
		}
		
		if (isInside) 
		{ 
			_state = REButton::StatePressed; 
		}
		else 
		{
			_state = REButton::StateDefault;
		}
	}
}

void REButton::userActionClickMovingDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
										   const REFloat32 endCoordX, const REFloat32 endCoordY)
{
	
}

/* IREXMLSerializable */
REBOOL REButton::acceptStringParameter(const char * key, const char * value)
{
	return REView::acceptStringParameter(key, value);
}

REBOOL REButton::acceptObjectParameter(const char * className, const char * key, REGUIObject * value)
{
	if (key && value) 
	{
        if (strcmp(key, RE_BUTTON_XML_PRESSED_TEXTURE_OBJECT_KEY_STRING) == 0)
		{
            RETextureObject * texture = (RETextureObject *)value;
			if (texture) 
			{
				this->setTextureForState(texture, REButton::StatePressed);
				texture->release();
				return true;
			}
		}
        else if (strcmp(key, RE_BUTTON_XML_DISABLED_TEXTURE_OBJECT_KEY_STRING) == 0)
		{
            RETextureObject * texture = (RETextureObject *)value;
			if (texture)
			{
				this->setTextureForState(texture, REButton::StateDisabled);
				texture->release();
				return true;
			}
		}
	}
	
	return REView::acceptObjectParameter(className, key, value);
}

const REUInt32 REButton::getClassIdentifier() const
{
	return REButton::classIdentifier();
}

const REUInt32 REButton::classIdentifier()
{
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("REButton");
	return clasIdentif;
}

REBOOL REButton::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REButton::classIdentifier() == classIdentifier) || REView::isImplementsClass(classIdentifier));
}

REButton::REButton() : REView(),
	_pressedTexture(NULL),
	_disabledTexture(NULL),
	_buttonDownTargetMethod(NULL),
	_state(REButton::StateDefault)
{
	
}

void REButton::onReleased()
{
	RE_SAFE_RELEASE(_pressedTexture);
	RE_SAFE_RELEASE(_disabledTexture);
	RE_SAFE_DELETE(_buttonDownTargetMethod);
	
	REView::onReleased();
}

REButton::~REButton()
{
	
}

REButton * REButton::create()
{
	REButton * newButton = new REButton();
	return newButton;
}

const char * REButton::getXMLPressedTextureObjectKeyString() { return RE_BUTTON_XML_PRESSED_TEXTURE_OBJECT_KEY_STRING; }
const char * REButton::getXMLDisabledTextureObjectKeyString() { return RE_BUTTON_XML_DISABLED_TEXTURE_OBJECT_KEY_STRING; }

