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


#include "../../include/regui/REView.h"
#include "../../include/regui/REButton.h"
#include "../../include/regui/RELabel.h"
#include "../../include/recore/REDictionary.h"
#include "../../include/recore/RENumber.h"
#include "../../include/recore/REStaticString.h"
#include "../../include/recore/RELog.h"
#include "../../include/regui/REGUIApplication.h"


REBOOL REView::acceptViewStringParameter(REView * view, const char * key, const char * value)
{
	return false;
}

/* REGUIObject */
REBOOL REView::acceptStringParameter(const char * key, const char * value)
{
    return false;
}

REBOOL REView::acceptObjectParameter(const char * className, const char * key, REGUIObject * value)
{
	return false;
}

/* IREView */
void REView::render()
{
	if (_isVisible)
	{
		if (_color.alpha > 0.0f) 
		{
			RERenderDevice * device = RERenderDevice::GetDefaultDevice();
			if (_texture) 
			{
				device->RenderRect(_frame, _color, _texture->GetFrame(), _texture->GetTexture());
			}
			else
			{
				device->RenderRect(_frame, _color, NULL);
			}
		}
		
		this->renderSubviews(_frame.x, _frame.y);
	}
}

void REView::renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	if ( _isVisible )
	{	
		const REFloat32 renderX = _frame.x + offsetX;
		const REFloat32 renderY = _frame.y + offsetY;
		
		if (_color.alpha > 0.0f) 
		{
			RERect rectWithOffset(renderX, renderY, _frame.width, _frame.height);
			RERenderDevice * device = RERenderDevice::GetDefaultDevice();
			if (_texture) 
			{
				device->RenderRect(rectWithOffset, _color, _texture->GetFrame(), _texture->GetTexture());
			}
			else
			{
				device->RenderRect(rectWithOffset, _color, NULL);
			}
		}
		
		this->renderSubviews(renderX, renderY);
	}
}

void REView::setFrame(const RERect & newViewFrame)
{	
	_frame = newViewFrame;
}

void REView::setFrameAnimated(const RERect & newViewFrame)
{
	if (REAnimation::isSetuping())
	{
		if (_frame.x != newViewFrame.x)
		{
			REAnimation::addFloatParam(this, &_animationsCount, &_frame.x, _frame.x, newViewFrame.x);
		}
		if (_frame.y != newViewFrame.y)
		{
			REAnimation::addFloatParam(this, &_animationsCount, &_frame.y, _frame.y, newViewFrame.y);
		}
		if (_frame.width != newViewFrame.width)
		{
			REAnimation::addFloatParam(this, &_animationsCount, &_frame.width, _frame.width, newViewFrame.width);
		}
		if (_frame.height != newViewFrame.height)
		{
			REAnimation::addFloatParam(this, &_animationsCount, &_frame.height, _frame.height, newViewFrame.height);
		}
	}
	else
	{
		this->setFrame(newViewFrame);
	}
}

const RERect & REView::frame() const
{
	return _frame;
}

void REView::setColor(const REColor & newViewColor)
{
	_color = newViewColor;
}

void REView::setColorAnimated(const REColor & newViewColor)
{
	if (REAnimation::isSetuping())
	{
		if (_color.red != newViewColor.red)
		{
			REAnimation::addFloatParam(this, &_animationsCount, &_color.red, _color.red, newViewColor.red);
		}
		if (_color.green != newViewColor.green)
		{
			REAnimation::addFloatParam(this, &_animationsCount, &_color.green, _color.green, newViewColor.green);
		}
		if (_color.blue != newViewColor.blue)
		{
			REAnimation::addFloatParam(this, &_animationsCount, &_color.blue, _color.blue, newViewColor.blue);
		}
		if (_color.alpha != newViewColor.alpha)
		{
			REAnimation::addFloatParam(this, &_animationsCount, &_color.alpha, _color.alpha, newViewColor.alpha);
		}
	}
	else
	{
		this->setColor(newViewColor);
	}
}

const REColor & REView::color() const
{
	return _color;
}

void REView::setAlpha(const REFloat32 newAlpha, const REBOOL isWithSubViews)
{
	if (isWithSubViews) 
	{
		REList<REView *>::Iterator iter(this->subviewsIterator());
		while (iter.next()) 
		{
			iter.value()->setAlpha(newAlpha, isWithSubViews);
		}
	}
	_color.alpha = newAlpha;
}

