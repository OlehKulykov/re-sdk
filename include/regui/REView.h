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


#ifndef __REVIEW_H__
#define __REVIEW_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REObject.h"
#include "../recore/RERect.h"
#include "../recore/REString.h"
#include "../recore/REColor.h"
#include "../recore/REObjectsArray.h"
#include "../recore/REClassMethod.h"

#include "REGUIObject.h"
#include "IRERenderable.h"
#include "IREUserActionResponder.h"
#include "RERenderDeviceTextureObject.h"
#include "RESubViewsContainer.h"
#include "RETextureObject.h"
#include "RERenderDevice.h"
#include "REAnimation.h"


/// Class of rectangular view used for display.
class __RE_PUBLIC_CLASS_API__ REView : public REGUIObject,
public RESubViewsContainer, 
public IRERenderable,
public IREUserActionResponder
{
protected:
	RETextureObject * _texture;
	RERect _frame;
	REColor _color;
	REInt32 _tag;
	
	REUInt16 _animationsCount;
	REBOOL _isVisible;
	REBOOL _isRespondsForUserAction;
	REBOOL _isInterceptsUserAction;
	
	REView();
	virtual ~REView();
	
	void RenderSubViews(const REFloat32 x, const REFloat32 y)
	{
		REObjectsArray * subViews = this->GetSubViewsArray();
		if (subViews) 
			for (REUInt32 i = 0; i < subViews->Count(); i++) 
				((REView*)(*subViews)[i])->RenderWithOffset(x, y);
	}

private:
    static REBOOL AcceptViewStringParameter(REView * view, const char * key, const char * value);
public:
	/* REGUIObject */
	/// Acepting string parameter and value string presentation from XML document. 
	/// Using when view controller loading from XML. This method can be invoked from non-main thread if view controller loading in background.
	virtual REBOOL AcceptStringParameter(const char * key, const char * value);
	
	/// Acepting object value for key from XML document with object's class name . 
	/// Using when view controller loading from XML. This method can be invoked from non-main thread if view controller loading in background.
	virtual REBOOL AcceptObjectParameter(const char * className, const char * key, REGUIObject * value);
	
	/* IREUserActionResponder */
	virtual REBOOL IsInterceptsUserAction() const { return _isInterceptsUserAction; }
	virtual void SetInterceptsUserAction(REBOOL isInterceptsAction) { _isInterceptsUserAction = isInterceptsAction; }
	virtual REBOOL IsRespondsForUserAction() const { return _isRespondsForUserAction; }
	virtual void SetRespondsForUserAction(REBOOL isRespondsForAction) { _isRespondsForUserAction = isRespondsForAction; }
#ifdef __RE_OS_DESKTOP__
	virtual void UserCursorMoved(const REFloat32 coordX, const REFloat32 coordY) { }
	virtual REUInt32 GetUserCursorIdentifier() const { return 0; }
#endif	
	virtual void UserActionClickDidStart(const REFloat32 coordX, const REFloat32 coordY) { }
	virtual void UserActionClickDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									   const REFloat32 currentCoordX, const REFloat32 currentCoordY) { }
	virtual void UserActionClickMoving(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									   const REFloat32 currentCoordX, const REFloat32 currentCoordY) { }
	
	virtual void UserActionClickMovingDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
											 const REFloat32 endCoordX, const REFloat32 endCoordY) { }
	
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	/* IRERenderable */
	virtual void Render();
	virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
	/// Setting rectangular frame of view.
	virtual void SetFrame(const RERect & newViewFrame);
	
	/// Setting rectangular frame of view.
	/// Can be animated using REAnimation, call this method between REAnimation::Setup() and REAnimation::Execute().
	/// If animation is not settuping !REAnimation::IsSetuping() than SetFrame() will be called.
	virtual void SetFrameAnimated(const RERect & newViewFrame);
	
	/// Receiving rectangular frame of view
	virtual const RERect & GetFrame() const;
	
	/// Setting color of view.
	virtual void SetColor(const REColor & newViewColor);
	
	/// Setting color of view.
	/// Can be animated using REAnimation, call this method between REAnimation::Setup() and REAnimation::Execute().
	/// If animation is not settuping !REAnimation::IsSetuping() than SetColor() will be called.
	virtual void SetColorAnimated(const REColor & newViewColor);
	
	/// Receiving color of view
	virtual const REColor & GetColor() const;
	
	/// Setting view color alpha channel in range [0.0f, 1.0f].
	/// If parameter 'isWithSubViews' is true than this alpha value will settes to all subview.
	virtual void SetAlpha(const REFloat32 newAlpha, const REBOOL isWithSubViews = true);
	
	/// Setting view color alpha channel in range [0.0f, 1.0f].
	/// If parameter 'isWithSubViews' is true than this alpha value will settes to all subview.
	/// Can be animated using REAnimation, call this method between REAnimation::Setup() and REAnimation::Execute().
	/// If animation is not settuping !REAnimation::IsSetuping() than SetAlpha() will be called.
	virtual void SetAlphaAnimated(const REFloat32 newAlpha, const REBOOL isWithSubViews = true);
	
	/// Receiving view color alpha channel value.
	virtual const REFloat32 GetAlpha() const;
	
	/// Returns count of working view animations count.
	const REUInt16 GetAnimationsCount() const { return _animationsCount; }
	
	/// Checks is view params is animating.
	REBOOL IsAnimating() const { return (_animationsCount > 0); }
	
	/// Receiving view texture object.
	virtual RETextureObject * GetTexture();
	
	/// Setting view texture object or NULL. If texture is NULL view will render only with color.
	virtual void SetTexture(RETextureObject * newTexture);
	
	/// Receiving rectangular frame using screen coordinates.
	virtual RERect GetScreenFrame() const;
	
	/// Setting visibility of view.
    virtual void SetVisible(const REBOOL isVisible ) { _isVisible = isVisible; }
	
	/// Returns value that indicating view visibility.
	REBOOL IsVisible() const { return _isVisible; }
	
	/// Setting tag value for view.
    virtual void SetTag(const REInt32 newTag) { _tag = newTag; }
	
	/// Receiving tag value of view.
	const REInt32 GetTag() const { return _tag; }
	
	/// Removes this view from parant view.
	/// Returns false on removing error or there is no parent view.
	REBOOL RemoveFromParentView();
	
	/// Stopes all animation of view with stop type.
	/// Value of parameter 'isStopWithSubviews' indicates that need stop subviews animation.
	/// Value of parameter 'isNeedCallDelegate' indicates than need inform animation delegate about stoping animation.
	void StopAnimation(const REAnimationStopType stopType, const REBOOL isStopWithSubviews, const REBOOL isNeedCallDelegate);
	
	/// Creates and returns new view object.
	static REView * Create();
	
	/// Creates and returns new view object with applied frame
	static REView * CreateWithFrame(const RERect & frame);
	
	/// Returns XML key string for tag.
	static const char * GetXMLTagKeyString();
	
	/// Returns XML format string for tag.
	static const char * GetXMLTagFormatString();
	
	/// Returns XML key string for visibility flag.
	static const char * GetXMLVisibilityKeyString();
	
	/// Returns XML format string for visibility flag.
	static const char * GetXMLVisibilityFormatString();
	
	/// Returns XML key string for frame.
	static const char * GetXMLFrameKeyString();
	
	/// Returns XML format string for frame.
	static const char * GetXMLFrameFormatString();
	
	/// Returns XML key string for color.
	static const char * GetXMLColorKeyString();
	
	/// Returns XML format string for color.
	static const char * GetXMLColorFormatString();
	
	/// Returns XML key string for flag responds user action.
	static const char * GetXMLRespondsUserActionKeyString();
	
	/// Returns XML format string for flag responds user action.
	static const char * GetXMLRespondsUserActionFormatString();
	
	/// Returns XML key string for flag intercepts user action.
	static const char * GetXMLInterceptsUserActionKeyString();
	
	/// Returns XML format string for flag intercepts user action.
	static const char * GetXMLInterceptsUserActionFormatString();
	
	/// Returns XML key string for new subview object.
	static const char * GetXMLSubViewObjectKeyString();
	
	/// Returns XML key string for texture object.
	static const char * GetXMLTextureObjectKeyString();
};

#endif /* __REVIEW_H__ */

