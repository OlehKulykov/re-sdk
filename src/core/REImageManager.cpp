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

#ifndef __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__ 

#ifdef __RE_HAVE_SYSTEM_SETJMP_H__ 
#include <setjmp.h>
#endif

#if defined(__RE_USING_ADITIONAL_JPEG_LIBRARY__)
#include "../addlibs/jpeg.h"
#elif defined(__RE_TRY_USE_SYSTEM_JPEG_LIBRARY__)

#ifdef __RE_HAVE_SYSTEM_JPEGLIB_H__ 
#include <jpeglib.h>
#else
#define __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__
#endif

#else
#define __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__
#endif

#endif /* __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__  */


#ifndef __RE_RECORE_NO_JPEG2000_IMAGE_SUPPORT__

#if defined(__RE_USING_ADITIONAL_OPENJPEG_LIBRARY__)
#include "../addlibs/openjpeg.h"
#elif defined(__RE_TRY_USE_SYSTEM_OPENJPEG_LIBRARY__)

#if defined(__RE_HAVE_SYSTEM_OPENJPEG_H__)
#include <openjpeg.h>
#elif defined(__RE_HAVE_SYSTEM_OPENJPEG_OPENJPEG_H__)
#include <openjpeg/openjpeg.h>
#elif defined(__RE_HAVE_SYSTEM_OPENJPEG_2_0_OPENJPEG_H__)
#include <openjpeg-2.0/openjpeg.h>
#elif defined(__RE_HAVE_SYSTEM_OPENJPEG_1_5_OPENJPEG_H__)
#include <openjpeg-1.5/openjpeg.h>
#else
#define __RE_RECORE_NO_JPEG2000_IMAGE_SUPPORT__
#endif

#else
#define __RE_RECORE_NO_JPEG2000_IMAGE_SUPPORT__
#endif

#endif /* __RE_RECORE_NO_JPEG2000_IMAGE_SUPPORT__ */


#ifndef __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__

#if defined(__RE_USING_ADITIONAL_WEBP_LIBRARY__)
#include "../addlibs/webp.h"
#elif defined(__RE_TRY_USE_SYSTEM_WEBP_LIBRARY__)

#if defined(__RE_HAVE_SYSTEM_WEBP_TYPES_H__) && defined(__RE_HAVE_SYSTEM_WEBP_MUX_H__) && defined(__RE_HAVE_SYSTEM_WEBP_DECODE_H__) && defined(__RE_HAVE_SYSTEM_WEBP_ENCODE_H__)
#include <webp/types.h>
#include <webp/mux.h>
#include <webp/decode.h>
#include <webp/encode.h>
#else
#define __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__
#endif

#elif defined(__RE_USING_SYSTEM_WEBP_FRAMEWORK__)
#include <WebP/types.h>
#include <WebP/mux.h>
#include <WebP/decode.h>
#include <WebP/encode.h>
#else
#define __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__
#endif

#endif /* __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__  */


#ifndef __RE_RECORE_NO_PNG_IMAGE_SUPPORT__

#ifdef __RE_HAVE_SYSTEM_SETJMP_H__ 
#include <setjmp.h>
#endif

#if defined(__RE_USING_ADITIONAL_PNG_LIBRARY__)
#include "../addlibs/png.h"
#elif defined(__RE_TRY_USE_SYSTEM_PNG_LIBRARY__)

#ifdef __RE_HAVE_SYSTEM_PNG_H__ 
#include <png.h>
#else
#define __RE_RECORE_NO_PNG_IMAGE_SUPPORT__
#endif

#else 
#define __RE_RECORE_NO_PNG_IMAGE_SUPPORT__
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
#endif 	
	
#ifndef __RE_RECORE_NO_JPEG2000_IMAGE_SUPPORT__ 	
	static REPtr<REImageBase> loadJPEG2000(const REUByte * fileData, const REUInt32 dataSize);
#endif
	
#ifndef __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__ 	
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

