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

void RETransformedView::SetTransform(const REAffineTransform & newTransform)
{
	_transform = newTransform;
	RETetragon newTetr(_transform.GetTransformedTetragon(_transformedFrame));
	this->SetTransformedFrame(newTetr);
}

void RETransformedView::SetTransformAnimated(const REAffineTransform & newTransform)
{
	_transform = newTransform;
	RETetragon newTetr(_transform.GetTransformedTetragon(_transformedFrame));
	if (REAnimation::IsSetuping()) 
	{
		this->SetTransformedFrameAnimated(newTetr);
	}
	else
	{
		this->SetTransformedFrame(newTetr);
	}
}

const RETetragon & RETransformedView::GetTransformedFrame() const
{
	return _transformedFrame;
}

void RETransformedView::SetTransformedFrame(const RETetragon & newFrame)
{
	_transformedFrame = newFrame;
}

void RETransformedView::SetTransformedFrameAnimated(const RETetragon & newFrame)
{
	if (REAnimation::IsSetuping()) 
	{
		for (REUInt32 i = 0; i < 8; i++) 
		{
			if (_transformedFrame.arr[i] != newFrame.arr[i]) 
			{
				REAnimation::AddFloatParam(this, &_animationsCount, &_transformedFrame.arr[i], _transformedFrame.arr[i], newFrame.arr[i]);
			}
		}
	}
	else
	{
		this->SetTransformedFrame(newFrame);
	}
}

void RETransformedView::SetFrame(const RERect & newViewFrame)
{
	RETetragon tetrFrame(newViewFrame);
	this->SetTransformedFrame(tetrFrame);
}

void RETransformedView::SetFrameAnimated(const RERect & newViewFrame)
{
	if (REAnimation::IsSetuping()) 
	{
		RETetragon tetrFrame(newViewFrame);
		this->SetTransformedFrameAnimated(tetrFrame);
	}
	else
	{
		this->SetFrame(newViewFrame);
	}
}

const REUInt32 RETransformedView::GetClassIdentifier() const
{
	return RETransformedView::ClassIdentifier();
}

const REUInt32 RETransformedView::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("RETransformedView");
	return clasIdentif;
}

REBOOL RETransformedView::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((RETransformedView::ClassIdentifier() == classIdentifier) || REView::IsImplementsClass(classIdentifier));
}

/* IRERenderable */
void RETransformedView::Render()
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
		
		this->RenderSubViews(_frame.x, _frame.y);
	}
}

void RETransformedView::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	if (_isVisible)
	{
		if (_color.alpha > 0.0f) 
		{
			RETetragon rectWithOffset(_transformedFrame);
			rectWithOffset.Translate(offsetX, offsetY);
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
		
		this->RenderSubViews(_transformedFrame.topLeftX + offsetX, _transformedFrame.topLeftY + offsetY);
	}
}

RETransformedView::RETransformedView() : REView()
{
	_transform.ToIdentity();
}

RETransformedView::~RETransformedView()
{
	
}

RETransformedView * RETransformedView::Create()
{
	RETransformedView * newView = new RETransformedView();
	return newView;
}


