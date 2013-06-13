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


#ifndef __REIMAGEBASE_H__
#define __REIMAGEBASE_H__

#include "RECommonHeader.h"
#include "REObject.h"
#include "REBuffer.h"

/// Format of image data.
typedef enum _reImagePixelFormat
{
	/// Undefined, default format.
	REImagePixelFormatNONE = 0,
	
	/// RGB pixel format, where each pixel channel (Red, Green, Blue) is 8 bit integer(byte) in range [0, 255].
	REImagePixelFormatR8G8B8 = 1,
	
	/// RGBA pixel format, where each pixel channel (Red, Green, Blue, Alpha) is 8 bit integer(byte) in range [0, 255].
	REImagePixelFormatR8G8B8A8 = 2,
	
	/// Luma/Alpha pixel format, where each pixel channel (Luminance, Alpha) is 8 bit integer(byte) in range [0, 255].
	REImagePixelFormatLuminance8Alpha8 = 3,
	
	/// Alpha pixel format, where each pixel channel (Alpha) is 8 bit integer(byte) in range [0, 255].
	REImagePixelFormatAlpha8 = 4
} 
/// Format of image data.
REImagePixelFormat;

/// Class using for storing image data(pixels) and info.
class __RE_PUBLIC_CLASS_API__ REImageBase : public REObject
{
private:
	REBuffer _buffer;
	REUInt16 _width;
	REUInt16 _height;
	REUByte _format;
public:
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	
	/// Check is image NULL
	REBOOL IsNull() const { return (_format == 0); }
	
	/// Returns pointer to image data(pixels).
	REUByte * GetImageData() const { return (REUByte *)_buffer.GetBuffer(); }
	
	/// Returns pointer to image data(pixels) with 'x', 'y' coordinates from top left corner.
	REUByte * GetImageDataTopLeftOffset(const REUInt32 x, const REUInt32 y) const;
	
	/// Returns size in bytes of image data(pixels).
	const REUInt32 GetImageDataSize() const { return _buffer.GetSize(); }
	
	/// Returns width of image in range [0, INT16_MAX].
	const REUInt32 GetWidth() const { return (REUInt32)_width; }
	
	/// Returns height of image in range [0, INT16_MAX].
	const REUInt32 GetHeight() const { return (REUInt32)_height; }
	
	/// Returns format of image.
	const REImagePixelFormat GetFormat() const { return ((REImagePixelFormat)_format); }
	
	/// Returns number of bits per each pixel.
	const REUInt32 GetBitsPerPixel() const { return REImageBase::BitsPerPixel((REImagePixelFormat)_format); }
	
	/// Returns number of bytes per each pixel.
	const REUInt32 GetBytesPerPixel() const { return REImageBase::BytesPerPixel((REImagePixelFormat)_format); }
	
	// Returns number of channels per each pixel.
	const REUInt32 GetChannelsCount() const { return REImageBase::ChannelsCount((REImagePixelFormat)_format); }
	
	/// Returns number of channels per each pixel using image format.
	static REUInt32 ChannelsCount(const REImagePixelFormat format);
	
	/// Returns number of bits per each pixel using image format.
	static REUInt32 BitsPerPixel(const REImagePixelFormat format);
	
	/// Returns number of bytes per each pixel using image format.
	static REUInt32 BytesPerPixel(const REImagePixelFormat format);
	
	/// Constructs image with format, width and height
	REImageBase(const REImagePixelFormat pixelsFormat, 
				const REUInt32 width, 
				const REUInt32 height);
	
	/// Constructs image with data(pixels), format, width and height
	REImageBase(const REUByte * pixelsData, 
				const REImagePixelFormat pixelsFormat, 
				const REUInt32 width, 
				const REUInt32 height);
	
	/// Default constructor.
	REImageBase();
	
	/// Destructor.
	virtual ~REImageBase();
};


#endif /* __REIMAGEBASE_H__ */


