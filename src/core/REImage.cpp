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


#include "../../include/recore/REImage.h"
#include "../../include/recore/REData.h"

#ifdef __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
#include "../../include/renetwork.h"
#endif

REImage & REImage::operator=(const REImage & anotherImage)
{
	this->Clear();
	
	if (_base && anotherImage._base) 
	{
		_base = anotherImage._base;
		_base->Retain();
	}
	
	return (*this);
}

REBOOL REImage::IsNull() const
{
	if (_base) 
	{
		return _base->IsNull(); 
	}	
	return true;
}

void REImage::Clear()
{
	if (_base) 
	{
		_base->Release();
		_base = NULL;
	}
}

REUByte * REImage::GetImageData() const 
{ 
	if (_base) 
	{
		return _base->GetImageData();
	}
	return NULL;
}

const REImagePixelFormat REImage::GetPixelFormat() const 
{ 
	if (_base) 
	{
		return _base->GetFormat(); 
	}
	return REImagePixelFormatNONE;
}

const REUInt32 REImage::GetBitsPerPixel() const 
{ 
	if (_base) 
	{
		return _base->GetBitsPerPixel();
	}
	return 0;
}

const REUInt32 REImage::GetWidth() const 
{ 
	if (_base) 
	{
		return _base->GetWidth();
	}
	return 0;
}

const REUInt32 REImage::GetHeight() const 
{
	if (_base) 
	{
		return _base->GetHeight();
	}
	return 0;
}

const REUInt32 REImage::GetChannelsCount() const 
{
	if (_base) 
	{
		return _base->GetChannelsCount();
	}
	return 0;
}

REBOOL REImage::InitFromFileDataBuffer(const REUByte * dataBuffer, const REUInt32 dataSize)
{
	this->Clear();

	REImageManager manager;
	_base = manager.CreateFromFileData(dataBuffer, dataSize);
	
	return (_base != NULL);
}

REBOOL REImage::InitFromFileData(const REData & data)
{
	return this->InitFromFileDataBuffer(data.GetBytes(), data.GetSize());
}

/// __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
REBOOL REImage::InitFromURLString(const REString & urlString)
{
	this->Clear();
	
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
			request->Release();
			if (isSended)
			{
				return this->InitFromFileDataBuffer((const REUByte *)downlBuff.GetBuffer(), downlBuff.GetSize());
			}
		}
	}
#endif	
	return false;
}

REBOOL REImage::ScaleToSize(const REUInt32 newWidth, const REUInt32 newHeight)
{
	if ( _base == NULL ) { return false; }

	if ((_base->GetWidth() == newWidth) && (_base->GetHeight() == newHeight) ) { return true; }

	REImageBase * newBase = new REImageBase(_base->GetFormat(), newWidth, newHeight);
	if (newBase == NULL) { return false; }
	if (newBase->IsNull()) { return false; }
	
	const REBOOL isScaled = REImage::ScaleImageData(_base->GetImageData(),
													_base->GetWidth(), 
													_base->GetHeight(), 
													_base->GetChannelsCount(), 
													newBase->GetImageData(), 
													newBase->GetWidth(), 
													newBase->GetHeight());
	if (isScaled)
	{
		_base->Release();
		_base = newBase;
	}
	else
	{
		delete newBase;
	}

	return isScaled;
}

REBOOL REImage::ScaleToSize(const RESize & newSize)
{
	REUInt32 w = (REUInt32)newSize.width;
	REUInt32 h = (REUInt32)newSize.height;
	return this->ScaleToSize(w, h);
}

REImage::REImage() : 
	_base(NULL)
{
	
}

REImage::REImage(REImageBase * base) :
	_base(NULL)
{
	if (base) 
	{
		_base = base;
		_base->Retain();
	}
}

REImage::REImage(const REImage & anotherImage) : 
	_base(NULL)
{
	if ( anotherImage._base ) 
	{
		_base = anotherImage._base;
		_base->Retain();
	}
}

REImage::REImage(const REUByte * pixelsData, const REUInt32 pixelsDataSize, REImagePixelFormat pixelsFormat, const REUInt32 width, const REUInt32 height) :
	_base(NULL)
{
	REImageBase * newBase = new REImageBase(pixelsData,
											pixelsFormat, 
											width, 
											height);
	if (newBase) 
	{
		if (newBase->IsNull()) 
		{
			delete newBase;
		}
		else
		{
			_base = newBase;
		}
	}
}

REImage::REImage(const REImagePixelFormat pixelsFormat, const REUInt32 width, const REUInt32 height) :
	_base(NULL)
{
	REImageBase * newBase = new REImageBase(pixelsFormat, width, height);
	if (newBase) 
	{
		if (newBase->IsNull()) 
		{
			delete newBase;
		}
		else
		{
			_base = newBase;
		}
	}
}

REImage::~REImage()
{
	this->Clear();
}

REBOOL REImage::ScaleImageData(const REUByte * srcData,
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


REImage * REImage::CreateWithBase(REImageBase * base)
{
	REImage * newImage = new REImage(base);
	if (newImage) 
	{
		if (newImage->IsNull()) 
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

REImage * REImage::CreateWithImage(const REImage & anotherImage)
{
	REImage * newImage = new REImage(anotherImage);
	if (newImage) 
	{
		if (newImage->IsNull()) 
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

REImage * REImage::CreateWithPixelsData(const REUByte * pixelsData, 
										const REUInt32 pixelsDataSize, 
										const REImagePixelFormat pixelsFormat, 
										const REUInt32 width,
										const REUInt32 height)
{
	REImage * newImage = new REImage(pixelsData, pixelsDataSize, pixelsFormat, width, height);
	if (newImage) 
	{
		if (newImage->IsNull()) 
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

REImage * REImage::CreateBlankImage(const REImagePixelFormat pixelsFormat, 
									const REUInt32 width, 
									const REUInt32 height)
{
	REImage * newImage = new REImage(pixelsFormat, width, height);
	if (newImage) 
	{
		if (newImage->IsNull()) 
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

REImage * REImage::CreateWithFilePath(const REString & filePath)
{
	REData data;
	if (data.InitFromPath(filePath)) 
	{
		REImage * newImage = new REImage();
		if (newImage) 
		{
			if (newImage->InitFromFileData(data)) 
			{
				return newImage;
			}
			delete newImage;
		}
	}
	return NULL;
}