void REView::setAlphaAnimated(const REFloat32 newAlpha, const REBOOL isWithSubViews)
{
	if (isWithSubViews) 
	{
		REList<REView *>::Iterator iter(this->subviewsIterator());
		while (iter.next()) 
		{
			iter.value()->setAlphaAnimated(newAlpha, isWithSubViews);
		}
	}
	
	if (REAnimation::isSetuping())
	{
		if (_color.alpha != newAlpha)
		{
			REAnimation::addFloatParam(this,&_animationsCount,&_color.alpha,_color.alpha,newAlpha);
		}
	}
	else
	{
		this->setAlpha(newAlpha, isWithSubViews);
	}
}

const REFloat32 REView::alpha() const
{
	return _color.alpha;
}

const REUInt16 REView::animationsCount() const 
{
	return _animationsCount; 
}

REBOOL REView::isAnimating() const 
{
	return (_animationsCount > 0); 
}

void REView::stopAnimation(const REAnimationStopType stopType, const REBOOL isStopWithSubviews, const REBOOL isNeedCallDelegate)
{
	if ( this->isAnimating() )
	{
		REAnimation::stopAllAnimations(this, stopType, isNeedCallDelegate);
	}
	
	if (isStopWithSubviews) 
	{
		REList<REView *>::Iterator iter(this->subviewsIterator());
		while (iter.next()) 
		{
			iter.value()->stopAnimation(stopType, isStopWithSubviews, isNeedCallDelegate);
		}
	}
}

REBOOL REView::removeFromParentView()
{
	RESubviewsContainer * parent = this->parentSubviewsContainer();
	if (parent) 
	{
		parent->removeSubview(this);
	}
	return true;
}

RERect REView::screenFrame() const
{
	REView * parent = dynamic_cast<REView *>(this->parentSubviewsContainer());
    if (parent)
	{
		RERect parentScreenFrame(parent->screenFrame());
		return RERect(_frame.x + parentScreenFrame.x,
					  _frame.y + parentScreenFrame.y,
					  _frame.width,
					  _frame.height);
	}
	return _frame;
}

RETextureObject * REView::texture()
{
	return _texture;
}

void REView::setTexture(RETextureObject * newTexture)
{
	RE_SAFE_RELEASE(_texture);
	
	if (newTexture)
	{
		_texture = newTexture;
		_texture->retain();
	}
}

void REView::setVisible(const REBOOL isVisible) 
{
	_isVisible = isVisible; 
}

REBOOL REView::isVisible() const 
{ 
	return _isVisible; 
}

void REView::setTag(const REInt32 newTag) 
{
	_tag = newTag; 
}

const REInt32 REView::tag() const 
{
	return _tag; 
}

void REView::onReleased()
{
	this->removeFromParentView();
	
	RE_SAFE_RELEASE(_texture);
	
	if (this->isAnimating())
	{
		REAnimation::stopAllAnimations(this, REAnimationStopTypeImmediately, false);
	}
	
	REGUIObject::onReleased();
}

REView::REView() : REGUIObject(), RESubviewsContainer(), RESerializable(),
	_texture(NULL),
	_tag(0),
	_animationsCount(0),
	_isVisible(true),
	_isRespondsForUserAction(false),
	_isInterceptsUserAction(true)
{
	_color.red = 1.0f;
	_color.green = 1.0f;
	_color.blue = 1.0f;
	_color.alpha = 1.0f;
}

REView::~REView()
{
	
}

#define TYPED_STATIC_STRING(s) RETypedPtr(new REStaticString(s), REPtrTypeString) 

void REView::serializeSubviewsToDictionary(REDictionary * dictionary) const
{
	if (this->isHasSubviews()) 
	{
		RETypedPtr arr(new RETypedArray(), REPtrTypeArray);
		RETypedArray * subviews = arr.array();
		if (subviews) 
		{
			REList<REView *>::Iterator iter(this->subviewsIterator());
			while (iter.next()) 
			{
				RETypedPtr subDict(iter.value()->serializeToDictionary());
				if (subDict.isNotEmpty()) 
				{
					subviews->add(subDict);
				}
			}
			if (subviews->count() > 0) 
			{
				dictionary->setValue(arr, TYPED_STATIC_STRING("subviews"));
			}
		}
	}
}

