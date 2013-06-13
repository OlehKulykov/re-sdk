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

void REButton::Render()
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
	
	REView::Render();
	
	_texture = originalTexture;
}

void REButton::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
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
	
	REView::RenderWithOffset(offsetX, offsetY);
	
	_texture = originalTexture;
}

RETextureObject * REButton::GetTextureForState(REButton::StateType state)
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

void REButton::SetTextureForState(RETextureObject * newTexture, REButton::StateType state)
{
	switch (state) 
	{
		case REButton::StateDefault:
			this->SetTexture(newTexture);
			break;
		case REButton::StatePressed:
		{
			RE_SAFE_RELEASE(_pressedTexture);

			if (newTexture) 
			{
				_pressedTexture = newTexture;
				_pressedTexture->Retain();
			}
		}
			break;
		case REButton::StateDisabled:
		{
			RE_SAFE_RELEASE(_disabledTexture);

			if (newTexture) 
			{
				_disabledTexture = newTexture;
				_disabledTexture->Retain();
			}
		}
			break;
		default:
			break;
	}
}

void REButton::SetButtonDownTargetMethod(REClassMethod * buttonDownTargetMethod)
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

REBOOL REButton::IsDisabled() const
{
	return (_state == REButton::StateDisabled);
}

void REButton::SetDisabled(const REBOOL isDisabled)
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
REBOOL REButton::IsRespondsForUserAction() const 
{
	if (_state != REButton::StateDisabled) 
	{
		return _isRespondsForUserAction;
	}
	return false; 
}

#ifdef __RE_OS_DESKTOP__
void REButton::UserCursorMoved(const REFloat32 coordX, const REFloat32 coordY)
{
	
}
REUInt32 REButton::GetUserCursorIdentifier() const
{
	return 0;
}
#endif	
void REButton::UserActionClickDidStart(const REFloat32 coordX, const REFloat32 coordY)
{
	if (_state != REButton::StateDisabled) 
	{
		if (this->GetScreenFrame().IsPointInRect(coordX, coordY)) 
		{
			_state = REButton::StatePressed;
		}
	}
}

void REButton::UserActionClickDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
								   const REFloat32 currentCoordX, const REFloat32 currentCoordY)
{
	if (_state == REButton::StatePressed) 
	{
		if ( this->GetScreenFrame().IsPointInRect(currentCoordX, currentCoordY) ) 
		{
			if (_buttonDownTargetMethod) 
			{
				_buttonDownTargetMethod->InvokeWithObject(this);
			}
		}
		
		if (_state == REButton::StatePressed) 
		{
			_state = REButton::StateDefault;
		}
	}
}

void REButton::UserActionClickMoving(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									 const REFloat32 currentCoordX, const REFloat32 currentCoordY)
{
	if (_state != REButton::StateDisabled) 
	{
		REBOOL isInside = false;
		RERect screenFrame( this->GetScreenFrame() );
		if ( screenFrame.IsPointInRect(startCoordX, startCoordY) ) 
		{
			isInside = screenFrame.IsPointInRect(currentCoordX, currentCoordY);
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

void REButton::UserActionClickMovingDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
										   const REFloat32 endCoordX, const REFloat32 endCoordY)
{
	
}

/* IREXMLSerializable */
REBOOL REButton::AcceptStringParameter(const char * key, const char * value)
{
	return REView::AcceptStringParameter(key, value);
}

REBOOL REButton::AcceptObjectParameter(const char * className, const char * key, REGUIObject * value)
{
	if (key && value) 
	{
        if (strcmp(key, RE_BUTTON_XML_PRESSED_TEXTURE_OBJECT_KEY_STRING) == 0)
		{
            RETextureObject * texture = (RETextureObject *)value;
			if (texture) 
			{
				this->SetTextureForState(texture, REButton::StatePressed);
				texture->Release();
				return true;
			}
		}
        else if (strcmp(key, RE_BUTTON_XML_DISABLED_TEXTURE_OBJECT_KEY_STRING) == 0)
		{
            RETextureObject * texture = (RETextureObject *)value;
			if (texture)
			{
				this->SetTextureForState(texture, REButton::StateDisabled);
				texture->Release();
				return true;
			}
		}
	}
	
	return REView::AcceptObjectParameter(className, key, value);
}

const REUInt32 REButton::GetClassIdentifier() const
{
	return REButton::ClassIdentifier();
}

const REUInt32 REButton::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REButton");
	return clasIdentif;
}

REBOOL REButton::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REButton::ClassIdentifier() == classIdentifier) || REView::IsImplementsClass(classIdentifier));
}

REButton::REButton() : REView(),
	_pressedTexture(NULL),
	_disabledTexture(NULL),
	_buttonDownTargetMethod(NULL),
	_state(REButton::StateDefault)
{
	
}

void REButton::OnReleased()
{
	RE_SAFE_RELEASE(_pressedTexture);
	RE_SAFE_RELEASE(_disabledTexture);
	RE_SAFE_DELETE(_buttonDownTargetMethod);
	
	REView::OnReleased();
}

REButton::~REButton()
{
	
}

REButton * REButton::Create()
{
	REButton * newButton = new REButton();
	return newButton;
}

const char * REButton::GetXMLPressedTextureObjectKeyString() { return RE_BUTTON_XML_PRESSED_TEXTURE_OBJECT_KEY_STRING; }
const char * REButton::GetXMLDisabledTextureObjectKeyString() { return RE_BUTTON_XML_DISABLED_TEXTURE_OBJECT_KEY_STRING; }

