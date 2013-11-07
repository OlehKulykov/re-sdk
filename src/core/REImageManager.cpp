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


#include "../../include/recore/REImageManager.h"
#include "../../include/recore/REMem.h"
#include "../../include/recore/RELog.h"
#include "../../include/recore/REBufferNoCopy.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H)
#include "recore_sdk_config.h"
#endif

#if defined(HAVE_SETJMP_H) 
#include <setjmp.h>
#endif



#ifndef __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__ 

#if defined(HAVE_JPEGLIB_H)
#include <jpeglib.h>
#else 
#define __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__ 1
#endif

#endif /* __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__  */



#ifndef __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__

#if defined(HAVE_WEBP_TYPES_H) || defined(HAVE_WEBP_MUX_H) || defined(HAVE_WEBP_DECODE_H) || defined(HAVE_WEBP_ENCODE_H)

#if defined(HAVE_WEBP_TYPES_H)
#include <webp/types.h>
#endif

#if defined(HAVE_WEBP_MUX_H)
#include <webp/mux.h>
#endif

#if defined(HAVE_WEBP_DECODE_H)
#include <webp/decode.h>
#endif

#if defined(HAVE_WEBP_ENCODE_H)
#include <webp/encode.h>
#endif

#else
#define __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__ 1
#endif

#endif /* __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__  */




#ifndef __RE_RECORE_NO_PNG_IMAGE_SUPPORT__

#if defined(HAVE_PNG_H)
#include <png.h>
#else
#define __RE_RECORE_NO_PNG_IMAGE_SUPPORT__ 1
#endif

#endif /* __RE_RECORE_NO_PNG_IMAGE_SUPPORT__ */

class REImageManagerPrivate
{	
public:
#ifndef __RE_RECORE_NO_PNG_IMAGE_SUPPORT__	
	typedef struct _PNGReadWriteStruct
	{
		void * data;
		REUInt32 dataOffset;
	} PNGReadWriteStruct;
	static REPtr<REImageBase> loadPNG(const REUByte * fileData, const REUInt32 dataSize);
	static REPtr<REBuffer> createPNGFilePresentation(const REUByte * pixelsData,
													 const REUInt32 width,
													 const REUInt32 height,
													 const REImagePixelFormat pixelFormat);
	static void PNGReadCallBack(png_structp png_ptr, png_bytep data, png_size_t length);
	static void PNGErrorCallBack(png_structp png_ptr, png_const_charp error_msg);
	static void PNGWarningCallBack(png_structp png_ptr, png_const_charp warning_msg);
	static void PNGWriteDataCallBack(png_structp png_ptr, png_bytep data, png_size_t length);
	static void PNGFlushDataCallBack(png_structp png_ptr);
#endif
	
#ifndef __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__ 	
	static REPtr<REImageBase> loadJPG(const REUByte * fileData, const REUInt32 dataSize);
	static REPtr<REBuffer> createJPEGFilePresentation(const REUByte * pixelsData,
													  const REUInt32 width,
													  const REUInt32 height,
													  const REImagePixelFormat pixelFormat,
													  const REFloat32 quality);
#endif 	
	
#ifndef __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__ 	
	static void freeWebPEncodedBuffer(void * buff);
	static REPtr<REImageBase> loadWebP(const REUByte * fileData, const REUInt32 dataSize);
	static REPtr<REBuffer> createWebPFilePresentation(const REUByte * pixelsData,
													  const REUInt32 width,
													  const REUInt32 height,
													  const REImagePixelFormat pixelFormat,
													  const REFloat32 quality);
#endif
};

