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
#include "../recore/REArrayObject.h"
#include "../recore/REClassMethod.h"
#include "../recore/REPtr.h"
#include "../recore/RETypedPtr.h"
#include "../recore/RESerializable.h"

#include "IRERenderable.h"
#include "IREUserActionResponder.h"
#include "RESubViewsContainer.h"
#include "RETexture.h"
#include "RERenderDevice.h"
#include "REAnimation.h"


/// Class of rectangular view used for display.
class __RE_PUBLIC_CLASS_API__ REView : public REObject, public RESubviewsContainer, public IRERenderable, public IREUserActionResponder
{
protected:
	RETexture * _texture;
	RERect _frame;
	REColor _color;
	REInt32 _tag;
	
	REUInt16 _animationsCount;
	REBOOL _isVisible;
	REBOOL _isRespondsForUserAction;
	REBOOL _isInterceptsUserAction;
	
	REView();
	virtual ~REView();
	
	void renderSubviews(const REFloat32 x, const REFloat32 y)
	{
		REList<REView *>::Iterator iter(this->subviewsIterator());
		while (iter.next()) 
		{
			if (iter.value()) iter.value()->renderWithOffset(x, y);
		}
	}
	
public:
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
	virtual void onReleased();
	
	/* RESerializable */
	virtual RETypedPtr serializeToDictionary() const;
	
	virtual void deserializeWithDictionary(const RETypedPtr & dictionary);
	
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
	virtual const RERect & frame() const;
	
	/// Setting color of view.
	virtual void setColor(const REColor & newViewColor);
	
	/// Setting color of view.
	/// Can be animated using REAnimation, call this method between REAnimation::Setup() and REAnimation::Execute().
	/// If animation is not settuping !REAnimation::isSetuping() than SetColor() will be called.
	virtual void setColorAnimated(const REColor & newViewColor);
	
	/// Receiving color of view
	virtual const REColor & color() const;
	
	/// Setting view color alpha channel in range [0.0f, 1.0f].
	/// If parameter 'isWithSubViews' is true than this alpha value will settes to all subview.
	virtual void setAlpha(const REFloat32 newAlpha, const REBOOL isWithSubViews = true);
	
	/// Setting view color alpha channel in range [0.0f, 1.0f].
	/// If parameter 'isWithSubViews' is true than this alpha value will settes to all subview.
	/// Can be animated using REAnimation, call this method between REAnimation::Setup() and REAnimation::Execute().
	/// If animation is not settuping !REAnimation::isSetuping() than SetAlpha() will be called.
	virtual void setAlphaAnimated(const REFloat32 newAlpha, const REBOOL isWithSubViews = true);
	
	/// Receiving view color alpha channel value.
	virtual const REFloat32 alpha() const;
	
	/// Returns count of working view animations count.
	const REUInt16 animationsCount() const;
	
	/// Checks is view params is animating.
	REBOOL isAnimating() const;
	
	/// Receiving view texture object.
	virtual RETexture * texture();
	
	/// Setting view texture object or NULL. If texture is NULL view will render only with color.
	virtual void setTexture(RETexture * newTexture);
	
	/// Receiving rectangular frame using screen coordinates.
	virtual RERect screenFrame() const;
	
	/// Setting visibility of view.
    virtual void setVisible(const REBOOL isVisible);
	
	/// Returns value that indicating view visibility.
	REBOOL isVisible() const;
	
	/// Setting tag value for view.
    virtual void setTag(const REInt32 newTag);
	
	/// Receiving tag value of view.
	const REInt32 tag() const;
	
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
	
	static REView * createWithDeserializableDictionary(const RETypedPtr & dictionary);
};

#endif /* __REVIEW_H__ */

