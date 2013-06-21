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
	
	void renderSubViews(const REFloat32 x, const REFloat32 y)
	{
		REObjectsArray * subViews = this->getSubViewsArray();
		if (subViews) 
		{
			for (REUInt32 i = 0; i < subViews->count(); i++) 
			{
				((REView*)(*subViews)[i])->renderWithOffset(x, y);
			}
		}
	}

private:
    static REBOOL acceptViewStringParameter(REView * view, const char * key, const char * value);
public:
	/* REGUIObject */
	/// Acepting string parameter and value string presentation from XML document. 
	/// Using when view controller loading from XML. This method can be invoked from non-main thread if view controller loading in background.
	virtual REBOOL acceptStringParameter(const char * key, const char * value);
	
	/// Acepting object value for key from XML document with object's class name . 
	/// Using when view controller loading from XML. This method can be invoked from non-main thread if view controller loading in background.
	virtual REBOOL acceptObjectParameter(const char * className, const char * key, REGUIObject * value);
	
	/* IREUserActionResponder */
	virtual REBOOL isInterceptsUserAction() const { return _isInterceptsUserAction; }
	virtual void setInterceptsUserAction(REBOOL isInterceptsAction) { _isInterceptsUserAction = isInterceptsAction; }
	virtual REBOOL isRespondsForUserAction() const { return _isRespondsForUserAction; }
	virtual void setRespondsForUserAction(REBOOL isRespondsForAction) { _isRespondsForUserAction = isRespondsForAction; }
#ifdef __RE_OS_DESKTOP__
	virtual void userCursorMoved(const REFloat32 coordX, const REFloat32 coordY) { }
	virtual REUInt32 getUserCursorIdentifier() const { return 0; }
#endif	
	virtual void userActionClickDidStart(const REFloat32 coordX, const REFloat32 coordY) { }
	virtual void userActionClickDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									   const REFloat32 currentCoordX, const REFloat32 currentCoordY) { }
	virtual void userActionClickMoving(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									   const REFloat32 currentCoordX, const REFloat32 currentCoordY) { }
	
	virtual void userActionClickMovingDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
											 const REFloat32 endCoordX, const REFloat32 endCoordY) { }
	
	/* REObject */
	virtual const REUInt32 getClassIdentifier() const;
	static const REUInt32 classIdentifier();
	virtual REBOOL isImplementsClass(const REUInt32 classIdentifier) const;
	virtual void onReleased();
	
	/* IRERenderable */
	virtual void render();
	virtual void renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
	/// Setting rectangular frame of view.
	virtual void setFrame(const RERect & newViewFrame);
	
	/// Setting rectangular frame of view.
	/// Can be animated using REAnimation, call this method between REAnimation::Setup() and REAnimation::Execute().
	/// If animation is not settuping !REAnimation::isSetuping() than SetFrame() will be called.
	virtual void setFrameAnimated(const RERect & newViewFrame);
	
	/// Receiving rectangular frame of view
	virtual const RERect & getFrame() const;
	
	/// Setting color of view.
	virtual void setColor(const REColor & newViewColor);
	
	/// Setting color of view.
	/// Can be animated using REAnimation, call this method between REAnimation::Setup() and REAnimation::Execute().
	/// If animation is not settuping !REAnimation::isSetuping() than SetColor() will be called.
	virtual void setColorAnimated(const REColor & newViewColor);
	
	/// Receiving color of view
	virtual const REColor & getColor() const;
	
	/// Setting view color alpha channel in range [0.0f, 1.0f].
	/// If parameter 'isWithSubViews' is true than this alpha value will settes to all subview.
	virtual void setAlpha(const REFloat32 newAlpha, const REBOOL isWithSubViews = true);
	
	/// Setting view color alpha channel in range [0.0f, 1.0f].
	/// If parameter 'isWithSubViews' is true than this alpha value will settes to all subview.
	/// Can be animated using REAnimation, call this method between REAnimation::Setup() and REAnimation::Execute().
	/// If animation is not settuping !REAnimation::isSetuping() than SetAlpha() will be called.
	virtual void setAlphaAnimated(const REFloat32 newAlpha, const REBOOL isWithSubViews = true);
	
	/// Receiving view color alpha channel value.
	virtual const REFloat32 getAlpha() const;
	
	/// Returns count of working view animations count.
	const REUInt16 getAnimationsCount() const { return _animationsCount; }
	
	/// Checks is view params is animating.
	REBOOL isAnimating() const { return (_animationsCount > 0); }
	
	/// Receiving view texture object.
	virtual RETextureObject * getTexture();
	
	/// Setting view texture object or NULL. If texture is NULL view will render only with color.
	virtual void setTexture(RETextureObject * newTexture);
	
	/// Receiving rectangular frame using screen coordinates.
	virtual RERect getScreenFrame() const;
	
	/// Setting visibility of view.
    virtual void setVisible(const REBOOL isVisible ) { _isVisible = isVisible; }
	
	/// Returns value that indicating view visibility.
	REBOOL isVisible() const { return _isVisible; }
	
	/// Setting tag value for view.
    virtual void setTag(const REInt32 newTag) { _tag = newTag; }
	
	/// Receiving tag value of view.
	const REInt32 getTag() const { return _tag; }
	
	/// Removes this view from parant view.
	/// Returns false on removing error or there is no parent view.
	REBOOL removeFromParentView();
	
	/// Stopes all animation of view with stop type.
	/// Value of parameter 'isStopWithSubviews' indicates that need stop subviews animation.
	/// Value of parameter 'isNeedCallDelegate' indicates than need inform animation delegate about stoping animation.
	void stopAnimation(const REAnimationStopType stopType, const REBOOL isStopWithSubviews, const REBOOL isNeedCallDelegate);
	
	/// Creates and returns new view object.
	static REView * create();
	
	/// Creates and returns new view object with applied frame
	static REView * createWithFrame(const RERect & frame);
	
	/// Returns XML key string for tag.
	static const char * getXMLTagKeyString();
	
	/// Returns XML format string for tag.
	static const char * getXMLTagFormatString();
	
	/// Returns XML key string for visibility flag.
	static const char * getXMLVisibilityKeyString();
	
	/// Returns XML format string for visibility flag.
	static const char * getXMLVisibilityFormatString();
	
	/// Returns XML key string for frame.
	static const char * getXMLFrameKeyString();
	
	/// Returns XML format string for frame.
	static const char * getXMLFrameFormatString();
	
	/// Returns XML key string for color.
	static const char * getXMLColorKeyString();
	
	/// Returns XML format string for color.
	static const char * getXMLColorFormatString();
	
	/// Returns XML key string for flag responds user action.
	static const char * getXMLRespondsUserActionKeyString();
	
	/// Returns XML format string for flag responds user action.
	static const char * getXMLRespondsUserActionFormatString();
	
	/// Returns XML key string for flag intercepts user action.
	static const char * getXMLInterceptsUserActionKeyString();
	
	/// Returns XML format string for flag intercepts user action.
	static const char * getXMLInterceptsUserActionFormatString();
	
	/// Returns XML key string for new subview object.
	static const char * getXMLSubViewObjectKeyString();
	
	/// Returns XML key string for texture object.
	static const char * getXMLTextureObjectKeyString();
};

#endif /* __REVIEW_H__ */