#ifndef __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__ 	
REPtr<REImageBase> REImageManagerPrivate::loadWebP(const REUByte * fileData, const REUInt32 dataSize)
{
	WebPDecoderConfig config;
	WebPInitDecoderConfig(&config);
	
	if (WebPGetFeatures((const uint8_t *)fileData, (uint32_t)dataSize, &config.input) == VP8_STATUS_OK)
	{
		REImageBase * newBase = NULL;
		REUInt32 channelsCount = 0;
		if (config.input.has_alpha) 
		{
			channelsCount = 4;
			newBase = new REImageBase(REImagePixelFormatR8G8B8A8, (REUInt32)config.input.width, (REUInt32)config.input.height);
			config.output.colorspace = MODE_RGBA;
		}
		else
		{
			channelsCount = 3;
			newBase = new REImageBase(REImagePixelFormatR8G8B8, (REUInt32)config.input.width, (REUInt32)config.input.height);
			config.output.colorspace = MODE_RGB;
		}
		
		if (newBase) 
		{
			if (newBase->isNull() || (newBase->channelsCount() != channelsCount)) 
			{
				delete newBase;
				return REPtr<REImageBase>();
			}
			
			config.output.u.RGBA.rgba = (uint8_t *)newBase->imageData();
			config.output.u.RGBA.size = (int)newBase->imageDataSize();
			config.output.u.RGBA.stride = channelsCount * config.input.width;
			config.output.is_external_memory = 1;
			
			if (WebPDecode((const uint8_t *)fileData, (uint32_t)dataSize, &config) == VP8_STATUS_OK)
			{
				return REPtr<REImageBase>(newBase);
			}
			delete newBase;
		}
	}
	return REPtr<REImageBase>();
}

void REImageManagerPrivate::freeWebPEncodedBuffer(void * buff)
{
	free(buff);
}

REPtr<REBuffer> REImageManagerPrivate::createWebPFilePresentation(const REUByte * pixelsData,
																  const REUInt32 width,
																  const REUInt32 height,
																  const REImagePixelFormat pixelFormat,
																  const REFloat32 quality)
{
	size_t outSize = 0;
	uint8_t * outBuff = NULL;
	if (quality > 100.f) 
	{
		switch (pixelFormat) 
		{
			case REImagePixelFormatR8G8B8:
				outSize = WebPEncodeLosslessRGB((const uint8_t *)pixelsData,
												(int)width, 
												(int)height,
												(int)REImageBase::rowStride(pixelFormat, width),
												&outBuff);
				break;
				
			case REImagePixelFormatR8G8B8A8:
				outSize = WebPEncodeLosslessRGBA((const uint8_t *)pixelsData,
												 (int)width, 
												 (int)height, 
												 (int)REImageBase::rowStride(pixelFormat, width),
												 &outBuff);
				break;
				
			default:
				break;
		}
	}
	else
	{
		switch (pixelFormat) 
		{
			case REImagePixelFormatR8G8B8:
				outSize = WebPEncodeRGB((const uint8_t *)pixelsData,
										(int)width, 
										(int)height, 
										(int)REImageBase::rowStride(pixelFormat, width),
										(float)quality, 
										&outBuff);
				break;
				
			case REImagePixelFormatR8G8B8A8:
				outSize = WebPEncodeRGBA((const uint8_t *)pixelsData,
										 (int)width, 
										 (int)height, 
										 (int)REImageBase::rowStride(pixelFormat, width),
										 (float)quality, 
										 &outBuff);
				break;
				
			default:
				break;
		}
	}
	
	if (outBuff && (outSize > 0)) 
	{
		REBufferNoCopy * newBuff = new REBufferNoCopy(outBuff, (REUInt32)outSize, REImageManagerPrivate::freeWebPEncodedBuffer);
		if (newBuff) 
		{
			if (newBuff->size() == outSize) 
			{
				return REPtr<REBuffer>(newBuff);
			}
			
			delete newBuff;
		}
	}
	
	RE_SAFE_FREE(outBuff);
	return REPtr<REBuffer>();
}

#endif

#ifndef __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__ 	
REPtr<REImageBase> REImageManagerPrivate::loadJPG(const REUByte * fileData, const REUInt32 dataSize)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr pub;
	
#if defined(HAVE_SETJMP_H) 
	jmp_buf setjmp_buffer;
#endif
	
	cinfo.err = jpeg_std_error(&pub);
	
