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

REBOOL REView::AcceptViewStringParameter(REView * view, const char * key, const char * value)
{
    if (strcmp(key, RE_VIEW_XML_TAG_KEY_STRING) == 0)
    {
        int t = 0;
        if (sscanf(value, RE_VIEW_XML_TAG_FORMAT_STRING, &t) == 1)
        {
            view->SetTag((REInt32)t);
            return true;
        }
    }
    else if (strcmp(key, RE_VIEW_XML_VISIBILITY_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_VIEW_XML_VISIBILITY_FORMAT_STRING, &v) == 1)
        {
            view->SetVisible((v != 0));
            return true;
        }
    }
    else if (strcmp(key, RE_VIEW_XML_FRAME_KEY_STRING) == 0)
    {
        RERect f;
        if (sscanf(value, RE_VIEW_XML_FRAME_FORMAT_STRING, &f.x, &f.y, &f.width, &f.height) == 4)
        {
            view->SetFrame(f);
            return true;
        }
    }
    else if (strcmp(key, RE_VIEW_XML_COLOR_KEY_STRING) == 0)
    {
        REColor c;
        if (sscanf(value, RE_VIEW_XML_COLOR_FORMAT_STRING, &c.red, &c.green, &c.blue, &c.alpha) == 4)
        {
            view->SetColor(c);
            return true;
        }
    }
    else if (strcmp(key, RE_VIEW_XML_RESPONDS_USER_ACTION_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_VIEW_XML_RESPONDS_USER_ACTION_FORMAT_STRING, &v) == 1)
        {
            view->SetRespondsForUserAction((v != 0));
            return true;
        }
    }
    else if (strcmp(key, RE_VIEW_XML_INTERCEPTS_USER_ACTION_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_VIEW_XML_INTERCEPTS_USER_ACTION_FORMAT_STRING, &v) == 1)
        {
            view->SetInterceptsUserAction((v != 0));
            return true;
        }
    }
    return false;
}

/* REGUIObject */
REBOOL REView::AcceptStringParameter(const char * key, const char * value)
{
    if (key && value)
    {
        return REView::AcceptViewStringParameter(this, key, value);
    }
    return false;
}

REBOOL REView::AcceptObjectParameter(const char * className, const char * key, REGUIObject * value)
{
	if (value && className && key) 
	{
		if (strcmp(key, RE_VIEW_XML_SUBVIEW_OBJECT_KEY_STRING) == 0) 
		{
			REView * v = (REView *)value;
			this->AddSubView(v);
			value->Release();
			return true;
		}
		else if (strcmp(key, RE_VIEW_XML_TEXTURE_OBJECT_KEY_STRING) == 0) 
		{
			RETextureObject * texture = (RETextureObject *)value;
			this->SetTexture(texture);
			value->Release();
			return true;
		}
	}
	
	return false;
}

const REUInt32 REView::GetClassIdentifier() const
{
	return REView::ClassIdentifier();
}

const REUInt32 REView::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REView");
	return clasIdentif;
}

REBOOL REView::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REView::ClassIdentifier() == classIdentifier) ||
			(REObject::GenerateClassIdentifierFromClassName("RESubViewsContainer") == classIdentifier) ||
			(REObject::GenerateClassIdentifierFromClassName("IRERenderable") == classIdentifier) ||
			(REObject::GenerateClassIdentifierFromClassName("IREUserActionResponder") == classIdentifier) ||
			REGUIObject::IsImplementsClass(classIdentifier));
}

/* IREView */
void REView::Render()
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
		
		this->RenderSubViews(_frame.x, _frame.y);
	}
}

void REView::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
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
		
		this->RenderSubViews(renderX, renderY);
	}
}

void REView::SetFrame(const RERect & newViewFrame)
{	
	_frame = newViewFrame;
}

void REView::SetFrameAnimated(const RERect & newViewFrame)
{
	if (REAnimation::IsSetuping())
	{
		if (_frame.x != newViewFrame.x)
		{
			REAnimation::AddFloatParam(this, &_animationsCount, &_frame.x, _frame.x, newViewFrame.x);
		}
		if (_frame.y != newViewFrame.y)
		{
			REAnimation::AddFloatParam(this, &_animationsCount, &_frame.y, _frame.y, newViewFrame.y);
		}
		if (_frame.width != newViewFrame.width)
		{
			REAnimation::AddFloatParam(this, &_animationsCount, &_frame.width, _frame.width, newViewFrame.width);
		}
		if (_frame.height != newViewFrame.height)
		{
			REAnimation::AddFloatParam(this, &_animationsCount, &_frame.height, _frame.height, newViewFrame.height);
		}
	}
	else
	{
		this->SetFrame(newViewFrame);
	}
}

const RERect & REView::GetFrame() const
{
	return _frame;
}

void REView::SetColor(const REColor & newViewColor)
{
	_color = newViewColor;
}

void REView::SetColorAnimated(const REColor & newViewColor)
{
	if (REAnimation::IsSetuping())
	{
		if (_color.red != newViewColor.red)
		{
			REAnimation::AddFloatParam(this, &_animationsCount, &_color.red, _color.red, newViewColor.red);
		}
		if (_color.green != newViewColor.green)
		{
			REAnimation::AddFloatParam(this, &_animationsCount, &_color.green, _color.green, newViewColor.green);
		}
		if (_color.blue != newViewColor.blue)
		{
			REAnimation::AddFloatParam(this, &_animationsCount, &_color.blue, _color.blue, newViewColor.blue);
		}
		if (_color.alpha != newViewColor.alpha)
		{
			REAnimation::AddFloatParam(this, &_animationsCount, &_color.alpha, _color.alpha, newViewColor.alpha);
		}
	}
	else
	{
		this->SetColor(newViewColor);
	}
}

