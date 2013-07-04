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


#define RE_VIEW_XML_TAG_KEY_STRING "tag"
#define RE_VIEW_XML_TAG_FORMAT_STRING "%i"
#define RE_VIEW_XML_VISIBILITY_KEY_STRING "visible"
#define RE_VIEW_XML_VISIBILITY_FORMAT_STRING "%i"
#define RE_VIEW_XML_FRAME_KEY_STRING "framef"
#define RE_VIEW_XML_FRAME_FORMAT_STRING "%f;%f;%f;%f"
#define RE_VIEW_XML_COLOR_KEY_STRING "colorrgbaf"
#define RE_VIEW_XML_COLOR_FORMAT_STRING "%f;%f;%f;%f"
#define RE_VIEW_XML_RESPONDS_USER_ACTION_KEY_STRING "responduseraction"
#define RE_VIEW_XML_RESPONDS_USER_ACTION_FORMAT_STRING "%i"
#define RE_VIEW_XML_INTERCEPTS_USER_ACTION_KEY_STRING "interceptuseraction"
#define RE_VIEW_XML_INTERCEPTS_USER_ACTION_FORMAT_STRING "%i"
#define RE_VIEW_XML_SUBVIEW_OBJECT_KEY_STRING "subview"
#define RE_VIEW_XML_TEXTURE_OBJECT_KEY_STRING "texture"

REBOOL REView::acceptViewStringParameter(REView * view, const char * key, const char * value)
{
    if (strcmp(key, RE_VIEW_XML_TAG_KEY_STRING) == 0)
    {
        int t = 0;
        if (sscanf(value, RE_VIEW_XML_TAG_FORMAT_STRING, &t) == 1)
        {
            view->setTag((REInt32)t);
            return true;
        }
    }
    else if (strcmp(key, RE_VIEW_XML_VISIBILITY_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_VIEW_XML_VISIBILITY_FORMAT_STRING, &v) == 1)
        {
            view->setVisible((v != 0));
            return true;
        }
    }
    else if (strcmp(key, RE_VIEW_XML_FRAME_KEY_STRING) == 0)
    {
        RERect f;
        if (sscanf(value, RE_VIEW_XML_FRAME_FORMAT_STRING, &f.x, &f.y, &f.width, &f.height) == 4)
        {
            view->setFrame(f);
            return true;
        }
    }
    else if (strcmp(key, RE_VIEW_XML_COLOR_KEY_STRING) == 0)
    {
        REColor c;
        if (sscanf(value, RE_VIEW_XML_COLOR_FORMAT_STRING, &c.red, &c.green, &c.blue, &c.alpha) == 4)
        {
            view->setColor(c);
            return true;
        }
    }
    else if (strcmp(key, RE_VIEW_XML_RESPONDS_USER_ACTION_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_VIEW_XML_RESPONDS_USER_ACTION_FORMAT_STRING, &v) == 1)
        {
            view->setRespondsForUserAction((v != 0));
            return true;
        }
    }
    else if (strcmp(key, RE_VIEW_XML_INTERCEPTS_USER_ACTION_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_VIEW_XML_INTERCEPTS_USER_ACTION_FORMAT_STRING, &v) == 1)
        {
            view->setInterceptsUserAction((v != 0));
            return true;
        }
    }
    return false;
}

/* REGUIObject */
REBOOL REView::acceptStringParameter(const char * key, const char * value)
{
    if (key && value)
    {
        return REView::acceptViewStringParameter(this, key, value);
    }
    return false;
}

REBOOL REView::acceptObjectParameter(const char * className, const char * key, REGUIObject * value)
{
	if (value && className && key) 
	{
		if (strcmp(key, RE_VIEW_XML_SUBVIEW_OBJECT_KEY_STRING) == 0) 
		{
			REView * v = (REView *)value;
			this->addSubView(v);
			value->release();
			return true;
		}
		else if (strcmp(key, RE_VIEW_XML_TEXTURE_OBJECT_KEY_STRING) == 0) 
		{
			RETextureObject * texture = (RETextureObject *)value;
			this->setTexture(texture);
			value->release();
			return true;
		}
	}
	
	return false;
}

const REUInt32 REView::getClassIdentifier() const
{
	return REView::classIdentifier();
}

const REUInt32 REView::classIdentifier()
{
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("REView");
	return clasIdentif;
}

REBOOL REView::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REView::classIdentifier() == classIdentifier) ||
			(REObject::generateClassIdentifierFromClassName("RESubViewsContainer") == classIdentifier) ||
			(REObject::generateClassIdentifierFromClassName("IRERenderable") == classIdentifier) ||
			(REObject::generateClassIdentifierFromClassName("IREUserActionResponder") == classIdentifier) ||
			REGUIObject::isImplementsClass(classIdentifier));
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
		
		this->renderSubViews(_frame.x, _frame.y);
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
		
		this->renderSubViews(renderX, renderY);
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