#if defined(HAVE_SETJMP_H) 
	if (setjmp(setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		return REPtr<REImageBase>();
	}
#endif	
	
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, (unsigned char *)fileData, dataSize);
	jpeg_read_header(&cinfo, (boolean)true);
	jpeg_start_decompress(&cinfo);
	
	
	const REUInt32 width = (REUInt32)cinfo.output_width;
	const REUInt32 height = (REUInt32)cinfo.output_height;
	const REUByte channelsCount = (REUByte)cinfo.output_components;
	REImagePixelFormat pixelFormat = REImagePixelFormatNONE;
	switch (channelsCount) 
	{
		case 3:
			pixelFormat = REImagePixelFormatR8G8B8;
			break;
		case 4:
			pixelFormat = REImagePixelFormatR8G8B8A8;
			break;
		default:
			jpeg_finish_decompress(&cinfo);
			jpeg_destroy_decompress(&cinfo);
			return REPtr<REImageBase>();
			break;
	}
	
	REImageBase * newBase = new REImageBase(pixelFormat, width, height);
	if (!newBase) 
	{
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		return REPtr<REImageBase>();
	}
	if ( newBase->isNull() || (newBase->channelsCount() != channelsCount) ) 
	{
		delete newBase;
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		return REPtr<REImageBase>();
	}
	
	const REUInt32 rowStride = width * channelsCount;
	REUByte * imageData = newBase->imageData();
	JSAMPROW rowPointer[1];
	for (REUInt32 rowIndex = 0; rowIndex < height; rowIndex++)
	{
		rowPointer[0] = &imageData[rowIndex * rowStride];
		jpeg_read_scanlines(&cinfo, rowPointer, 1);
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	
	return REPtr<REImageBase>(newBase);
}

REPtr<REBuffer> REImageManagerPrivate::createJPEGFilePresentation(const REUByte * pixelsData,
																  const REUInt32 width,
																  const REUInt32 height,
																  const REImagePixelFormat pixelFormat,
																  const REFloat32 quality)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	
	unsigned char * outbuffer = NULL; 
	unsigned long outsize = 0;
	jpeg_mem_dest(&cinfo, &outbuffer, &outsize);
	
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	
	jpeg_set_defaults(&cinfo);
	
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
	
	/* TRUE ensures that we will write a complete interchange-JPEG file.
	 * Pass TRUE unless you are very sure of what you're doing.
	 */
	jpeg_start_compress(&cinfo, TRUE);
	
	JSAMPROW * row_pointers = new JSAMPROW[height];
	if (!row_pointers) 
	{
		jpeg_finish_compress(&cinfo);
		jpeg_destroy_compress(&cinfo);
		return REPtr<REBuffer>();
	}
	
	REUByte * imageData = const_cast<REUByte *>(pixelsData);
	const REUInt32 rowStride = REImageBase::rowStride(pixelFormat, width);
	for (REUInt32 i = 0; i < height; i++) 
	{
		row_pointers[i] = imageData;
		imageData += rowStride;
	}
	
	jpeg_write_scanlines(&cinfo, row_pointers, height);
	delete [] row_pointers;	
	
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	
	if (outbuffer && (outsize > 0)) 
	{
		REBufferNoCopy * newBuff = new REBufferNoCopy(outbuffer, (REUInt32)outsize);
		if (newBuff) 
		{
			if (newBuff->size() == outsize)
			{
				return REPtr<REBuffer>(newBuff);
			}
			
			delete newBuff;
		}
	}
	
	RE_SAFE_FREE(outbuffer);
	return REPtr<REBuffer>();
}

#endif /* __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__ */ 

#ifndef __RE_RECORE_NO_PNG_IMAGE_SUPPORT__	
void REImageManagerPrivate::PNGReadCallBack(png_structp png_ptr, png_bytep data, png_size_t length)
{
	REImageManagerPrivate::PNGReadWriteStruct * readStruct = (REImageManagerPrivate::PNGReadWriteStruct *)png_get_io_ptr(png_ptr);//  png_ptr->io_ptr;
	REUByte * ubData = (REUByte *)readStruct->data;
	memcpy(data, &ubData[readStruct->dataOffset], length);
	readStruct->dataOffset += (REUInt32)length;
}

