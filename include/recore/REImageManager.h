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


#ifndef __REIMAGEMANAGER_H__
#define __REIMAGEMANAGER_H__


#include "RECommonHeader.h"
#include "REData.h"
#include "REImageBase.h"

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
	
	/// JPG200 image.
	REImageTypeJPEG2000 = 4,
	
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
	REImageBase * CreateFromFileData(const REData & fileData);
	
	/// Creates base image object from image file data with data size.
	REImageBase * CreateFromFileData(const REUByte * fileData, const REUInt32 dataSize);
	
	/// Detects image file data type from it's data with size.
	REImageType GetTypeFromFileData(const REUByte * fileData, const REUInt32 dataSize);
	
	/// Detects image file data type from it's data.
	REImageType GetTypeFromFileData(const REData & fileData);
	
	/// Creates and return new buffer object with PNG file presentation from image data(pixels), width, height and pixel format.
	/// After using this buffer object call Release().
	/// This functionality requires undefined flag __RE_RECORE_NO_PNG_IMAGE_SUPPORT__
	REBufferObject * CreatePNGFilePresentation(const REUByte * pixelsData,
											   const REUInt32 width,
											   const REUInt32 height,
											   const REImagePixelFormat pixelFormat);
	
	/// Creates and return new buffer object with WEBP file presentation from image data(pixels), width, height and pixel format.
	/// After using this buffer object call Release().
	/// This functionality requires undefined flag __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__
	REBufferObject * CreateWebPFilePresentation(const REUByte * pixelsData,
												const REUInt32 width,
												const REUInt32 height,
												const REImagePixelFormat pixelFormat,
												const REFloat32 quality);
	
	/// Default constructor.
	REImageManager();
	
	/// Destructor.
	~REImageManager();
};
 

#endif /* __REIMAGEMANAGER_H__ */


