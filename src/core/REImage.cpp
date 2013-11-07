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


#include "../../include/recore/REImage.h"
#include "../../include/recore/REData.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H)
#include "recore_sdk_config.h"
#endif

#ifdef __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
#include "../../include/renetwork.h"
#endif

/// Makes a copy of the base image.
REPtr<REImageBase> REImage::copyBase() const
{
	if (!this->isNull()) 
	{
		REImageBase * newBase = new REImageBase(this->imageData(),
												this->pixelFormat(), 
												this->width(), 
												this->height());
		if (newBase) 
		{
			if (newBase->isNull()) 
			{
				delete newBase;
			}
			else
			{
				return REPtr<REImageBase>(newBase);
			}
		}
	}
	return REPtr<REImageBase>();
}

REImage & REImage::operator=(const REImage & anotherImage)
{
	_base.release();
	_base = anotherImage._base;
	return (*this);
}

REBOOL REImage::isNull() const
{
	return _base.isEmpty();
}

void REImage::clear()
{
	_base.release();
}

REUByte * REImage::imageData() const 
{ 
	return _base.isNotEmpty() ? _base->imageData() : NULL;
}

const REImagePixelFormat REImage::pixelFormat() const 
{ 
	return _base.isNotEmpty() ? _base->format() : REImagePixelFormatNONE;
}

const REUInt32 REImage::bitsPerPixel() const 
{ 
	return _base.isNotEmpty() ? _base->bitsPerPixel() : 0;
}

const REUInt32 REImage::bytesPerPixel() const
{
	return _base.isNotEmpty() ? _base->bytesPerPixel() : 0;
}

const REUInt32 REImage::width() const 
{ 
	return _base.isNotEmpty() ? _base->width() : 0;
}

const REUInt32 REImage::height() const 
{
	return _base.isNotEmpty() ? _base->height() : 0;
}

const REUInt32 REImage::channelsCount() const 
{
	return _base.isNotEmpty() ? _base->channelsCount() : 0;
}

const REUInt32 REImage::rowStride() const
{
	return _base.isNotEmpty() ? _base->rowStride() : 0;
}

REBOOL REImage::initFromFileDataBuffer(const REUByte * dataBuffer, const REUInt32 dataSize)
{
	_base.release();

	REImageManager manager;
	_base = manager.createFromFileData(dataBuffer, dataSize);
	
	return _base.isNotEmpty();
}

REBOOL REImage::initFromFileData(const REData & data)
{
	return this->initFromFileDataBuffer(data.bytes(), data.size());
}

REBOOL REImage::initFromFilePath(const REString & filePath)
{
	REData data;
	if (data.initFromPath(filePath)) 
	{
		this->initFromFileDataBuffer(data.bytes(), data.size());
	}
	else
	{
		_base.release();
	}
	return false;
}

/// __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
REBOOL REImage::initFromURLString(const REString & urlString)
{
	_base.release();
	
#ifdef __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
	REURL url(urlString);
	if (url.IsFileURL()) 
	{
		REString hiPart(url.GetHierarchicalPart());
		REData data;
		if (data.InitFromPath(hiPart)) 
		{
			return this->InitFromFileData(data);
		}
	}
	else
	{
		REURLRequestObject * request = REURLRequestObject::CreateWithURL(url);
		if (request) 
		{
			REBuffer downlBuff;
			const REBOOL isSended = REURLConnectionObject::SendRequest(request, &downlBuff, NULL);
			request->release();
			if (isSended)
			{
				return this->InitFromFileDataBuffer((const REUByte *)downlBuff.GetBuffer(), downlBuff.GetSize());
			}
		}
	}
#endif	
	return false;
}

REBOOL REImage::scaleToSize(const REUInt32 newWidth, const REUInt32 newHeight)
{
	if (_base.isEmpty()) 
	{
		return false; 
	}

	if ((_base->width() == newWidth) && (_base->height() == newHeight) )
	{
		return true;
	}

	REImageBase * newBase = new REImageBase(_base->format(), newWidth, newHeight);
	if (!newBase) 
	{
		return false; 
	}
	
	if (newBase->isNull()) 
	{
		delete newBase;
		return false; 
	}
	
	const REBOOL isScaled = REImage::scaleImageData(_base->imageData(),
													_base->width(), 
													_base->height(), 
													_base->channelsCount(), 
													newBase->imageData(), 
													newBase->width(), 
													newBase->height());
	if (isScaled)
	{
		_base = REPtr<REImageBase>(newBase);
	}
	else
	{
		delete newBase;
	}

	return isScaled;
}

REBOOL REImage::scaleToSize(const RESize & newSize)
{
	REUInt32 w = (REUInt32)newSize.width;
	REUInt32 h = (REUInt32)newSize.height;
	return this->scaleToSize(w, h);
}

REImage::REImage() : 
	_base(NULL)
{
	
}

REImage::REImage(REImageBase * base) : 
	_base(base)
{

}

REImage::REImage(const REImage & anotherImage) :
	_base(anotherImage._base)
{

}