void REImageManagerPrivate::PNGErrorCallBack(png_structp png_ptr, png_const_charp error_msg)
{
	
}

void REImageManagerPrivate::PNGWarningCallBack(png_structp png_ptr, png_const_charp warning_msg)
{
	
}

void REImageManagerPrivate::PNGWriteDataCallBack(png_structp png_ptr, png_bytep data, png_size_t length)
{
	REImageManagerPrivate::PNGReadWriteStruct * writeStruct = (REImageManagerPrivate::PNGReadWriteStruct *)png_get_io_ptr(png_ptr);//  png_ptr->io_ptr;
	REBuffer * buff = (REBuffer *)writeStruct->data;
	if (buff->resize((REUInt32)(buff->size() + length), true)) 
	{
		REUByte * bufferData = (REUByte *)buff->buffer();
		bufferData += writeStruct->dataOffset;
		memcpy(bufferData, data, length);
		writeStruct->dataOffset += length;
	}
}

void REImageManagerPrivate::PNGFlushDataCallBack(png_structp png_ptr)
{
	
}

REPtr<REImageBase> REImageManagerPrivate::loadPNG(const REUByte * fileData, const REUInt32 dataSize)
{
	REImageManagerPrivate::PNGReadWriteStruct readStruct;
	readStruct.data = (REUByte *)fileData;
	readStruct.dataOffset = 0;
	
	png_byte sig[8];
	memcpy(sig, (const png_byte *)fileData, 8);
	
	// Check for valid magic number
	/*
	 if (!png_check_sig (sig, 8))
	 {
	 return false;
	 }
	 */
	char errorString[128] = { 0 };
	
	// Create PNG read struct
	png_structp png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, 
												  errorString, 
												  REImageManagerPrivate::PNGErrorCallBack, 
												  REImageManagerPrivate::PNGWarningCallBack);
	if (png_ptr == NULL) { return REPtr<REImageBase>(); }
	
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL); 
		return REPtr<REImageBase>();
	}
	
#if defined(HAVE_SETJMP_H) 
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return REPtr<REImageBase>();
	}
#endif	
	
	// Set "read" callback function and give source of data
	png_set_read_fn(png_ptr, &readStruct, REImageManagerPrivate::PNGReadCallBack);
	
	// Read png info
	png_read_info(png_ptr, info_ptr);
	
	// Get some usefull information from header
	int bit_depth = png_get_bit_depth (png_ptr, info_ptr);
	int color_type = png_get_color_type (png_ptr, info_ptr);
	
	// Convert index color images to RGB images
	if (color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(png_ptr);
	}
	
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_ptr);
	}
	
	// Convert high bit colors to 8 bit colors
	if (bit_depth == 16)
	{
		png_set_strip_16(png_ptr);
	}
	
	// Convert gray color to true color
	if ((color_type == PNG_COLOR_TYPE_GRAY) || (color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
	{
		png_set_gray_to_rgb(png_ptr);
	}
	
	// Update the changes
	png_read_update_info(png_ptr, info_ptr);
	
	const REUInt32 width = (REUInt32)png_get_image_width(png_ptr, info_ptr);
	const REUInt32 height = (REUInt32)png_get_image_height(png_ptr, info_ptr);
	const REUInt32 channelsCount = (REUInt32)png_get_channels(png_ptr, info_ptr);
	//d.bitsPerPixel = png_get_bit_depth(png_ptr, info_ptr);
	REImagePixelFormat pixelFormat = REImagePixelFormatNONE;
	switch (channelsCount) 
	{
		case 3:
			pixelFormat = REImagePixelFormatR8G8B8;
			break;
		case 4:
			pixelFormat = REImagePixelFormatR8G8B8A8;
			break;
		default:
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			return REPtr<REImageBase>();
			break;
	}
	
	REImageBase * newBase = new REImageBase(pixelFormat, width, height);
	if (!newBase) 
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return REPtr<REImageBase>();
	}
	if ( newBase->isNull() || (newBase->channelsCount() != channelsCount) ) 
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		delete newBase;
		return REPtr<REImageBase>();
	}
	
	png_bytep * row_pointers = new png_bytep[height];	
	if (!row_pointers) 
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		delete newBase;
		return REPtr<REImageBase>();
	}
	
	const size_t bitmapBytesPerRow = (width * channelsCount);
	REUByte * pixelsData = newBase->imageData();
	for (REUInt32 rowIndex = 0; rowIndex < height; rowIndex++)
	{
		row_pointers[rowIndex] = (png_byte*)&pixelsData[bitmapBytesPerRow * rowIndex];
	}
	
	png_read_image(png_ptr, row_pointers);
	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	delete[] row_pointers;
	
	return REPtr<REImageBase>(newBase);
}

