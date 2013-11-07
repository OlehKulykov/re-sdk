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
#include "REPtr.h"

/// Class of image.
class __RE_PUBLIC_CLASS_API__ REImage
{
protected:
	REPtr<REImageBase> _base;
	
public:
	/// Makes a copy of the base image.
	REPtr<REImageBase> copyBase() const;
	
	/// Copy operator.
	/// 'base' image object retained.
	REImage & operator=(const REImage & anotherImage);
	
	/// Clears image object.
	void clear();
	
	/// Check image is empty.
	REBOOL isNull() const;
	
	/// Returns image data(pixels).
	REUByte * imageData() const;
	
	/// Returns format of image.
	const REImagePixelFormat pixelFormat() const;
	
	/// Returns number of bits per pixel.
	const REUInt32 bitsPerPixel() const;
	
	/// Returns number of bytes per each pixel.
	const REUInt32 bytesPerPixel() const;
	
	/// Returns image width.
	const REUInt32 width() const;
	
	/// Returns image height.
	const REUInt32 height() const;
	
	/// Returns number of channels per pixel.
	const REUInt32 channelsCount() const;

	/// Returns row stride which is bytes per pixel of the current format multiplyed to the current width.
	const REUInt32 rowStride() const;
	
	/// Initialize image from file buffer using data buffer and it's size.
	/// Depends of build options can be init PNG, JPG, WEBP images.
	REBOOL initFromFileDataBuffer(const REUByte * dataBuffer, const REUInt32 dataSize);
	
	/// Initialize image from file data using data.
	/// Depends of build options can be init PNG, JPG, WEBP images.
	REBOOL initFromFileData(const REData & data);
	
	/// Initialize image from file path.
	/// Depends of build options can be init PNG, JPG, WEBP images.
	REBOOL initFromFilePath(const REString & filePath);
	
	/// This functionality avaiable only with RENetwork and
	/// in config file must be defined __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__ 
	/// Initializing image object from URL String.
	/// Example: http://example.com/image.png for downloading from web.
	/// Example: file:///Volumes/Data/image.png for reading from from file. See file url scemes.
	REBOOL initFromURLString(const REString & urlString);
	
	/// Scales image to new width and new height.
	REBOOL scaleToSize(const REUInt32 newWidth, const REUInt32 newHeight);
	
	/// Scales image to new size.
	REBOOL scaleToSize(const RESize & newSize);

	/// Default constructor.
	REImage();
	
	/// Constructs image from image base object.
	/// 'base' will be deleted by this object.
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
	static REBOOL scaleImageData(const REUByte * srcData,
								 const REUInt32 srcWidth, 
								 const REUInt32 srcHeight, 
								 const REUInt32 srcColorComponents, 
								 REUByte * destData, 
								 const REUInt32 destWidth, 
								 const REUInt32 destHeight);
	
	/// Creates and return new image object with image base object or NULL on error.
	/// After using this image delete it.
	static REImage * createWithBase(REImageBase * base);
	
	/// Creates and return new image object with another image object or NULL on error.
	/// After using this image delete it.
	static REImage * createWithImage(const REImage & anotherImage);
	
	/// Creates and return new image object with image data(pixels), data size, format, width and height or NULL on error.
	/// After using this image delete it.
	static REImage * createWithPixelsData(const REUByte * pixelsData, 
										  const REUInt32 pixelsDataSize, 
										  const REImagePixelFormat pixelsFormat, 
										  const REUInt32 width,
										  const REUInt32 height);
	
	/// Creates and return new image object with image format, width and height or NULL on error.
	/// After using this image delete it.
	static REImage * createBlankImage(const REImagePixelFormat pixelsFormat, 
									  const REUInt32 width, 
									  const REUInt32 height);
	
	/// Creates and return new image object by reading from file path or NULL on error.
	/// After using this image delete it.
	static REImage * createWithFilePath(const REString & filePath);
};

#endif /* __REIMAGE_H__ */

