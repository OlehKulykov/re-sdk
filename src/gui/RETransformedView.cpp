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
#include "../../include/regui/private/REAnimationController.h"


const REAffineTransform & RETransformedView::GetTransform() const
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

const RETetragon & RETransformedView::getTransformedFrame() const
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
	RETetragon tetrFrame(newViewFrame);
	this->setTransformedFrame(tetrFrame);
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
			RERenderDevice * device = RERenderDevice::GetDefaultDevice();
			if (_texture) 
			{
				device->RenderRect(_transformedFrame, _color, _texture->GetFrame(), _texture->GetTexture());
			}
			else
			{
				static const RETetragon defaultTetr;
				device->RenderRect(_transformedFrame, _color, defaultTetr, NULL);
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
			RERenderDevice * device = RERenderDevice::GetDefaultDevice();
			if (_texture) 
			{
				device->RenderRect(rectWithOffset, _color, _texture->GetFrame(), _texture->GetTexture());
			}
			else
			{
				static const RETetragon defaultTetr;
				device->RenderRect(rectWithOffset, _color, defaultTetr, NULL);
			}
		}
		
		this->renderSubviews(_transformedFrame.topLeftX + offsetX, _transformedFrame.topLeftY + offsetY);
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


