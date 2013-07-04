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


#include "../../include/regui/RETextureObject.h"
#include "../../include/recore/REThread.h"
#include "../../include/recore/RERect.h"
#include "../../include/regui/RERenderDevice.h"
#include "../../include/regui/REView.h"
#include "../../include/regui/private/REAnimationController.h"


#define RE_TEXTURE_XML_FILTER_KEY_STRING "filter"
#define RE_TEXTURE_XML_PATH_KEY_STRING "path"
#define RE_TEXTURE_XML_FRAME_KEY_STRING "frametetrf"
#define RE_TEXTURE_XML_FRAME_FORMAT_STRING "%f;%f;%f;%f;%f;%f;%f;%f"
#define RE_TEXTURE_XML_FILTER_NONE_KEY_STRING "none"
#define RE_TEXTURE_XML_FILTER_NEAREST_KEY_STRING "nearest"
#define RE_TEXTURE_XML_FILTER_LINEAR_KEY_STRING "linear"
#define RE_TEXTURE_XML_FILTER_MIPMAP_STANDART_KEY_STRING "mipmapstandart"
#define RE_TEXTURE_XML_FILTER_MIPMAP_TRILINEAR_KEY_STRING "mipmaptrilinear"


/* REObject */
const REUInt32 RETextureObject::getClassIdentifier() const
{
	return RETextureObject::classIdentifier();
}

const REUInt32 RETextureObject::classIdentifier()
{
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("RETextureObject");
	return clasIdentif;
}

REBOOL RETextureObject::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((RETextureObject::classIdentifier() == classIdentifier) ||
			(REObject::generateClassIdentifierFromClassName("IRETexture") == classIdentifier) ||
			REGUIObject::isImplementsClass(classIdentifier));
}

RERenderDeviceTextureObject * RETextureObject::CreateNewRenderDeviceTexture()
{
	RERenderDevice * device = RERenderDevice::GetDefaultDevice();
	if (device) 
	{
		return device->CreateNewTexture();
	}
	return NULL;
}

/* REGUIObject */
void RETextureObject::onPrepareGUIObjectForSetuping()
{
	if (_texture)
	{
		_texture->release();
	}
	
	_texture = RETextureObject::CreateNewRenderDeviceTexture();
}

void RETextureObject::onSetupingGUIObjectFinished(const REBOOL isAcceptedByParent)
{
	if (_texture) 
	{
		if (_texture->IsNull()) 
		{
			_texture->release();
			_texture = NULL;
		}
	}
}

REBOOL RETextureObject::AcceptStringParameterForTexture(const char * key, const char * value, RETextureObject * texture)
{
	if (strcmp(key, RE_TEXTURE_XML_FILTER_KEY_STRING) == 0)
	{
		RETextureFilterType filter = RETextureFilterNone;
		if (strcmp(value, RE_TEXTURE_XML_FILTER_NEAREST_KEY_STRING) == 0) { filter = RETextureFilterLinear; }
		else if (strcmp(value, RE_TEXTURE_XML_FILTER_LINEAR_KEY_STRING) == 0) { filter = RETextureFilterLinear; }
		else if (strcmp(value, RE_TEXTURE_XML_FILTER_MIPMAP_STANDART_KEY_STRING) == 0) { filter = RETextureFilterMipmapedStandart; }
		else if (strcmp(value, RE_TEXTURE_XML_FILTER_MIPMAP_TRILINEAR_KEY_STRING) == 0) { filter = RETextureFilterMipmapedTrilinear; }
		if (filter != RETextureFilterNone)
		{
			texture->SetFilterType(filter);
			return true;
		}
		return false;
	}
	else if (strcmp(key, RE_TEXTURE_XML_PATH_KEY_STRING) == 0)
	{
		if (value)
		{
			REString imageFilePath(value);
			return texture->UpdateFromImageFilePath(imageFilePath, texture->GetFilterType());
		}
	}
	else if (strcmp(key, RE_TEXTURE_XML_FRAME_KEY_STRING) == 0)
	{
		RETetragon f;
		if (sscanf(value, RE_TEXTURE_XML_FRAME_FORMAT_STRING,
				   &f.arr[0],
				   &f.arr[1],
				   &f.arr[2],
				   &f.arr[3],
				   &f.arr[4],
				   &f.arr[5],
				   &f.arr[6],
				   &f.arr[7]) == 8)
		{
			texture->_frame.set(f);
			return true;
		}
	}
	return false;
}

REBOOL RETextureObject::acceptStringParameter(const char * key, const char * value)
{
	if (key && value)
	{
        return RETextureObject::AcceptStringParameterForTexture(key, value, this);
	}
	return false;
}