REPtr<REBuffer> REImageManagerPrivate::createPNGFilePresentation(const REUByte * pixelsData,
																  const REUInt32 width,
																  const REUInt32 height,
																  const REImagePixelFormat pixelFormat)
{
	REBuffer * file = new REBuffer();
	if (file) 
	{
		REString errString;
		png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
													  &errString, 
													  REImageManagerPrivate::PNGErrorCallBack, 
													  REImageManagerPrivate::PNGWarningCallBack);
		if (!png_ptr)
		{
			delete file;
			return REPtr<REBuffer>();
		}
		
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			delete file;
			png_destroy_write_struct(&png_ptr,  NULL);
			return REPtr<REBuffer>();
		}
		
		REImageManagerPrivate::PNGReadWriteStruct wrStr;
		wrStr.data = file;
		wrStr.dataOffset = 0;
		
		png_set_write_fn(png_ptr, 
						 &wrStr, 
						 REImageManagerPrivate::PNGWriteDataCallBack,
						 REImageManagerPrivate::PNGFlushDataCallBack);
		
		REUInt32 bytes_per_pixel = 0;
		int color_type = 0;
		int bit_depth = 8;
		switch (pixelFormat) 
		{
			case REImagePixelFormatR8G8B8:
				bytes_per_pixel = 3;
				color_type = PNG_COLOR_TYPE_RGB;
				break;
			case REImagePixelFormatR8G8B8A8:
				bytes_per_pixel = 4;
				color_type = PNG_COLOR_TYPE_RGBA;
				break;
			case REImagePixelFormatLuminance8Alpha8:
				bytes_per_pixel = 2;
				color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
				break;
			case REImagePixelFormatAlpha8:
				bytes_per_pixel = 1;
				color_type = PNG_COLOR_TYPE_GRAY;
				break;
			default:
				break;
		}
		
		png_set_IHDR(png_ptr, 
					 info_ptr, 
					 width, 
					 height, 
					 bit_depth, 
					 color_type,
					 PNG_INTERLACE_NONE, 
					 PNG_COMPRESSION_TYPE_BASE, 
					 PNG_FILTER_TYPE_BASE);
		
		png_write_info(png_ptr, info_ptr);
		
		REUByte * pixData = (REUByte *)pixelsData;
		png_bytep * row_pointers = new png_bytep[height];	
		if (row_pointers) 
		{	
			const REUInt32 width__bytes_per_pixel = width * bytes_per_pixel;
			for (REUInt32 k = 0; k < height; k++)
			{
				row_pointers[k] = pixData + (k * width__bytes_per_pixel);
			}
			png_write_image(png_ptr, row_pointers);
			
			png_write_end(png_ptr, info_ptr);
		}
		
		png_destroy_write_struct(&png_ptr, &info_ptr);
		delete[] row_pointers;
	}
	return REPtr<REBuffer>(file);
}
#endif


REPtr<REImageBase> REImageManager::createFromFileData(const REData & fileData)
{
	return this->createFromFileData(fileData.bytes(), fileData.size());
}

