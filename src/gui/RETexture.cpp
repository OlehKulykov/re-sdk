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


#include "../../include/regui/RETexture.h"
#include "../../include/recore/REData.h"
#include "../../include/recore/REImage.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/REStaticString.h"
#include "../../include/recore/RENumber.h"

#include "../../include/regui/private/REAnimationController.h"

#include "../../include/regui/private/RETextureOpenGLPrivate.h"

REUInt32 RETexture::openGLTexureIdentifier() const
{
#if defined(IS_RE_USING_OPENGL)
	return _t.isNotEmpty() ? _t->texureIdentifier() : 0;
#else
	return 0;
#endif	
}

void * RETexture::direct3DDevice9() const
{	
#if defined(IS_RE_USING_OPENGL)
	return NULL;
#else
	REPtrCast<IDirect3DDevice9, void>(...);
	//TODO:
	return NULL;
#endif	
}

void * RETexture::direct3DTexture9() const
{
#if defined(IS_RE_USING_OPENGL)
	return NULL;
#else
	REPtrCast<LPDIRECT3DTEXTURE9, void>(...)
	//TODO:
	return NULL;
#endif	
}

/* REObject */
void RETexture::onReleased()
{
	_t.release();
	
	if (_animationsCounter) 
	{
		REAnimationController::defaultController()->stopAllAnimationForView(this, REAnimationStopTypeImmediately, false);
	}
}

REBOOL RETexture::isNull() const
{
	return _t.isNotEmpty() ? _t->isNull() : true;
}

REBOOL RETexture::isMipmaped() const
{
	return _t.isNotEmpty() ? _t->isMipmaped() : false;
}

REBOOL RETexture::update(const REUByte * pixelsData, 
						 const REImagePixelFormat pixelsFormat,
						 const REUInt32 width,
						 const REUInt32 height)
{
	return _t.isNotEmpty() ? _t->update(pixelsData, pixelsFormat, width, height) : false;
}

const REBOOL RETexture::isBlended() const
{
	return _t.isNotEmpty() ? _t->isBlended() : false;
}

void RETexture::setFilterType(const RETextureFilterType filter)
{
	if (_t.isNotEmpty())
	{
		_t->setFilterType(filter);
	}
}

const RETextureFilterType RETexture::filterType() const
{
	return _t.isNotEmpty() ? _t->filterType() : RETextureFilterNone;
}

void RETexture::bind() const
{
	if (_t.isNotEmpty()) 
	{
		_t->bind();
	}
}

REBOOL RETexture::initWithPixelsData(const REUByte * pixelsData, 
									 const REImagePixelFormat pixelsFormat,
									 const REUInt32 width,
									 const REUInt32 height,
									 const RETextureFilterType filterType)
{
	if (pixelsData && pixelsFormat && width && height && filterType) 
	{
		REPtr<RETextureInternal> prev(_t);
		REPtr<RETextureInternal> t(new RETextureInternal());
		if (t.isNotEmpty()) 
		{
			if (t->initWithPixelsData(pixelsData, pixelsFormat, width, height, filterType))
			{
				_t = t;
				return true;
			}
		}
	}
	return false;
}

REBOOL RETexture::initBlankTexture(const RETextureFilterType filterType)
{
	if (filterType) 
	{
		REPtr<RETextureInternal> prev(_t);
		REPtr<RETextureInternal> t(new RETextureInternal());
		if (t.isNotEmpty()) 
		{
			if (t->initWithPixelsData(NULL, REImagePixelFormatNONE, 0, 0, filterType))
			{
				_t = t;
				return true;
			}
		}
	}
	return false;
}

REBOOL RETexture::initWithImageFilePath(const REString & imageFilePath, 
										const RETextureFilterType filterType)
{
	if (imageFilePath.isNotEmpty() && (filterType != RETextureFilterNone)) 
	{
		REData data;
		if (data.initFromPath(imageFilePath)) 
		{
			REImage image;
			if (image.initFromFileData(data)) 
			{
				data.clear();
				return this->initWithImage(image, filterType);
			}
		}
	}
	return false;
}

