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


#ifndef __REMUTABLEIMAGE_H__
#define __REMUTABLEIMAGE_H__


#include "RECommonHeader.h"
#include "REImageManager.h"
#include "RESize.h"
#include "REImageBase.h"
#include "REImage.h"
#include "REPtr.h"
#include "REColor.h"
#include "REAffineTransform.h"

/// Class of mutable image.
class __RE_PUBLIC_CLASS_API__ REMutableImage : public REImage
{
private:

public:
	/// Apply affine transform to the image.
	/// Size of the image will be changed and empty spaces will be filled with background color.
	/// Implemented only for REImagePixelFormatR8G8B8 images.
	REBOOL applyTransform(const REAffineTransform & transform, const REColor & backColor = REColor::blackColor());
	
	/// Fills image with specific color.
	/// Implemented only for REImagePixelFormatR8G8B8 and REImagePixelFormatR8G8B8A8 images
	REBOOL fillWithColor(const REColor & color);
	
	void writeAtCoords(const REUInt32 x,
					   const REUInt32 y,
					   const REUByte * pixelsData, 
					   const REImagePixelFormat pixelsFormat, 
					   const REUInt32 width, 
					   const REUInt32 height);
	
	/// Constructs image from image data(pixels), image format, width and height.
	/// 'pixelsData' will be copyed.
	REMutableImage(const REUByte * pixelsData, 
				   const REImagePixelFormat pixelsFormat, 
				   const REUInt32 width,
				   const REUInt32 height);
	
	/// Constructs image using image pixel format, width and height.
	REMutableImage(const REImagePixelFormat pixelsFormat, 
				   const REUInt32 width, 
				   const REUInt32 height);
	
	/// 'base' will be retained and modified.
	/// 'base' data will not be copyed.
	REMutableImage(const REPtr<REImageBase> & base);
	
	/// 'base' will be deleted.
	/// 'base' data will not be copyed, retained and deleted after using.
	REMutableImage(REImageBase * base);
	
	/// Copy contructor.
	/// image data will be copyed.
	REMutableImage(const REMutableImage & image);
	
	/// Copy contructor.
	/// image data will be copyed.
	REMutableImage(const REImage & image);
	
	/// Creates empty image.
	REMutableImage();
	

	virtual ~REMutableImage();
};

#endif /* __REMUTABLEIMAGE_H__ */

