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


#ifndef __REIMAGE_H__
#define __REIMAGE_H__


#include "RECommonHeader.h"
#include "REImageManager.h"
#include "RESize.h"
#include "REImageBase.h"

/// Class of image.
class __RE_PUBLIC_CLASS_API__ REImage
{
private:
	REImageBase * _base;
	
public:
	/// Copy opertor.
	REImage & operator=(const REImage & anotherImage);
	
	/// Clears image object.
	void Clear();
	
	/// Check image is empty.
	REBOOL IsNull() const;
	
	/// Returns image data(pixels).
	REUByte * GetImageData() const;
	
	/// Returns format of image.
	const REImagePixelFormat GetPixelFormat() const;
	
	/// Returns number of bits per pixel.
	const REUInt32 GetBitsPerPixel() const;
	
	/// Returns image width.
	const REUInt32 GetWidth() const;
	
	/// Returns image height.
	const REUInt32 GetHeight() const;
	
	/// Returns number of channels per pixel.
	const REUInt32 GetChannelsCount() const;

	/// Initialize image from file buffer using data buffer and it's size.
	/// Depends of build options can be init PNG, JPG, JPG2000, WEBP images.
	REBOOL InitFromFileDataBuffer(const REUByte * dataBuffer, const REUInt32 dataSize);
	
	/// Initialize image from file data using data.
	/// Depends of build options can be init PNG, JPG, JPG2000, WEBP images.
	REBOOL InitFromFileData(const REData & data);
	
	
	/// This functionality avaiable only with RENetwork and
	/// in config file must be defined __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__ 
	/// Initializing image object from URL String.
	/// Example: http://example.com/image.png for downloading from web.
	/// Example: file:///Volumes/Data/image.png for reading from from file. See file url scemes.
	REBOOL InitFromURLString(const REString & urlString);
	
	/// Scales image to new width and new height.
	REBOOL ScaleToSize(const REUInt32 newWidth, const REUInt32 newHeight);
	
	/// Scales image to new size.
	REBOOL ScaleToSize(const RESize & newSize);

	/// Default constructor.
	REImage();
	
	/// Constructs image from image base object.
	REImage(REImageBase * base);
	
	/// Constructs image from another image.
	REImage(const REImage & anotherImage);
	
	/// Constructs image from image data(pixels), data size, image format, width and height.
	REImage(const REUByte * pixelsData, 
			const REUInt32 pixelsDataSize, 
			const REImagePixelFormat pixelsFormat, 
			const REUInt32 width,
			const REUInt32 height);
	
	/// Constructs image using image pixel format, width and height.
	REImage(const REImagePixelFormat pixelsFormat, 
			const REUInt32 width, 
			const REUInt32 height);
	
	/// Destructor.
	virtual ~REImage(void);
	
	/// Scales image data(pixels).
	static REBOOL ScaleImageData(const REUByte * srcData,
								 const REUInt32 srcWidth, 
								 const REUInt32 srcHeight, 
								 const REUInt32 srcColorComponents, 
								 REUByte * destData, 
								 const REUInt32 destWidth, 
								 const REUInt32 destHeight);
	
	/// Creates and return new image object with image base object or NULL on error.
	/// After using this array call Release().
	static REImage * CreateWithBase(REImageBase * base);
	
	/// Creates and return new image object with another image object or NULL on error.
	/// After using this array call Release().
	static REImage * CreateWithImage(const REImage & anotherImage);
	
	/// Creates and return new image object with image data(pixels), data size, format, width and height or NULL on error.
	/// After using this array call Release().
	static REImage * CreateWithPixelsData(const REUByte * pixelsData, 
										  const REUInt32 pixelsDataSize, 
										  const REImagePixelFormat pixelsFormat, 
										  const REUInt32 width,
										  const REUInt32 height);
	
	/// Creates and return new image object with image format, width and height or NULL on error.
	/// After using this array call Release().
	static REImage * CreateBlankImage(const REImagePixelFormat pixelsFormat, 
									  const REUInt32 width, 
									  const REUInt32 height);
	
	/// Creates and return new image object by reading from file path or NULL on error.
	/// After using this array call Release().
	static REImage * CreateWithFilePath(const REString & filePath);
};

#endif /* __REIMAGE_H__ */