const REColor & REView::GetColor() const
{
	return _color;
}

void REView::SetAlpha(const REFloat32 newAlpha, const REBOOL isWithSubViews)
{
	if (isWithSubViews) 
	{
		REObjectsArray * subViews = this->GetSubViewsArray();
		if (subViews) 
		{
			for (REUInt32 index = 0; index < subViews->Count(); index++) 
			{
				REView * subView = (REView*)(*subViews)[index];
				subView->SetAlpha(newAlpha, isWithSubViews);
			}
		}
	}
	
	_color.alpha = newAlpha;
}

void REView::SetAlphaAnimated(const REFloat32 newAlpha, const REBOOL isWithSubViews)
{
	if (isWithSubViews) 
	{
		REObjectsArray * subViews = this->GetSubViewsArray();
		if (subViews) 
		{
			for (REUInt32 index = 0; index < subViews->Count(); index++) 
			{
				REView * subView = (REView*)(*subViews)[index];
				subView->SetAlphaAnimated(newAlpha, isWithSubViews);
			}
		}
	}
	
	if (REAnimation::IsSetuping())
	{
		if (_color.alpha != newAlpha)
		{
			REAnimation::AddFloatParam(this,&_animationsCount,&_color.alpha,_color.alpha,newAlpha);
		}
	}
	else
	{
		this->SetAlpha(newAlpha, isWithSubViews);
	}
}

const REFloat32 REView::GetAlpha() const
{
	return _color.alpha;
}

void REView::StopAnimation(const REAnimationStopType stopType, const REBOOL isStopWithSubviews, const REBOOL isNeedCallDelegate)
{
	if ( this->IsAnimating() )
	{
		REAnimation::StopAllAnimations(this, stopType, isNeedCallDelegate);
	}
	
	if (isStopWithSubviews) 
	{
		REObjectsArray * subViews = this->GetSubViewsArray();
		if (subViews) 
		{
			for (REUInt32 index = 0; index < subViews->Count(); index++) 
			{
				REView * subView = (REView*)((*subViews)[index]);
				subView->StopAnimation(stopType, isStopWithSubviews, isNeedCallDelegate);
			}
		}
	}
}

REBOOL REView::RemoveFromParentView()
{
	REView * parent = this->GetParentSubViewsContainer();
	if (parent) 
	{
		parent->RemoveSubView(this);
	}
	return true;
}

RERect REView::GetScreenFrame() const
{
	REView * parent = this->GetParentSubViewsContainer();
    if (parent)
	{
		RERect parentScreenFrame(parent->GetScreenFrame());
		return RERect(_frame.x + parentScreenFrame.x,
					  _frame.y + parentScreenFrame.y,
					  _frame.width,
					  _frame.height);
	}
	return _frame;
}

RETextureObject * REView::GetTexture()
{
	return _texture;
}

void REView::SetTexture(RETextureObject * newTexture)
{
	RE_SAFE_RELEASE(_texture);
	
	if (newTexture)
	{
		_texture = newTexture;
		_texture->Retain();
	}
}

void REView::OnReleased()
{
	this->RemoveFromParentView();
	
	RE_SAFE_RELEASE(_texture);
	
	if (this->IsAnimating())
	{
		REAnimation::StopAllAnimations(this, REAnimationStopTypeImmediately, false);
	}
	
	REGUIObject::OnReleased();
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

REView * REView::Create()
{
	REView * newView = new REView();
	return newView;
}

REView * REView::CreateWithFrame(const RERect & frame)
{
	REView * newView = new REView();
	if (newView)
	{
		newView->SetFrame(frame);
	}
	return newView;
}

const char * REView::GetXMLTagKeyString() { return RE_VIEW_XML_TAG_KEY_STRING; }
const char * REView::GetXMLTagFormatString() { return RE_VIEW_XML_TAG_FORMAT_STRING; }
const char * REView::GetXMLVisibilityKeyString() { return RE_VIEW_XML_VISIBILITY_KEY_STRING; }
const char * REView::GetXMLVisibilityFormatString() { return RE_VIEW_XML_VISIBILITY_FORMAT_STRING; }
const char * REView::GetXMLFrameKeyString() { return RE_VIEW_XML_FRAME_KEY_STRING; }
const char * REView::GetXMLFrameFormatString() { return RE_VIEW_XML_FRAME_FORMAT_STRING; }
const char * REView::GetXMLColorKeyString() { return RE_VIEW_XML_COLOR_KEY_STRING; }
const char * REView::GetXMLColorFormatString() { return RE_VIEW_XML_COLOR_FORMAT_STRING; }
const char * REView::GetXMLRespondsUserActionKeyString() { return RE_VIEW_XML_RESPONDS_USER_ACTION_KEY_STRING; }
const char * REView::GetXMLRespondsUserActionFormatString() { return RE_VIEW_XML_RESPONDS_USER_ACTION_FORMAT_STRING; }
const char * REView::GetXMLInterceptsUserActionKeyString() { return RE_VIEW_XML_INTERCEPTS_USER_ACTION_KEY_STRING; }
const char * REView::GetXMLInterceptsUserActionFormatString() { return RE_VIEW_XML_INTERCEPTS_USER_ACTION_FORMAT_STRING; }
const char * REView::GetXMLSubViewObjectKeyString() { return RE_VIEW_XML_SUBVIEW_OBJECT_KEY_STRING; }
const char * REView::GetXMLTextureObjectKeyString() { return RE_VIEW_XML_TEXTURE_OBJECT_KEY_STRING; }