REBOOL RETextureObject::UpdateFromImageFilePath(const REString & imageFilePath, const RETextureFilterType filterType)
{
	if (_texture) 
	{
		if (imageFilePath.isEmpty()) { return false; }
		
		REData fileData;
		if (fileData.initFromPath(imageFilePath)) 
		{ 
			REImage image;
			if (image.InitFromFileData(fileData)) 
			{
				fileData.clear();
                return this->UpdateFromImage(image, filterType);
			}
		}
	}
	return false;
}

REBOOL RETextureObject::UpdateFromImage(const REImage & image, const RETextureFilterType filterType)
{
	if (_texture) 
	{
		const REUInt32 inW = image.GetWidth();
		const REUInt32 inH = image.GetHeight();
		if (inW && inH) 
		{
			const REUInt32 w = RETextureObject::GetNearestPowerOfTwo(inW);
			const REUInt32 h = RETextureObject::GetNearestPowerOfTwo(inH);
			if ((inW == w) && (inH == h)) 
			{
				return this->Update(image.GetImageData(), image.GetPixelFormat(), w, h);
			}
			else
			{
				REImage img(image);
				if (img.ScaleToSize(w, h))
				{
					return this->Update(img.GetImageData(), img.GetPixelFormat(), w, h);
				}
			}
		}
	}
	return false;
}

/* RETextureObject */
REBOOL RETextureObject::InitFromImageFilePath(const REString & imageFilePath, const RETextureFilterType filterType)
{
	this->Clear();
	
	_texture = RETextureObject::CreateNewRenderDeviceTexture();
	if (_texture) 
	{
		_texture->SetFilterType(filterType);
		return this->UpdateFromImageFilePath(imageFilePath, filterType);
	}	
	
	return false;
}

REBOOL RETextureObject::InitFromImage(const REImage & image, const RETextureFilterType filterType)
{
	this->Clear();
	
	_texture = RETextureObject::CreateNewRenderDeviceTexture();
	if (_texture) 
	{
		_texture->SetFilterType(filterType);
		return this->UpdateFromImage(image, filterType);
	}
	
	return false;
}

REBOOL RETextureObject::InitBlankTexture(const REImagePixelFormat pixelsFormat,
										 const REUInt32 width,
										 const REUInt32 height,
										 const RETextureFilterType filterType)
{
	this->Clear();
	
	_texture = RETextureObject::CreateNewRenderDeviceTexture();
	if (_texture) 
	{
		_texture->SetFilterType(filterType);
		return true;
	}
	
	return false;
}

RETextureObject & RETextureObject::operator=(const RETextureObject & anotherTexture)
{
	this->Clear();
	
	if (anotherTexture._texture) 
	{
		_texture = anotherTexture._texture;
		_texture->retain();
		_frame = anotherTexture._frame;
	}
	return (*this);
}

void RETextureObject::Clear()
{
	if (_texture) 
	{
		_texture->release();
		_texture = NULL;
	}
	_frame.set(RERect(0.0f, 0.0f, 1.0f, 1.0f));
}

void RETextureObject::onReleased()
{
	RE_SAFE_RELEASE(_texture);
	
	REGUIObject::onReleased();
}

RETextureObject::RETextureObject() : REGUIObject(),
	_texture(NULL)
{
	_frame.set(RERect(0.0f, 0.0f, 1.0f, 1.0f));
}

RETextureObject::~RETextureObject()
{
	
}

/* IRETexture */
#if (defined(__RE_USING_OPENGL_ES__) || defined(__RE_USING_OPENGL__))
const GLuint RETextureObject::GetTexureIdentifier() const
{
	if (_texture) { return _texture->GetTexureIdentifier(); }
	return 0;
}
#endif	

#ifdef __RE_USING_DIRECTX9__ 
IDirect3DDevice9 * RETextureObject::GetD3DDevice9() const
{
	if (_texture) { return _texture->GetD3DDevice9(); }
	return 0;
}

LPDIRECT3DTEXTURE9 RETextureObject::GetDirect3DTexture9() const
{
	if (_texture) { return _texture->GetDirect3DTexture9(); }
	return 0;
}
#endif	

REBOOL RETextureObject::IsNull() const
{
	if (_texture) { return _texture->IsNull(); }
	return true;
}

REBOOL RETextureObject::IsMipmaped() const
{
	if (_texture) { return _texture->IsMipmaped(); }
	return false;
}

REBOOL RETextureObject::Update(const REUByte * pixelsData, 
							   const REImagePixelFormat pixelsFormat,
							   const REUInt32 width,
							   const REUInt32 height)
{
	if (_texture) 
	{
		if (_texture->Update(pixelsData, pixelsFormat, width, height))
		{
			return true;
		}
	}
	return false;
}

