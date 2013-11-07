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


#include "../../include/recore/REMutableImage.h"
#include "../../include/recore/REColor.h"
#include "../../include/recore/REAffineTransform.h"
#include "../../include/recore/RERect.h"
#include "../../include/recore/RETetragon.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H)
#include "recore_sdk_config.h"
#endif

#include <libart_lgpl/libart.h>
#include <libart_lgpl/art_rgba_rgba_affine.h>
#include <libart_lgpl/art_rgba.h>

REBOOL REMutableImage::applyTransform(const REAffineTransform & transform, 
									  const REColor & backColor)
{
	if (transform.isIdentity()) 
	{
		return true;
	}
	
	if (this->isNull()) 
	{
		return false;
	}
	
	const REImagePixelFormat thisFormat = this->pixelFormat();
	switch (thisFormat) 
	{
		case REImagePixelFormatR8G8B8:
		case REImagePixelFormatR8G8B8A8:
			break;
			
		default:
			return false;
			break;
	}
	
	const REUInt32 thisWidth = this->width();
	const REUInt32 thisHeight = this->height();
	
	RETetragon tetrFrame(RERect(0.0f, 0.0f, thisWidth, thisHeight));
	
	tetrFrame = transform.transformedTetragon(tetrFrame);
	
	RERect bounds(tetrFrame.boundRect());
	
	bounds.x = roundf(bounds.x);
	bounds.y = roundf(bounds.y);
	bounds.width = roundf(bounds.width);
	bounds.height = roundf(bounds.height);
	
	const REUInt32 newWith = bounds.width;
	const REUInt32 newHeight = bounds.height;
	
	REMutableImage newImage(thisFormat, newWith, newHeight);
	if (newImage.isNull())
	{
		return false;
	}
	newImage.fillWithColor(backColor);
	
	ArtAlphaGamma * alphagamma = art_alphagamma_new(1.0);
	if (!alphagamma) 
	{
		return false;
	}
	
	const double affine[6] = { transform.a, transform.b, transform.c, transform.d, transform.tx, transform.ty };
	
	if (thisFormat == REImagePixelFormatR8G8B8) 
	{
		art_rgb_affine(newImage.imageData(),
					   bounds.x,
					   bounds.y,
					   bounds.width,
					   bounds.height,
					   newImage.rowStride(),
					   this->imageData(),
					   thisWidth,
					   thisHeight,
					   this->rowStride(),
					   affine,
					   ART_FILTER_NEAREST,
					   alphagamma);
	}
	else if (thisFormat == REImagePixelFormatR8G8B8A8)
	{
		art_rgba_rgba_affine(newImage.imageData(),
							 bounds.x,
							 bounds.y,
							 bounds.width,
							 bounds.height,
							 newImage.rowStride(),
							 this->imageData(),
							 thisWidth,
							 thisHeight,
							 this->rowStride(),
							 affine,
							 ART_FILTER_NEAREST,
							 alphagamma);
	}
	
	
//	ART_FILTER_NEAREST,
//	ART_FILTER_TILES,
//	ART_FILTER_BILINEAR,
//	ART_FILTER_HYPER
	
	art_alphagamma_free(alphagamma);
	
	_base = newImage._base;
	
	return true;
}

REBOOL REMutableImage::fillWithColor(const REColor & color)
{
	if (this->isNull()) 
	{
		return false;
	}
	
	const REUInt32 width = this->width();
	const REUInt32 height = this->height();
	
	switch (this->pixelFormat()) 
	{
			/// RGB pixel format, where each pixel channel (Red, Green, Blue) is 8 bit integer(byte) in range [0, 255].
		case REImagePixelFormatR8G8B8:
		{
			const REUByte r = (REUByte)roundf(color.red * 255.0f);
			const REUByte g = (REUByte)roundf(color.green * 255.0f);
			const REUByte b = (REUByte)roundf(color.blue * 255.0f);
			const REUInt32 pixelsCount = width * height;
			REUByte * pixels = this->imageData();
			for (REUInt32 i = 0; i < pixelsCount; i++) 
			{
				*pixels++ = r;
				*pixels++ = g;
				*pixels++ = b;
			}
			return true;
		}
			break;
			
			/// RGBA pixel format, where each pixel channel (Red, Green, Blue, Alpha) is 8 bit integer(byte) in range [0, 255].			
		case REImagePixelFormatR8G8B8A8:
		{
			union 
			{
				struct
				{
					REUByte r;
					REUByte g;
					REUByte b;
					REUByte a;
				};
				REUInt32 value;
			} c1;
			c1.r = (REUByte)roundf(color.red * 255.0f);
			c1.g = (REUByte)roundf(color.green * 255.0f);
			c1.b = (REUByte)roundf(color.blue * 255.0f);
			c1.a = (REUByte)roundf(color.alpha * 255.0f);
			const REUInt32 pixelsCount = width * height;
			REUInt32 * pixels = (REUInt32 *)this->imageData();
			for (REUInt32 i = 0; i < pixelsCount; i++) 
			{
				*pixels++ = c1.value;
			}
			return true;
		}
			break;
			
			/// Luma/Alpha pixel format, where each pixel channel (Luminance, Alpha) is 8 bit integer(byte) in range [0, 255].			
		case REImagePixelFormatLuminance8Alpha8:	
		{
			
		}
			break;
			
			/// Alpha pixel format, where each pixel channel (Alpha) is 8 bit integer(byte) in range [0, 255].			
		case REImagePixelFormatAlpha8:	
		{
			
		}
			break;
			
		default:
			break;
	}
	return false;
}