const RERect & REView::getFrame() const
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

const REColor & REView::getColor() const
{
	return _color;
}

void REView::setAlpha(const REFloat32 newAlpha, const REBOOL isWithSubViews)
{
	if (isWithSubViews) 
	{
<<<<<<< HEAD
		REArrayObject * subViews = this->getSubViewsArray();
=======
		REObjectsArray * subViews = this->getSubViewsArray();
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
		if (subViews) 
		{
			for (REUInt32 index = 0; index < subViews->count(); index++) 
			{
				REView * subView = (REView*)(*subViews)[index];
				subView->setAlpha(newAlpha, isWithSubViews);
			}
		}
	}
	
	_color.alpha = newAlpha;
}

void REView::setAlphaAnimated(const REFloat32 newAlpha, const REBOOL isWithSubViews)
{
	if (isWithSubViews) 
	{
<<<<<<< HEAD
		REArrayObject * subViews = this->getSubViewsArray();
=======
		REObjectsArray * subViews = this->getSubViewsArray();
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
		if (subViews) 
		{
			for (REUInt32 index = 0; index < subViews->count(); index++) 
			{
				REView * subView = (REView*)(*subViews)[index];
				subView->setAlphaAnimated(newAlpha, isWithSubViews);
			}
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

const REFloat32 REView::getAlpha() const
{
	return _color.alpha;
}

void REView::stopAnimation(const REAnimationStopType stopType, const REBOOL isStopWithSubviews, const REBOOL isNeedCallDelegate)
{
	if ( this->isAnimating() )
	{
		REAnimation::stopAllAnimations(this, stopType, isNeedCallDelegate);
	}
	
	if (isStopWithSubviews) 
	{
<<<<<<< HEAD
		REArrayObject * subViews = this->getSubViewsArray();
=======
		REObjectsArray * subViews = this->getSubViewsArray();
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
		if (subViews) 
		{
			for (REUInt32 index = 0; index < subViews->count(); index++) 
			{
				REView * subView = (REView*)((*subViews)[index]);
				subView->stopAnimation(stopType, isStopWithSubviews, isNeedCallDelegate);
			}
		}
	}
}

REBOOL REView::removeFromParentView()
{
	REView * parent = this->getParentSubViewsContainer();
	if (parent) 
	{
		parent->removeSubView(this);
	}
	return true;
}

RERect REView::getScreenFrame() const
{
	REView * parent = this->getParentSubViewsContainer();
    if (parent)
	{
		RERect parentScreenFrame(parent->getScreenFrame());
		return RERect(_frame.x + parentScreenFrame.x,
					  _frame.y + parentScreenFrame.y,
					  _frame.width,
					  _frame.height);
	}
	return _frame;
}

RETextureObject * REView::getTexture()
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

REView::REView() : REGUIObject(), RESubViewsContainer(),
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

const char * REView::getXMLTagKeyString() { return RE_VIEW_XML_TAG_KEY_STRING; }
const char * REView::getXMLTagFormatString() { return RE_VIEW_XML_TAG_FORMAT_STRING; }
const char * REView::getXMLVisibilityKeyString() { return RE_VIEW_XML_VISIBILITY_KEY_STRING; }
const char * REView::getXMLVisibilityFormatString() { return RE_VIEW_XML_VISIBILITY_FORMAT_STRING; }
const char * REView::getXMLFrameKeyString() { return RE_VIEW_XML_FRAME_KEY_STRING; }
const char * REView::getXMLFrameFormatString() { return RE_VIEW_XML_FRAME_FORMAT_STRING; }
const char * REView::getXMLColorKeyString() { return RE_VIEW_XML_COLOR_KEY_STRING; }
const char * REView::getXMLColorFormatString() { return RE_VIEW_XML_COLOR_FORMAT_STRING; }
const char * REView::getXMLRespondsUserActionKeyString() { return RE_VIEW_XML_RESPONDS_USER_ACTION_KEY_STRING; }
const char * REView::getXMLRespondsUserActionFormatString() { return RE_VIEW_XML_RESPONDS_USER_ACTION_FORMAT_STRING; }
const char * REView::getXMLInterceptsUserActionKeyString() { return RE_VIEW_XML_INTERCEPTS_USER_ACTION_KEY_STRING; }
const char * REView::getXMLInterceptsUserActionFormatString() { return RE_VIEW_XML_INTERCEPTS_USER_ACTION_FORMAT_STRING; }
const char * REView::getXMLSubViewObjectKeyString() { return RE_VIEW_XML_SUBVIEW_OBJECT_KEY_STRING; }
const char * REView::getXMLTextureObjectKeyString() { return RE_VIEW_XML_TEXTURE_OBJECT_KEY_STRING; }

