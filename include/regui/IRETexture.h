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


#ifndef __IRETEXTURE_H__
#define __IRETEXTURE_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REImage.h"

/// Structure used for typing of texture filters.
typedef enum _reTextureFilterType
{
	/// No texture filter.
	RETextureFilterNone = 0,
	
	/// Nearest texture filter.
	/// On OpenGL/ES textures settes 'GL_TEXTURE_MIN_FILTER' to 'GL_NEAREST' and 'GL_TEXTURE_MAG_FILTER' to 'GL_NEAREST'.
	RETextureFilterNearest = 1,
	
	/// Linear texture filter.
	/// On OpenGL/ES textures settes 'GL_TEXTURE_MIN_FILTER' to 'GL_LINEAR' and 'GL_TEXTURE_MAG_FILTER' to 'GL_LINEAR'.
	RETextureFilterLinear = 2,
	
	/// Mipmaped standart linear texture filter.
	/// On OpenGL/ES textures settes 'GL_TEXTURE_MIN_FILTER' to 'GL_LINEAR_MIPMAP_NEAREST' and 'GL_TEXTURE_MAG_FILTER' to 'GL_LINEAR'.
	RETextureFilterMipmapedStandart = 3,
	
	/// Mipmaped trilinear texture filter.
	/// On OpenGL/ES textures settes 'GL_TEXTURE_MIN_FILTER' to 'GL_LINEAR_MIPMAP_LINEAR' and 'GL_TEXTURE_MAG_FILTER' to 'GL_LINEAR'.
	RETextureFilterMipmapedTrilinear = 4
} 
/// Structure used for typing of texture filters.
RETextureFilterType;

/// Class interface for textures.
class __RE_PUBLIC_CLASS_API__ IRETexture
{
public:
	/// Checks for null.
	virtual REBOOL isNull() const = 0;
	
	/// Checks is texture generated from mipmaps.
	virtual REBOOL isMipmaped() const = 0;
	
	/// Updates texture with unpacked pixels from image.
	/// On OpenGL/ES textures must set filter before update texture with image data.
	virtual REBOOL update(const REUByte * pixelsData, 
						  const REImagePixelFormat pixelsFormat,
						  const REUInt32 width,
						  const REUInt32 height) = 0;
	
	/// Checks is texture updated from image data that contained alpha channel.
	virtual const REBOOL isBlended() const = 0;
	
	/// Settes and stores filter type to texture.
	/// On OpenGL/ES textures must set filter before update texture with image data.
	virtual void setFilterType(const RETextureFilterType filter) = 0;
	
	/// Returns texture type of setted or stored texture filter type.
	virtual const RETextureFilterType filterType() const = 0;
	
	/// Binds i.e. settes texture as current.
	virtual void bind() const = 0;
	
	/// Destructor.
	virtual ~IRETexture() { }
};

#endif /* __IRETEXTURE_H__ */


