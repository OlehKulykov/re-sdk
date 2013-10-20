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


#include "../../include/recore/REImageBase.h"
#include "../../include/recore/REMem.h"

REBOOL REImageBase::isNull() const 
{
	return (_format == 0); 
}

REUByte * REImageBase::imageData() const
{
	return (REUByte *)_buffer.buffer(); 
}

const REUInt32 REImageBase::imageDataSize() const
{
	return _buffer.size(); 
}

const REUInt32 REImageBase::width() const
{ 
	return (REUInt32)_width; 
}

const REUInt32 REImageBase::height() const
{ 
	return (REUInt32)_height;
}

const REImagePixelFormat REImageBase::format() const
{ 
	return ((REImagePixelFormat)_format);
}

const REUInt32 REImageBase::bitsPerPixel() const
{ 
	return REImageBase::bitsPerPixel((REImagePixelFormat)_format); 
}

const REUInt32 REImageBase::bytesPerPixel() const
{
	return REImageBase::bytesPerPixel((REImagePixelFormat)_format); 
}

const REUInt32 REImageBase::channelsCount() const
{ 
	return REImageBase::channelsCount((REImagePixelFormat)_format);
}

REUByte * REImageBase::imageDataTopLeftOffset(const REUInt32 x, const REUInt32 y) const
{
	if (_format && _width <= x && _height <= y) 
	{
		const REUInt32 bytesPP = this->bytesPerPixel();
		REUByte * dataPtr = this->imageData();
		dataPtr += ((_width * bytesPP) * y);
		dataPtr += (bytesPP * x);
		return dataPtr;
	}
	return NULL;
}

REImageBase::REImageBase(const REImagePixelFormat pixelsFormat, 
						 const REUInt32 width, 
						 const REUInt32 height) : 
	_width(0),
	_height(0),
	_format(0)
{
	if ((pixelsFormat != REImagePixelFormatNONE) && 
		width &&
		height &&
		(width <= REUInt16Max) && 
		(height <= REUInt16Max) ) 
	{
		const REUInt32 bytesPPixel = REImageBase::bytesPerPixel(pixelsFormat);
		if (bytesPPixel) 
		{
			const REUInt32 dataSize = (width * height * bytesPPixel);
			if (_buffer.resize(dataSize, false)) 
			{
				_width = (REUInt16)width;
				_height = (REUInt16)height;
				_format = (REUByte)pixelsFormat;
			}
		}
	}
}

REImageBase::REImageBase(const REUByte * pixelsData, 
						 const REImagePixelFormat pixelsFormat, 
						 const REUInt32 width, 
						 const REUInt32 height) : 
	_width(0),
	_height(0),
	_format(0)
{
	if (pixelsData && 
		(pixelsFormat != REImagePixelFormatNONE) && 
		width &&
		height &&
		(width <= REUInt16Max) && 
		(height <= REUInt16Max) ) 
	{
		const REUInt32 bytesPPixel = REImageBase::bytesPerPixel(pixelsFormat);
		if (bytesPPixel) 
		{
			const REUInt32 dataSize = (width * height * bytesPPixel);
			if (_buffer.resize(dataSize, false)) 
			{
				memcpy(_buffer.buffer(), pixelsData, dataSize);
				_width = (REUInt16)width;
				_height = (REUInt16)height;
				_format = (REUByte)pixelsFormat;
			}
		}
	}
}

REImageBase::REImageBase() : 
	_width(0),
	_height(0),
	_format(0)
{
	
}

REImageBase::~REImageBase()
{
	
}


REUInt32 REImageBase::channelsCount(const REImagePixelFormat format)
{
	switch (format) 
	{
		case REImagePixelFormatR8G8B8:
			return 3;
			break;
		case REImagePixelFormatR8G8B8A8:
			return 4;
			break;
		case REImagePixelFormatLuminance8Alpha8:
			return 2;
			break;
		case REImagePixelFormatAlpha8:
			return 1;
			break;
		default:
			break;
	}
	return 0;
}

REUInt32 REImageBase::bitsPerPixel(const REImagePixelFormat format)
{
	switch (format) 
	{
		case REImagePixelFormatR8G8B8:
			return 24;
			break;
		case REImagePixelFormatR8G8B8A8:
			return 32;
			break;
		case REImagePixelFormatLuminance8Alpha8:
			return 16;
			break;
		case REImagePixelFormatAlpha8:
			return 8;
			break;
		default:
			break;
	}
	return 0;
}

REUInt32 REImageBase::bytesPerPixel(const REImagePixelFormat format)
{
	switch (format) 
	{
		case REImagePixelFormatR8G8B8:
			return 3;
			break;
		case REImagePixelFormatR8G8B8A8:
			return 4;
			break;
		case REImagePixelFormatLuminance8Alpha8:
			return 2;
			break;
		case REImagePixelFormatAlpha8:
			return 1;
			break;
		default:
			break;
	}
	return 0;
}