void RETextureObject::SetFilterType(const RETextureFilterType filter)
{
	if (_texture) { _texture->SetFilterType(filter); }
}

const RETextureFilterType RETextureObject::GetFilterType() const
{
	if (_texture) { return _texture->GetFilterType(); }
	return RETextureFilterNone;
}

void RETextureObject::SetFrame(const RETetragon & newFrame)
{
	_frame.set(newFrame);
}

void RETextureObject::SetFrameAnimated(const RETetragon & newFrame)
{
	if (REAnimation::isSetuping()) 
	{
		for (REUInt32 i = 0; i < 8; i++) 
		{
			if (_frame.arr[i] != newFrame.arr[i]) 
			{
				REAnimation::addFloatParam(this,
										   NULL,
										   &_frame.arr[i],
										   _frame.arr[i],
										   newFrame.arr[i]);
			}
		}
	}
	else
	{
		this->SetFrame(newFrame);
	}
}

void RETextureObject::StopAnimation(const REAnimationStopType stopType, REBOOL isNeedCallDelegate)
{
	REAnimationController::getDefaultController()->stopAllAnimationForView(this, stopType, isNeedCallDelegate);
}

REUInt32 RETextureObject::GetNearestPowerOfTwo(const REUInt32 inValue)
{
	static const REUInt32 normalSizes[10] = { 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
	REUInt32 leftValue = 0;
	for (REUInt32 i = 0; i < 10; i++)
	{
		const REUInt32 rightValue = normalSizes[i];
		if ( inValue == rightValue )
		{
			return inValue;
		}
		else if ( (leftValue < inValue) && (inValue < rightValue) )
		{
			const REUInt32 leftRange = inValue - leftValue;
			const REUInt32 rightRange = rightValue - inValue;
			if ( leftRange < rightRange )
			{
				return leftValue;
			}
			else
			{
				return rightValue;
			}
		}
		leftValue = rightValue;
	}
	
	return 1024;
}

RETextureObject * RETextureObject::Create()
{
	RETextureObject * newTexture = new RETextureObject();
	if (newTexture) 
	{
		return newTexture;
	}
	return NULL;
}

RETextureObject * RETextureObject::CreateWithBlankTexture(const REImagePixelFormat pixelsFormat,
														  const REUInt32 width,
														  const REUInt32 height,
														  const RETextureFilterType filterType)
{
	RETextureObject * newTexture = new RETextureObject();
	if (newTexture) 
	{
		if (newTexture->InitBlankTexture(pixelsFormat, width, height, filterType)) 
		{
			return newTexture;
		}
		else
		{
			delete newTexture;
		}
	}
	return newTexture;
}

RETextureObject * RETextureObject::CreateWithImageFilePath(const REString & imageFilePath, 
														   const RETextureFilterType filterType)
{
	RETextureObject * newTexture = new RETextureObject();
	if (newTexture) 
	{
		if (newTexture->InitFromImageFilePath(imageFilePath, filterType)) 
		{
			return newTexture;
		}
		else
		{
			delete newTexture;
		}
	}
	return NULL;
}

RETextureObject * RETextureObject::CreateWithImage(const REImage & image, 
												   const RETextureFilterType filterType)
{
	RETextureObject * newTexture = new RETextureObject();
	if (newTexture) 
	{
		if (newTexture->InitFromImage(image, filterType)) 
		{
			return newTexture;
		}
		else
		{
			delete newTexture;
		}
	}
	return NULL;
}

const char * RETextureObject::getXMLFilterKeyString() { return RE_TEXTURE_XML_FILTER_KEY_STRING; }
const char * RETextureObject::getXMLPathKeyString() { return RE_TEXTURE_XML_PATH_KEY_STRING; }
const char * RETextureObject::getXMLFrameKeyString() { return RE_TEXTURE_XML_FRAME_KEY_STRING; }
const char * RETextureObject::getXMLFrameFormatString() { return RE_TEXTURE_XML_FRAME_FORMAT_STRING; }
const char * RETextureObject::getXMLFilterStringByType(const RETextureFilterType filterType)
{
    switch (filterType)
    {
    case RETextureFilterNearest: return RE_TEXTURE_XML_FILTER_NEAREST_KEY_STRING; break;
    case RETextureFilterLinear: return RE_TEXTURE_XML_FILTER_LINEAR_KEY_STRING; break;
    case RETextureFilterMipmapedStandart: return RE_TEXTURE_XML_FILTER_MIPMAP_STANDART_KEY_STRING; break;
    case RETextureFilterMipmapedTrilinear: return RE_TEXTURE_XML_FILTER_MIPMAP_TRILINEAR_KEY_STRING; break;
    default: break;
    };
    return RE_TEXTURE_XML_FILTER_NONE_KEY_STRING;
}




