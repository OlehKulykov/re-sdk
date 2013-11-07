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

#include "../../include/recore/RELog.h"
#include "../../include/recore/REStaticString.h"

#include "../../include/regui/REGUIApplication.h"



void REButton::render()
{
	RETexture * originalTexture = _texture;
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
	RETexture * originalTexture = _texture;
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

RETexture * REButton::textureForState(REButton::StateType state)
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

void REButton::setTextureForState(RETexture * newTexture, REButton::StateType state)
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
		if (this->screenFrame().isPointInRect(coordX, coordY)) 
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
		if ( this->screenFrame().isPointInRect(currentCoordX, currentCoordY) ) 
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
		RERect screenFrame( this->screenFrame() );
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

#define TYPED_STATIC_STRING(s) RETypedPtr(new REStaticString(s), REPtrTypeString) 

RETypedPtr REButton::serializeToDictionary() const
{
	RETypedPtr d(REView::serializeToDictionary());
	REDictionary * dict = d.dictionary();
	if (dict) 
	{
		dict->setValue(TYPED_STATIC_STRING("REButton"), TYPED_STATIC_STRING(RESerializable::classNameKey()));
		if (_pressedTexture) 
		{
			dict->setValue(_pressedTexture->serializeToDictionary(), TYPED_STATIC_STRING("pressedtexture"));
		}
	}
	return d;
}

void REButton::deserializeWithDictionary(const RETypedPtr & dictionary)
{
	REView::deserializeWithDictionary(dictionary);
	
	REDictionary * dict = dictionary.dictionary();
	if (dict) 
	{
		RETypedPtr textureDict(dict->valueForKey("pressedtexture"));
		REDictionary * texture = textureDict.dictionary();
		if (texture) 
		{
			REGUIApplication * app = REGUIApplication::currentApplication();
			if (app) 
			{
				RETexture * t = app->createSerializableClassWithDictionary<RETexture>(textureDict);
				if (t) 
				{
					t->deserializeWithDictionary(textureDict);
					this->setTextureForState(t, REButton::StatePressed);
					t->release();
				}
			}
			else { RELog::log("ERROR: REGUIApplication not initialized"); }
		}
	}
}

REButton::~REButton()
{
	
}

REButton * REButton::create()
{
	REButton * newButton = new REButton();
	return newButton;
}


