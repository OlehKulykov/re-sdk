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


#ifndef __REIMAGEMANAGER_H__
#define __REIMAGEMANAGER_H__


#include "RECommonHeader.h"
#include "REData.h"
#include "REImageBase.h"
#include "REPtr.h"
#include "REBuffer.h"

/// Type of image file data.
typedef enum _reImageType
{
	/// Default or undefined.
	REImageTypeNONE = 0,
	
	/// TGA image.
	REImageTypeTGA = 1,
	
	/// PNG image.
	REImageTypePNG = 2,
	
	/// JPEG image.
	REImageTypeJPEG = 3,

	/// WEBP image.
	REImageTypeWEBP = 5
} 
/// Type of image file data.
REImageType;

/// Class using for reading/writing images.
class __RE_PUBLIC_CLASS_API__ REImageManager
{
public:
	/// Creates base image object from image file data.
	REPtr<REImageBase> createFromFileData(const REData & fileData);
	
	/// Creates base image object from image file data with data size.
	REPtr<REImageBase> createFromFileData(const REUByte * fileData, const REUInt32 dataSize);
	
	/// Detects image file data type from it's data with size.
	REImageType typeFromFileData(const REUByte * fileData, const REUInt32 dataSize);
	
	/// Detects image file data type from it's data.
	REImageType typeFromFileData(const REData & fileData);
	
	/// Creates and return new buffer object with PNG file presentation from image data(pixels), width, height and pixel format.
	/// This functionality requires undefined flag __RE_RECORE_NO_PNG_IMAGE_SUPPORT__
	REPtr<REBuffer> createPNGFilePresentation(const REUByte * pixelsData,
											  const REImagePixelFormat pixelFormat,
											  const REUInt32 width,
											  const REUInt32 height);
	
	/// Creates and return new buffer object with WEBP file presentation from image data(pixels), width, height and pixel format.
	/// This functionality requires undefined flag __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__
	/// Use quality in range [0.0f; 100.0f], for lossless encoding provide quality bigger than 100.0f.
	REPtr<REBuffer> createWebPFilePresentation(const REUByte * pixelsData,
											   const REImagePixelFormat pixelFormat,
											   const REUInt32 width,
											   const REUInt32 height,
											   const REFloat32 quality);
	
	/// Creates and return new buffer object with JPEG file presentation from image data(pixels), width, height and pixel format.
	/// This functionality requires undefined flag __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__
	/// Use quality in range [0.0f; 100.0f].
	/// Currently supported only RGB images(REImagePixelFormatR8G8B8 format).
	REPtr<REBuffer> createJPEGFilePresentation(const REUByte * pixelsData,
											   const REImagePixelFormat pixelFormat,
											   const REUInt32 width,
											   const REUInt32 height,
											   const REFloat32 quality);
	
	/// Default constructor.
	REImageManager();
	
	/// Destructor.
	~REImageManager();
};
 

#endif /* __REIMAGEMANAGER_H__ */