RETypedPtr REView::serializeToDictionary() const
{
	RETypedPtr d(RESerializable::serializeToDictionary());
	REDictionary * dict = d.dictionary();
	if (dict) 
	{
		dict->setValue(TYPED_STATIC_STRING("REView"), TYPED_STATIC_STRING(RESerializable::classNameKey()));
		dict->setValue(RETypedPtr(new REString(RERect::toString(_frame)), REPtrTypeString), TYPED_STATIC_STRING("frame"));
		dict->setValue(RETypedPtr(new REString(REColor::toString(_color)), REPtrTypeString), TYPED_STATIC_STRING("color"));
		dict->setValue(RETypedPtr(new RENumber((REInt64)_tag), REPtrTypeNumber), TYPED_STATIC_STRING("tag"));
		dict->setValue(RETypedPtr(new RENumber((REBOOL)_isVisible), REPtrTypeNumber), TYPED_STATIC_STRING("visible"));
		dict->setValue(RETypedPtr(new RENumber((REBOOL)_isRespondsForUserAction), REPtrTypeNumber), TYPED_STATIC_STRING("responduseraction"));
		dict->setValue(RETypedPtr(new RENumber((REBOOL)_isInterceptsUserAction), REPtrTypeNumber), TYPED_STATIC_STRING("interceptuseraction"));
		if (_texture) 
		{
			dict->setValue(_texture->serializeToDictionary(), TYPED_STATIC_STRING("texture"));
		}
		this->serializeSubviewsToDictionary(dict);
	}
	return d;
}

void REView::deserializeSubviewsFromDictionary(RETypedArray * subviews)
{
	for (REUInt32 i = 0; i < subviews->count(); i++) 
	{
		RETypedPtr subDict(subviews->at(i));
		if (RETypedPtr::isNotEmpty(&subDict, REPtrTypeDictionary)) 
		{
			REGUIApplication * app = REGUIApplication::currentApplication();
			if (app) 
			{
				REView * v = app->createSerializableClassWithDictionary<REView>(subDict);
				if (v) 
				{
					this->addSubview(v);
				}
			}
			else { RELog::log("ERROR: REGUIApplication not initialized"); }
		}
	}
}

void REView::deserializeWithDictionary(const RETypedPtr & dictionary)
{
	REDictionary * dict = dictionary.dictionary();
	if (dict) 
	{
		REString * str = dict->valueForKey("frame").string();
		if (str) { _frame = RERect::fromString(str->UTF8String()); }
		str = dict->valueForKey("color").string();
		if (str) { _color = REColor::fromString(str->UTF8String()); }
		RENumber * num = dict->valueForKey("tag").number();
		if (num) { _tag = num->int32Value(); }
		num = dict->valueForKey("visible").number();
		if (num) { _isVisible = num->boolValue(); }
		num = dict->valueForKey("responduseraction").number();
		if (num) { _isRespondsForUserAction = num->boolValue(); }
		num = dict->valueForKey("interceptuseraction").number();
		if (num) { _isInterceptsUserAction = num->boolValue(); }
		
		RETypedPtr textureDict(dict->valueForKey("texture"));
		REDictionary * texture = textureDict.dictionary();
		if (texture) 
		{
			REGUIApplication * app = REGUIApplication::currentApplication();
			if (app) 
			{
				RETextureObject * t = app->createSerializableClassWithDictionary<RETextureObject>(textureDict);
				this->setTexture(t);
			}
			else { RELog::log("ERROR: REGUIApplication not initialized"); }
		}
		
		RETypedArray * subviews = dict->valueForKey("subviews").array();
		if (subviews) 
		{
			this->deserializeSubviewsFromDictionary(subviews);
		}
	}
}

REView * REView::create()
{
	REView * newView = new REView();
	return newView;
}

REView * REView::createWithFrame(const RERect & frame)
{
	REView * newView = new REView();
	if (newView)
	{
		newView->setFrame(frame);
	}
	return newView;
}

REView * REView::createWithDeserializableDictionary(const RETypedPtr & dictionary)
{
	REView * newView = new REView();
	if (newView) 
	{
		newView->deserializeWithDictionary(dictionary);
	}
	return newView;
}

