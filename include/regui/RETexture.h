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


#ifndef __RETEXTURE_H__
#define __RETEXTURE_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REObject.h"
#include "../recore/REPtr.h"
#include "IRETexture.h"

class RETextureInternal;

class __RE_PUBLIC_CLASS_API__ RETexture : public REObject, public IRETexture
{
protected:
	REPtr<RETextureInternal> _t;
	
	RETexture();
	virtual ~RETexture();
public:
	REUInt32 openGLTexureIdentifier() const;
	void * direct3DDevice9() const;
	void * direct3DTexture9() const;
	
#if (defined(__RE_USING_OPENGL_ES__) || defined(__RE_USING_OPENGL__))
	/// Returns OpenGL or OpenGLES texture identifier.
	virtual const GLuint texureIdentifier() const { return 0; }
#endif	
#ifdef __RE_USING_DIRECTX9__ 
	/// Returns pointer to DirectX 9 device.
	virtual IDirect3DDevice9 * D3DDevice9() const { return NULL; }
	
	/// Returns pointer to DirectX 9 texture.
	virtual LPDIRECT3DTEXTURE9 direct3DTexture9() const { return NULL; };
#endif	
	/// Checks for null.
	virtual REBOOL isNull() const;
	
	/// Checks is texture generated from mipmaps.
	virtual REBOOL isMipmaped() const;
	
	/// Updates texture with unpacked pixels from image.
	/// On OpenGL/ES textures must set filter before update texture with image data.
	virtual REBOOL update(const REUByte * pixelsData, 
						  const REImagePixelFormat pixelsFormat,
						  const REUInt32 width,
						  const REUInt32 height);
	
	/// Checks is texture updated from image data that contained alpha channel.
	virtual const REBOOL isBlended() const;
	
	/// Settes and stores filter type to texture.
	/// On OpenGL/ES textures must set filter before update texture with image data.
	virtual void setFilterType(const RETextureFilterType filter);
	
	/// Returns texture type of setted or stored texture filter type.
	virtual const RETextureFilterType filterType() const;
	
	/// Binds i.e. settes texture as current.
	virtual void bind() const;
	
	static RETexture * create();
};

#endif /* __RETEXTURE_H__ */