REPtr<REBuffer> REImageManagerPrivate::createWebPFilePresentation(const REUByte * pixelsData,
																  const REUInt32 width,
																  const REUInt32 height,
																  const REImagePixelFormat pixelFormat,
																  const REFloat32 quality)
{
	size_t outSize = 0;
	uint8_t * outBuff = NULL;
	switch (pixelFormat) 
	{
		case REImagePixelFormatR8G8B8:
			outSize = WebPEncodeRGB((const uint8_t *)pixelsData,
									(int)width, 
									(int)height, 
									(int)width * 3,
									(float)quality, 
									&outBuff);
			break;
			
		case REImagePixelFormatR8G8B8A8:
			outSize = WebPEncodeRGBA((const uint8_t *)pixelsData,
									 (int)width, 
									 (int)height, 
									 (int)width * 4,
									 (float)quality, 
									 &outBuff);
			break;
			
		default:
			break;
	}
	
	REBuffer * file = NULL;
	if ((outSize > 0) && outBuff) 
	{
		file = new REBuffer(outBuff, (REUInt32)outSize);
	}
	
	if (outBuff) 
	{
		free(outBuff);
	}
	
	return REPtr<REBuffer>(file);
}

#endif

#ifndef __RE_RECORE_NO_JPEG2000_IMAGE_SUPPORT__ 	
REPtr<REImageBase> REImageManagerPrivate::loadJPEG2000(const REUByte * fileData, const REUInt32 dataSize)
{
	opj_dparameters_t parameters;
	opj_set_default_decoder_parameters(&parameters);
	
	opj_dinfo_t * dinfo = opj_create_decompress(CODEC_JP2);
	
	opj_set_event_mgr((opj_common_ptr)dinfo, NULL, NULL);
	
	opj_setup_decoder(dinfo, &parameters);
	
	opj_cio_t * cio = opj_cio_open((opj_common_ptr)dinfo, (unsigned char *)fileData, dataSize);
	
	opj_image_t * image = opj_decode(dinfo, cio);
	if (!image)
	{
		opj_destroy_decompress(dinfo);
		opj_cio_close(cio);
		return REPtr<REImageBase>();
	}
	
	opj_cio_close(cio);
	
	const REUInt32 width = (REUInt32)image->comps[0].w;
	const REUInt32 height = (REUInt32)image->comps[0].h;
	const REUInt32 channelsCount = (REUInt32)image->numcomps;
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
			opj_destroy_decompress(dinfo);
			opj_cio_close(cio);
			return REPtr<REImageBase>();
			break;
	}
	
	REImageBase * newBase = new REImageBase(pixelFormat, width, height);
	if (!newBase) 
	{
		opj_destroy_decompress(dinfo);
		opj_cio_close(cio);
		return REPtr<REImageBase>();
	}
	if (newBase->isNull() || (newBase->channelsCount() != channelsCount)) 
	{
		opj_destroy_decompress(dinfo);
		opj_cio_close(cio);
		delete newBase;
		return REPtr<REImageBase>();
	}
	
	REUByte * imageData = newBase->imageData();
	if (image->numcomps == 3) 
	{
		REUByte * rgb = imageData;
		for (REUInt32 y = 0; y < height; y++) 
		{
			const REUInt32 index = y * width;
			int * r = &image->comps[0].data[index];
			int * g = &image->comps[1].data[index];
			int * b = &image->comps[2].data[index];
			for (REUInt32 x = 0; x < width; x++)	
			{
				*rgb++ = (REUByte)*r++;
				*rgb++ = (REUByte)*g++;
				*rgb++ = (REUByte)*b++;
			}
		}
	}
	else if (image->numcomps == 4)
	{
		REUByte * rgba = imageData;
		const REUInt32 alphaComponent = image->numcomps - 1;
		for (REUInt32 y = 0; y < height; y++) 
		{
			const REUInt32 index = y * width;
			int * r = &image->comps[0].data[index];
			int * g = &image->comps[1].data[index];
			int * b = &image->comps[2].data[index];
			int * a = &image->comps[alphaComponent].data[index];
			for (REUInt32 x = 0; x < width; x++)	
			{
				*rgba++ = (REUByte)*r++;
				*rgba++ = (REUByte)*g++;
				*rgba++ = (REUByte)*b++;
				*rgba++ = (REUByte)*a++;
			}
		}
	}
	
	opj_image_destroy(image);
	opj_destroy_decompress(dinfo);
	
	return REPtr<REImageBase>(newBase);
}
#endif