void REMutableImage::writeAtCoords(const REUInt32 x,
								   const REUInt32 y,
								   const REUByte * pixelsData, 
								   const REImagePixelFormat pixelsFormat, 
								   const REUInt32 width, 
								   const REUInt32 height)
{	
	if (this->isNull())
	{
		REImageBase * newBase = new REImageBase(pixelsData, pixelsFormat, width, height);
		if (newBase) 
		{
			if (newBase->isNull()) 
			{
				delete newBase;
				return;
			}
			else
			{
				_base = REPtr<REImageBase>(newBase);
			}
		}
		return;
	}

	const REImagePixelFormat toFormat = this->pixelFormat();
	const REUInt32 toW = this->width();
	const REUInt32 toH = this->height();
	const REUInt32 toBPP = this->bytesPerPixel();
	const REUInt32 toStride = toW * toBPP;
	REUByte * sourceImageData = this->imageData();
//	d += ((stride * y) + (bpp * x));
	
	const REUInt32 fromBPP = REImageBase::bytesPerPixel(pixelsFormat);
	const REUInt32 fromStride = width * fromBPP;
	
	//TODO: optimize
	for (REUInt32 toY = y, fromY = 0; ((fromY < height) && (toY <= toH)); toY++, fromY++) 
	{
		REUByte * to = sourceImageData;  to += ((toStride * toY) + (toBPP * x));
		const REUByte * from = pixelsData; from += (fromStride * fromY);
		for (REUInt32 toX = x, fromX = 0; ((fromX < width) && (toX <= toW)); toX++, fromX++) 
		{
			if ((toFormat == REImagePixelFormatR8G8B8) && (pixelsFormat == REImagePixelFormatR8G8B8A8)) 
			{
				const REInt32 alpha = from[3];
				
				REInt32 v = *to;
				REInt32 c = *from++;
				*to++ = v + (((c - v) * alpha + 0x80) >> 8);
				
				v = *to;
				c = *from++;
				*to++ = v + (((c - v) * alpha + 0x80) >> 8);
				
				v = *to;
				c = *from++;
				*to++ = v + (((c - v) * alpha + 0x80) >> 8);
				
				from++;
//				art_rgb_run_alpha(to, *from, from[1], from[2], (int)from[3], 1);
//				to += toBPP;
//				from += fromBPP;
			}
			else if ((toFormat == REImagePixelFormatR8G8B8A8) && (pixelsFormat == REImagePixelFormatR8G8B8))
			{
				art_rgba_run_alpha(to, *from, from[1], from[2], (int)255, 1);
				to += toBPP;
				from += fromBPP;
			}
			else if ((toFormat == REImagePixelFormatR8G8B8A8) && (pixelsFormat == REImagePixelFormatR8G8B8A8))
			{
				art_rgba_rgba_composite(to, from, 1);
				to += toBPP;
				from += fromBPP;
			}
			else if (toFormat == pixelsFormat)
			{
				for (REUInt32 i = 0; i < toBPP; i++) 
				{
					*to++ = *from++;
				}
			}
		}
	}
}

/// Constructs image from image data(pixels), data size, image format, width and height.
/// 'pixelsData' will be copyed.
REMutableImage::REMutableImage(const REUByte * pixelsData, 
							   const REImagePixelFormat pixelsFormat, 
							   const REUInt32 width,
							   const REUInt32 height) : REImage()
{
	REImageBase * newBase = new REImageBase(pixelsData, pixelsFormat, width, height);
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

/// Constructs image using image pixel format, width and height.
REMutableImage::REMutableImage(const REImagePixelFormat pixelsFormat, 
							   const REUInt32 width, 
							   const REUInt32 height) : REImage()
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

/// 'base' will be retained and modified.
/// 'base' data will not be copyed.
REMutableImage::REMutableImage(const REPtr<REImageBase> & base) : REImage()
{
	_base = base;
}

/// 'base' will be deleted.
/// 'base' data will not be copyed.
REMutableImage::REMutableImage(REImageBase * base) : REImage()
{
	if (base) 
	{
		_base = REPtr<REImageBase>(base);
	}
}

/// Copy contructor.
/// image data will be copyed.
REMutableImage::REMutableImage(const REMutableImage & image) : REImage()
{
	_base = image.copyBase();
}

/// Copy contructor.
/// image data will be copyed.
REMutableImage::REMutableImage(const REImage & image) : REImage()
{
	_base = image.copyBase();
}

/// Creates empty image.
REMutableImage::REMutableImage() : REImage()
{
	
}


REMutableImage::~REMutableImage()
{
	
}


