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


#ifndef __REBUTTON_H__
#define __REBUTTON_H__

#include "../recore/RECommonHeader.h"
#include "REView.h"
#include "IREUserActionResponder.h"

/// Class of button
class __RE_PUBLIC_CLASS_API__ REButton : public REView
{
public:
	/// Enum using for state of button
	typedef enum _stateType
	{
		/// Default state of button.
		StateDefault = 0,
		
		/// Pressed state of button.
		StatePressed,
		
		/// Disabled state of button.
		StateDisabled
	} 
	/// Enum using for state of button
	StateType;
	
protected:
	RETextureObject * _pressedTexture;
	RETextureObject * _disabledTexture;
	REClassMethod * _buttonDownTargetMethod;
	REButton::StateType _state;
	
	REButton();
	virtual ~REButton();
	
public:
	/// Set texture of button for selected state, prev texture if released and new assigned.
	virtual void setTextureForState(RETextureObject * newTexture, REButton::StateType state);
	
	/// Retuns pointer to texture for state. If no texture assigned NULL wil return.
	virtual RETextureObject * getTextureForState(REButton::StateType state);
	
	/// Set method pointer to method that will invoked when button pressed down. Prev method will delete and new will set.
	void setButtonDownTargetMethod(REClassMethod * buttonDownTargetMethod);
	
	/// Checks is button disabled.
	REBOOL isDisabled() const;
	
	/// Sets button state to disable or enabled
	void setDisabled(const REBOOL isDisabled);
	
	/* IREUserActionResponder */
	virtual REBOOL isRespondsForUserAction() const;
	
#ifdef __RE_OS_DESKTOP__
	virtual void userCursorMoved(const REFloat32 coordX, const REFloat32 coordY);
	virtual REUInt32 getUserCursorIdentifier() const; /* {return 0;} */
#endif	
	virtual void userActionClickDidStart(const REFloat32 coordX, const REFloat32 coordY);
	virtual void userActionClickDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									   const REFloat32 currentCoordX, const REFloat32 currentCoordY);
	virtual void userActionClickMoving(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									   const REFloat32 currentCoordX, const REFloat32 currentCoordY);
	virtual void userActionClickMovingDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
											 const REFloat32 endCoordX, const REFloat32 endCoordY);
	
	
	/* REGUIObject */
	virtual REBOOL acceptStringParameter(const char * key, const char * value);
	virtual REBOOL acceptObjectParameter(const char * className, const char * key, REGUIObject * value);
	
	/* REView */
	virtual void render();
	virtual void renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
	/* REObject */
	virtual const REUInt32 getClassIdentifier() const;
	static const REUInt32 classIdentifier();
	virtual REBOOL isImplementsClass(const REUInt32 classIdentifier) const;
	virtual void onReleased();
	
	/// Creates and returns new button object.
	static REButton * create();

	/// Returns XML key string for pressed texture object.
    static const char * getXMLPressedTextureObjectKeyString();
	
	/// Returns XML key string for disabled texture object.
    static const char * getXMLDisabledTextureObjectKeyString();
};




#endif /* __REBUTTON_H__ */


