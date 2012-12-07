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


#ifndef __IRETEXTURE_H__
#define __IRETEXTURE_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REImage.h"

#if defined(__RE_USING_DIRECTX9__)
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#elif defined(__RE_USING_OPENGL__)

#if defined(__RE_OS_WINDOWS__)
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#elif defined(__RE_OS_MACOSX__)
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#elif defined(__RE_OS_LINUX__)
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#elif defined(__RE_USING_OPENGL_ES__) && defined(__RE_USING_OPENGL_ES_1__)

#if defined(__RE_OS_IPHONE__)
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#elif defined(__RE_OS_ANDROID__)
#include <GLES/gl.h>
#include <GLES/glext.h>
#endif

#endif

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
#if (defined(__RE_USING_OPENGL_ES__) || defined(__RE_USING_OPENGL__))
	/// Returns OpenGL or OpenGLES texture identifier.
	virtual const GLuint GetTexureIdentifier() const = 0;
#endif	
#ifdef __RE_USING_DIRECTX9__ 
	/// Returns pointer to DirectX 9 device.
	virtual IDirect3DDevice9 * GetD3DDevice9() const = 0;
	
	/// Returns pointer to DirectX 9 texture.
	virtual LPDIRECT3DTEXTURE9 GetDirect3DTexture9() const = 0;
#endif	
	/// Checks for null.
	virtual REBOOL IsNull() const = 0;
	
	/// Checks is texture generated from mipmaps.
	virtual REBOOL IsMipmaped() const = 0;
	
	/// Updates texture with unpacked pixels from image.
	/// On OpenGL/ES textures must set filter before update texture with image data.
	virtual REBOOL Update(const REUByte * pixelsData, 
						  const REImagePixelFormat pixelsFormat,
						  const REUInt32 width,
						  const REUInt32 height) = 0;
	
	/// Checks is texture updated from image data that contained alpha channel.
	virtual const REBOOL IsBlended() const = 0;
	
	/// Settes and stores filter type to texture.
	/// On OpenGL/ES textures must set filter before update texture with image data.
	virtual void SetFilterType(const RETextureFilterType filter) = 0;
	
	/// Returns texture type of setted or stored texture filter type.
	virtual const RETextureFilterType GetFilterType() const = 0;
	
	/// Binds i.e. settes texture as current.
	virtual void Bind() const = 0;
	
	/// Destructor.
	virtual ~IRETexture() { }
};

#endif /* __IRETEXTURE_H__ */