#ifndef __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__ 	
REPtr<REImageBase> REImageManagerPrivate::loadJPG(const REUByte * fileData, const REUInt32 dataSize)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr pub;
	
#ifdef __RE_HAVE_SYSTEM_SETJMP_H__ 	
	jmp_buf setjmp_buffer;
#endif	
	
	cinfo.err = jpeg_std_error(&pub);
	
#ifdef __RE_HAVE_SYSTEM_SETJMP_H__ 	
	if ( setjmp(setjmp_buffer) )
	{
		jpeg_destroy_decompress(&cinfo);
		return REPtr<REImageBase>();
	}
#endif	
	
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, (unsigned char *)fileData, dataSize);
	jpeg_read_header(&cinfo, true);
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
	for (REUInt32 rowIndex = 0; rowIndex < height; rowIndex++ )
	{
		rowPointer[0] = &imageData[rowIndex * rowStride];
		jpeg_read_scanlines(&cinfo, rowPointer, 1);
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	
	return REPtr<REImageBase>(newBase);
}
#endif 	

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
	if (buff->resize(buff->size() + length, true)) 
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
	
#ifdef __RE_HAVE_SYSTEM_SETJMP_H__ 	
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
			for (REUInt32 k = 0; k < height; k++)
			{
				row_pointers[k] = pixData + (k * width * bytes_per_pixel);
			}
			png_write_image(png_ptr, row_pointers);
			
			png_write_end(png_ptr, info_ptr);
		}
		
		png_destroy_write_struct(&png_ptr, &info_ptr);
		delete [] row_pointers;
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
			
		case REImageTypeJPEG2000:
#ifndef __RE_RECORE_NO_JPEG2000_IMAGE_SUPPORT__			
			return REImageManagerPrivate::loadJPEG2000(fileData, dataSize);
#else
			RELog::log("Loading JPEG2000 image format requires undefined flag __RE_RECORE_NO_JPEG2000_IMAGE_SUPPORT__");
#endif			
			break;	
			
		case REImageTypeTGA:
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
	
	REUInt32 * data = (REUInt32*)fileData;
	if (data[1] == 0x2020506a)
	{
		return REImageTypeJPEG2000;
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
														   const REUInt32 width,
														   const REUInt32 height,
														   const REImagePixelFormat pixelFormat)
{
#ifndef __RE_RECORE_NO_PNG_IMAGE_SUPPORT__
	return REImageManagerPrivate::createPNGFilePresentation(pixelsData, width, height, pixelFormat);
#else
	RELog::log("\"CreatePNGFilePresentation\" functionality requires undefined flag __RE_RECORE_NO_PNG_IMAGE_SUPPORT__");
	return REPtr<REBuffer>();
#endif
}

REPtr<REBuffer> REImageManager::createWebPFilePresentation(const REUByte * pixelsData,
															const REUInt32 width,
															const REUInt32 height,
															const REImagePixelFormat pixelFormat,
															const REFloat32 quality)
{
#ifndef __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__  
	return REImageManagerPrivate::createWebPFilePresentation(pixelsData, width, height, pixelFormat, quality);
#else
	RELog::log("\"CreateWebPFilePresentation\" functionality requires undefined flag __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__");
	return REPtr<REBuffer>();
#endif	
}

REImageManager::REImageManager()
{

}

REImageManager::~REImageManager()
{

}