REPtr<REImageBase> REImageManager::createFromFileData(const REUByte * fileData, const REUInt32 dataSize)
{
	const REImageType imgType = this->typeFromFileData(fileData, dataSize);
	switch (imgType) 
	{
		case REImageTypePNG:
#ifndef __RE_RECORE_NO_PNG_IMAGE_SUPPORT__			
			return REImageManagerPrivate::loadPNG(fileData, dataSize);
#else
			RELog::log("Loading PNG image format requires undefined flag __RE_RECORE_NO_PNG_IMAGE_SUPPORT__");
#endif	
			break;

		case REImageTypeJPEG:
#ifndef __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__ 			
			return REImageManagerPrivate::loadJPG(fileData, dataSize);
#else
			RELog::log("Loading JPEG image format requires undefined flag __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__");
#endif			
			break;	
						
		case REImageTypeTGA:
		{
			RELog::log("No Targa image support");
		}
			break;
			
		case REImageTypeWEBP:
#ifndef __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__ 			
			return REImageManagerPrivate::loadWebP(fileData, dataSize);
#else
			RELog::log("Loading WebP image format requires undefined flag __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__");
#endif			
			break;
			
		default:
			break;
	}
	return REPtr<REImageBase>();
}

REImageType REImageManager::typeFromFileData(const REUByte * fileData, const REUInt32 dataSize)
{
	if (!fileData || (dataSize < 10))
	{
		return REImageTypeNONE;
	}

	if ((fileData[1] == 80) && (fileData[2] == 78) && (fileData[3] == 71))
	{
			return REImageTypePNG;
	}
	
	const char jpegSignature[4] = {'J', 'F', 'I', 'F'};
	if (memcmp(&fileData[6], jpegSignature, 4) == 0)
	{
		return REImageTypeJPEG;
	}
	
        /*
	if ( stbi_tga_test_memory((stbi_uc const *)fileData, (int)dataSize) ) 
	{
		return REImageTypeTGA;
	}
        */
	
#ifndef __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__ 	
	int width = 0;
	int height = 0;
	const int checkWebP = WebPGetInfo((const uint8_t *)fileData, (uint32_t)dataSize, &width, &height);
	if ((checkWebP != 0) && (width > 1) && (width < 16383) && (height > 1) && (height < 16383)) 
	{
		return REImageTypeWEBP;
	}
#endif
	
	return REImageTypeNONE;
}

REImageType REImageManager::typeFromFileData(const REData & fileData)
{
	return this->typeFromFileData(fileData.bytes(), fileData.hash());
}

REPtr<REBuffer> REImageManager::createPNGFilePresentation(const REUByte * pixelsData,
														  const REImagePixelFormat pixelFormat,
														  const REUInt32 width,
														  const REUInt32 height)
{
#ifndef __RE_RECORE_NO_PNG_IMAGE_SUPPORT__
	return REImageManagerPrivate::createPNGFilePresentation(pixelsData, width, height, pixelFormat);
#else
	RELog::log("\"CreatePNGFilePresentation\" functionality requires undefined flag __RE_RECORE_NO_PNG_IMAGE_SUPPORT__");
	return REPtr<REBuffer>();
#endif
}

REPtr<REBuffer> REImageManager::createWebPFilePresentation(const REUByte * pixelsData,
														   const REImagePixelFormat pixelFormat,
														   const REUInt32 width,
														   const REUInt32 height,
														   const REFloat32 quality)
{
#ifndef __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__  
	return REImageManagerPrivate::createWebPFilePresentation(pixelsData, width, height, pixelFormat, quality);
#else
	RELog::log("\"CreateWebPFilePresentation\" functionality requires undefined flag __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__");
	return REPtr<REBuffer>();
#endif	
}

REPtr<REBuffer> REImageManager::createJPEGFilePresentation(const REUByte * pixelsData,
														   const REImagePixelFormat pixelFormat,
														   const REUInt32 width,
														   const REUInt32 height,
														   const REFloat32 quality)
{
#ifndef __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__  
	return REImageManagerPrivate::createJPEGFilePresentation(pixelsData, width, height, pixelFormat, quality);
#else
	RELog::log("\"createJPEGFilePresentation\" functionality requires undefined flag __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__");
	return REPtr<REBuffer>();
#endif	
}

REImageManager::REImageManager()
{

}

REImageManager::~REImageManager()
{

}