REBOOL RETexture::initWithImage(const REImage & image, 
								const RETextureFilterType filterType)
{
	if (!image.isNull() && (filterType != RETextureFilterNone)) 
	{
		const REUInt32 inW = image.width();
		const REUInt32 inH = image.height();
		if (inW && inH) 
		{
			const REUInt32 w = RETexture::nearestPowerOfTwo(inW);
			const REUInt32 h = RETexture::nearestPowerOfTwo(inH);
			if ((inW == w) && (inH == h)) 
			{
				return this->initWithPixelsData(image.imageData(), image.pixelFormat(), w, h, filterType);
			}
			else
			{
				REImage img(image);
				if (img.scaleToSize(w, h))
				{
					return this->initWithPixelsData(img.imageData(), img.pixelFormat(), w, h, filterType);
				}
			}
		}
	}
	return false;
}

#define TYPED_STATIC_STRING(s) RETypedPtr(new REStaticString(s), REPtrTypeString) 

RETypedPtr RETexture::serializeToDictionary() const
{
	RETypedPtr d(RESerializable::serializeToDictionary());
	REDictionary * dict = d.dictionary();
	if (dict) 
	{
		dict->setValue(TYPED_STATIC_STRING("RETexture"), RESerializable::classNameKey());
		dict->setValue(RETypedPtr(new REString(RETetragon::toString(_frame)), REPtrTypeString), TYPED_STATIC_STRING("frame"));
		const RETextureFilterType filter = this->filterType();
		dict->setValue(RETypedPtr(new RENumber((REInt64)filter), REPtrTypeNumber), TYPED_STATIC_STRING("filter"));
	}
	return d;
}

void RETexture::deserializeWithDictionary(const RETypedPtr & dictionary)
{
	RETypedPtr d(dictionary);
	REDictionary * dict = d.dictionary();
	if (dict) 
	{
		REString * str = dict->valueForKey("frame").string();
		if (str) { _frame = RETetragon::fromString(str->UTF8String()); }
		
		RENumber * num = dict->valueForKey("filter").number();
		const RETextureFilterType filter = num ? (RETextureFilterType)num->int32Value() : RETextureFilterLinear;
		
		str = dict->valueForKey(RETexture::imagePathKey()).string();
		if (str) 
		{
			this->initWithImageFilePath(*str, filter);
		}		
	}
}

const RETetragon & RETexture::frame() const
{
	return _frame;
}

void RETexture::setFrame(const RETetragon & newFrame)
{
	_frame = newFrame;
}

void RETexture::setFrameAnimated(const RETetragon & newFrame)
{
	if (REAnimation::isSetuping()) 
	{
		for (REUInt32 i = 0; i < 8; i++) 
		{
			if (_frame.arr[i] != newFrame.arr[i]) 
			{
				REAnimation::addFloatParam(this,
										   &_animationsCounter,
										   &_frame.arr[i],
										   _frame.arr[i],
										   newFrame.arr[i]);
			}
		}
	}
	else
	{
		this->setFrame(newFrame);
	}
}

void RETexture::stopAnimation(const REAnimationStopType stopType, REBOOL isNeedCallDelegate)
{
	if (_animationsCounter) 
	{
		REAnimationController::defaultController()->stopAllAnimationForView(this, stopType, isNeedCallDelegate);
	}
}

RETexture::RETexture() : REObject(), RESerializable(),	
	_animationsCounter(0)
{
	
}

RETexture::~RETexture()
{
	
}

RETexture * RETexture::create()
{
	RETexture * t = new RETexture();
	return t;
}

RETexture * RETexture::createBlankTexture(const RETextureFilterType filterType)
{
	RETexture * t = new RETexture();
	if (t) 
	{
		if (t->initBlankTexture(filterType))
		{
			return t;
		}
		delete t;
	}
	return NULL;
}

const char * RETexture::imagePathKey()
{
	return "path";
}

REUInt32 RETexture::nearestPowerOfTwo(const REUInt32 inValue)
{
	REUInt32 prev = 1;
	for (REUInt32 shift = 1; shift < 18; shift++) 
	{
		const REUInt32 curr = 1 << shift;
		if (curr == inValue) { return curr; }
		
		const REUInt32 next = curr << 1;
		if (inValue > curr && inValue < next) 
		{
			const REUInt32 c = inValue - curr;
			const REUInt32 n = next - inValue;
			if (c < n) { return curr; }
			else { return next; }
		}
		
		prev = curr;
	}
	return 2;
}