REImage::REImage(const REUByte * pixelsData, const REUInt32 pixelsDataSize, REImagePixelFormat pixelsFormat, const REUInt32 width, const REUInt32 height)
{
	REImageBase * newBase = new REImageBase(pixelsData,
											pixelsFormat, 
											width, 
											height);
	if (newBase) 
	{
		if (newBase->isNull()) 
		{
			delete newBase;
		}
		else
		{
			_base = REPtr<REImageBase>(newBase);
		}
	}
}

REImage::REImage(const REImagePixelFormat pixelsFormat, const REUInt32 width, const REUInt32 height)
{
	REImageBase * newBase = new REImageBase(pixelsFormat, width, height);
	if (newBase) 
	{
		if (newBase->isNull()) 
		{
			delete newBase;
		}
		else
		{
			_base = REPtr<REImageBase>(newBase);
		}
	}
}

REImage::~REImage()
{
	_base.release();
}

REBOOL REImage::scaleImageData(const REUByte * srcData,
							   const REUInt32 srcWidth, 
							   const REUInt32 srcHeight, 
							   const REUInt32 srcColorComponents, 
							   REUByte * destData, 
							   const REUInt32 destWidth, 
							   const REUInt32 destHeight)
{
	float dx, dy;
	REUInt32 x, y, c;
	bool bagInParams = false;
	bagInParams |= (srcWidth < 1);
	bagInParams |= (srcHeight < 1);
	bagInParams |= (destWidth < 2);
	bagInParams |= (destHeight < 2);
	bagInParams |= (destHeight < 2);
	bagInParams |= (srcData == 0);
	bagInParams |= (destData == 0);
	if ( bagInParams )
	{
		return false;
	}
	dx = (srcWidth - 1.0f) / (destWidth - 1.0f);
	dy = (srcHeight - 1.0f) / (destHeight - 1.0f);
	for ( y = 0; y < destHeight; ++y )
	{
		float sampley = y * dy;
		REUInt32 inty = (REUInt32)sampley;
		if( inty > (srcHeight - 2) ) 
		{ 
			inty = srcHeight - 2; 
		}
		sampley -= inty;
		for ( x = 0; x < destWidth; ++x )
		{
			float samplex = x * dx;
			REUInt32 intx = (REUInt32)samplex;
			REInt32 base_index;
			if( intx > (srcWidth - 2) ) 
			{ 
				intx = srcWidth - 2; 
			}
			samplex -= intx;
			base_index = ((inty * srcWidth) + intx) * srcColorComponents;
			for ( c = 0; c < srcColorComponents; ++c )
			{
				float value = 0.5f;
				value += srcData[base_index] * (1.0f-samplex) * (1.0f-sampley);
				value += srcData[base_index+srcColorComponents] * (samplex) * (1.0f-sampley);
				value += srcData[base_index+(srcWidth*srcColorComponents)] * (1.0f-samplex) * sampley;
				value += srcData[base_index+(srcWidth*srcColorComponents)+srcColorComponents] * samplex * sampley;
				++base_index;
				destData[(y*destWidth*srcColorComponents)+(x*srcColorComponents)+c] = (REUByte)(value);
			}
		}
	}
	return true;	
}


REImage * REImage::createWithBase(REImageBase * base)
{
	REImage * newImage = new REImage(base);
	if (newImage) 
	{
		if (newImage->isNull()) 
		{
			delete newImage;
		}
		else
		{
			return newImage;
		}
	}
	return NULL;
}

REImage * REImage::createWithImage(const REImage & anotherImage)
{
	REImage * newImage = new REImage(anotherImage);
	if (newImage) 
	{
		if (newImage->isNull()) 
		{
			delete newImage;
		}
		else
		{
			return newImage;
		}
	}
	return NULL;
}

REImage * REImage::createWithPixelsData(const REUByte * pixelsData, 
										const REUInt32 pixelsDataSize, 
										const REImagePixelFormat pixelsFormat, 
										const REUInt32 width,
										const REUInt32 height)
{
	REImage * newImage = new REImage(pixelsData, pixelsDataSize, pixelsFormat, width, height);
	if (newImage) 
	{
		if (newImage->isNull()) 
		{
			delete newImage;
		}
		else
		{
			return newImage;
		}
	}
	return NULL;
}

REImage * REImage::createBlankImage(const REImagePixelFormat pixelsFormat, 
									const REUInt32 width, 
									const REUInt32 height)
{
	REImage * newImage = new REImage(pixelsFormat, width, height);
	if (newImage) 
	{
		if (newImage->isNull()) 
		{
			delete newImage;
		}
		else
		{
			return newImage;
		}
	}
	return NULL;
}

REImage * REImage::createWithFilePath(const REString & filePath)
{
	REData data;
	if (data.initFromPath(filePath)) 
	{
		REImage * newImage = new REImage();
		if (newImage) 
		{
			if (newImage->initFromFileData(data)) 
			{
				return newImage;
			}
			delete newImage;
		}
	}
	return NULL;
}


