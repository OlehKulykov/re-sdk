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
#include "../recore/RESerializable.h"
#include "../recore/RETetragon.h"
#include "../recore/REObject.h"
#include "../recore/REPtr.h"
#include "IRETexture.h"
#include "REAnimation.h"

class RETextureInternal;

class __RE_PUBLIC_CLASS_API__ RETexture : public REObject, public RESerializable, public IRETexture
{
protected:
	/// Internal implementation.
	REPtr<RETextureInternal> _t;
	
	/// Frame of texture coordinates.
	RETetragon _frame;
	
	/// Counter of texture animations. If greater than zero than animation in progress.
	REUInt16 _animationsCounter;
	
	/* REObject */
	virtual void onReleased();
	
	RETexture();
	virtual ~RETexture();
public:
	/* RESerializable */
	virtual RETypedPtr serializeToDictionary() const;
	
	virtual void deserializeWithDictionary(const RETypedPtr & dictionary);
	
	/// Returns frame with texture coordinates.
	const RETetragon & frame() const;
	
	/// Settes frame with texture coordinates.
	void setFrame(const RETetragon & newFrame);
	
	/// Settes frame with texture coordinates.
	/// Can be animated using REAnimation, call this method between REAnimation::Setup() and REAnimation::Execute().
	/// If animation is not settuping !REAnimation::isSetuping() than setFrame() will be called.
	void setFrameAnimated(const RETetragon & newFrame);
	
	/// Stops all animations of object
	void stopAnimation(const REAnimationStopType stopType, REBOOL isNeedCallDelegate);
	
	/// Return OpenGL identifier or 0 if texture "isNull()" or not for OpenGL.
	REUInt32 openGLTexureIdentifier() const;
	
	/// Return void pointer to Direct3D device or NULL if texture "isNull()" or not for DirectX.
	/// This is casted value of IDirect3DDevice9 to void using REPtrCast<IDirect3DDevice9, void>(...);
	/// For getting IDirect3DDevice9 use REPtrCast<void, IDirect3DDevice9>(...)
	void * direct3DDevice9() const;
	
	/// Return void pointer to Direct3D texture or NULL if texture "isNull()" or not for DirectX.
	/// This is casted value of LPDIRECT3DTEXTURE9 to void using REPtrCast<LPDIRECT3DTEXTURE9, void>(...);
	/// For getting LPDIRECT3DTEXTURE9 use REPtrCast<void, LPDIRECT3DTEXTURE9>(...)
	void * direct3DTexture9() const;
	
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
	
	REBOOL initWithPixelsData(const REUByte * pixelsData, 
							  const REImagePixelFormat pixelsFormat,
							  const REUInt32 width,
							  const REUInt32 height,
							  const RETextureFilterType filterType = RETextureFilterLinear);
	
	REBOOL initBlankTexture(const RETextureFilterType filterType = RETextureFilterLinear);
	
	REBOOL initWithImageFilePath(const REString & imageFilePath, 
								 const RETextureFilterType filterType = RETextureFilterLinear);
	
	REBOOL initWithImage(const REImage & image, 
						 const RETextureFilterType filterType = RETextureFilterLinear);
	
	
	/// Creates null'able(not initialized) texture object instance.
	/// For initializing texture use init or serializable methods.
	static RETexture * create();
	
	/// Creates initialized blank texture object instance.
	static RETexture * createBlankTexture(const RETextureFilterType filterType = RETextureFilterLinear);
	
	/// Returns closed power of 2 value.
	static REUInt32 nearestPowerOfTwo(const REUInt32 inValue);
	
	/// Returns key for texture image file path.
	static const char * imagePathKey();
};

#endif /* __RETEXTURE_H__ */


