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


#include "../../include/regui/RETransformedView.h"
#include "../../include/regui/REGUIApplication.h"

#include "../../include/recore/REStaticString.h"
#include "../../include/recore/RELog.h"

#include "../../include/regui/private/REAnimationController.h"


const REAffineTransform & RETransformedView::transform() const
{
	return _transform;
}

void RETransformedView::setTransform(const REAffineTransform & newTransform)
{
	_transform = newTransform;
	RETetragon newTetr(_transform.transformedTetragon(_transformedFrame));
	this->setTransformedFrame(newTetr);
}

void RETransformedView::setTransformAnimated(const REAffineTransform & newTransform)
{
	_transform = newTransform;
	RETetragon newTetr(_transform.transformedTetragon(_transformedFrame));
	if (REAnimation::isSetuping()) 
	{
		this->setTransformedFrameAnimated(newTetr);
	}
	else
	{
		this->setTransformedFrame(newTetr);
	}
}

const RETetragon & RETransformedView::transformedFrame() const
{
	return _transformedFrame;
}

void RETransformedView::setTransformedFrame(const RETetragon & newFrame)
{
	_transformedFrame = newFrame;
}

void RETransformedView::setTransformedFrameAnimated(const RETetragon & newFrame)
{
	if (REAnimation::isSetuping()) 
	{
		for (REUInt32 i = 0; i < 8; i++) 
		{
			if (_transformedFrame.arr[i] != newFrame.arr[i]) 
			{
				REAnimation::addFloatParam(this, &_animationsCount, &_transformedFrame.arr[i], _transformedFrame.arr[i], newFrame.arr[i]);
			}
		}
	}
	else
	{
		this->setTransformedFrame(newFrame);
	}
}

void RETransformedView::setFrame(const RERect & newViewFrame)
{
	this->setTransformedFrame(RETetragon(newViewFrame));
}

void RETransformedView::setFrameAnimated(const RERect & newViewFrame)
{
	if (REAnimation::isSetuping()) 
	{
		RETetragon tetrFrame(newViewFrame);
		this->setTransformedFrameAnimated(tetrFrame);
	}
	else
	{
		this->setFrame(newViewFrame);
	}
}

/* IRERenderable */
void RETransformedView::render()
{
	if (_isVisible)
	{
		if (_color.alpha > 0.0f) 
		{
			RERenderDevice * device = RERenderDevice::defaultDevice();
			if (_texture) 
			{
				device->renderRect(_transformedFrame, _color, _texture->frame(), _texture);
			}
			else
			{
				static const RETetragon defaultTetr;
				device->renderRect(_transformedFrame, _color, defaultTetr, NULL);
			}
		}
		
		this->renderSubviews(_frame.x, _frame.y);
	}
}

void RETransformedView::renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	if (_isVisible)
	{
		if (_color.alpha > 0.0f) 
		{
			RETetragon rectWithOffset(_transformedFrame);
			rectWithOffset.translate(offsetX, offsetY);
			RERenderDevice * device = RERenderDevice::defaultDevice();
			if (_texture) 
			{
				device->renderRect(rectWithOffset, _color, _texture->frame(), _texture);
			}
			else
			{
				static const RETetragon defaultTetr;
				device->renderRect(rectWithOffset, _color, defaultTetr, NULL);
			}
		}
		
		this->renderSubviews(_transformedFrame.topLeftX + offsetX, _transformedFrame.topLeftY + offsetY);
	}
}


#define TYPED_STATIC_STRING(s) RETypedPtr(new REStaticString(s), REPtrTypeString) 
RETypedPtr RETransformedView::serializeToDictionary() const
{
	RETypedPtr d(REView::serializeToDictionary());
	REDictionary * dict = d.dictionary();
	if (dict) 
	{
		dict->setValue(TYPED_STATIC_STRING("RETransformedView"), TYPED_STATIC_STRING(RESerializable::classNameKey()));		
		dict->setValue(RETypedPtr(new REString(REAffineTransform::toString(_transform)), REPtrTypeString), TYPED_STATIC_STRING("transform"));
	}
	return d;
}

void RETransformedView::deserializeWithDictionary(const RETypedPtr & dictionary)
{
	REView::deserializeWithDictionary(dictionary);
	REDictionary * dict = dictionary.dictionary();
	if (dict) 
	{
		REString * str = dict->valueForKey("transform").string();
		if (str) { this->setTransform(REAffineTransform::fromString(str->UTF8String())); }
	}
}

RETransformedView::RETransformedView() : REView()
{
	_transform.toIdentity();
}

RETransformedView::~RETransformedView()
{
	
}

RETransformedView * RETransformedView::create()
{
	RETransformedView * newView = new RETransformedView();
	return newView;
}


