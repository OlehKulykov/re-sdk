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
	virtual void SetTextureForState(RETextureObject * newTexture, REButton::StateType state);
	
	/// Retuns pointer to texture for state. If no texture assigned NULL wil return.
	virtual RETextureObject * GetTextureForState(REButton::StateType state);
	
	/// Set method pointer to method that will invoked when button pressed down. Prev method will delete and new will set.
	void SetButtonDownTargetMethod(REClassMethod * buttonDownTargetMethod);
	
	/// Checks is button disabled.
	REBOOL IsDisabled() const;
	
	/// Sets button state to disable or enabled
	void SetDisabled(const REBOOL isDisabled);
	
	/* IREUserActionResponder */
	virtual REBOOL IsRespondsForUserAction() const;
	
#ifdef __RE_OS_DESKTOP__
	virtual void UserCursorMoved(const REFloat32 coordX, const REFloat32 coordY);
	virtual REUInt32 GetUserCursorIdentifier() const; /* {return 0;} */
#endif	
	virtual void UserActionClickDidStart(const REFloat32 coordX, const REFloat32 coordY);
	virtual void UserActionClickDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									   const REFloat32 currentCoordX, const REFloat32 currentCoordY);
	virtual void UserActionClickMoving(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									   const REFloat32 currentCoordX, const REFloat32 currentCoordY);
	virtual void UserActionClickMovingDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
											 const REFloat32 endCoordX, const REFloat32 endCoordY);
	
	
	/* REGUIObject */
	virtual REBOOL AcceptStringParameter(const char * key, const char * value);
	virtual REBOOL AcceptObjectParameter(const char * className, const char * key, REGUIObject * value);
	
	/* REView */
	virtual void Render();
	virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	/// Creates and returns new button object.
	static REButton * Create();

	/// Returns XML key string for pressed texture object.
    static const char * GetXMLPressedTextureObjectKeyString();
	
	/// Returns XML key string for disabled texture object.
    static const char * GetXMLDisabledTextureObjectKeyString();
};




#endif /* __REBUTTON_H__ */


