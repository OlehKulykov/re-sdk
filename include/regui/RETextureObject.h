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


#ifndef __RETEXTUREOBJECT_H__
#define __RETEXTUREOBJECT_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REImage.h"
#include "../recore/RETetragon.h"
#include "REGUIObject.h"
#include "IRETexture.h"
#include "RERenderDeviceTextureObject.h"
#include "IREAnimation.h"

/// Class of texture.
class __RE_PUBLIC_CLASS_API__ RETextureObject : public REGUIObject, public IRETexture
{
private:
	static REBOOL AcceptStringParameterForTexture(const char * key, const char * value, RETextureObject * texture);
protected:
	/// Creates and returns new device specific texture object or NULL on error.
	/// Actualy same functionality of 'RERenderDevice' 'CreateNewTexture()' with NULL checking.
	static RERenderDeviceTextureObject * CreateNewRenderDeviceTexture();
	
	/// Updates device specific texture with image from file path and texture filter.
    virtual REBOOL UpdateFromImageFilePath(const REString & imageFilePath, const RETextureFilterType filterType);
	
	/// Updates device specific texture with image and texture filter.
	REBOOL UpdateFromImage(const REImage & image, const RETextureFilterType filterType);
	
	/// Release prev device specific texture and than updates device specific texture with image from file path and texture filter.
	REBOOL InitFromImageFilePath(const REString & imageFilePath, const RETextureFilterType filterType);
	
	/// Release prev device specific texture and updates device specific texture with image and texture filter.
	REBOOL InitFromImage(const REImage & image, const RETextureFilterType filterType);
	
	/// Release prev device specific texture and initialized new empty device specific texture.
	REBOOL InitBlankTexture(const REImagePixelFormat pixelsFormat,
							const REUInt32 width,
							const REUInt32 height,
							const RETextureFilterType filterType);
	
	RETextureObject();
	virtual ~RETextureObject();
	
	/// Pointer to device specific texture.
	RERenderDeviceTextureObject * _texture;
	
	/// Frame of texture coordinates.
	RETetragon _frame;
	
public:
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	/* REGUIObject */
	virtual void OnPrepareGUIObjectForSetuping();
	virtual void OnSetupingGUIObjectFinished(const REBOOL isAcceptedByParent);
	virtual REBOOL AcceptStringParameter(const char * key, const char * value);
	
	/* IRETexture */
#if (defined(__RE_USING_OPENGL_ES__) || defined(__RE_USING_OPENGL__))
	/// Returns OpenGL or OpenGLES texture identifier
	virtual const GLuint GetTexureIdentifier() const;
#endif	
#ifdef __RE_USING_DIRECTX9__ 
	/// Returns pointer to DirectX 9 device.
	virtual IDirect3DDevice9 * GetD3DDevice9() const;
	
	/// Returns pointer to DirectX 9 texture.
	virtual LPDIRECT3DTEXTURE9 GetDirect3DTexture9() const;
#endif 
	virtual REBOOL IsNull() const;
	virtual REBOOL IsMipmaped() const;
	virtual REBOOL Update(const REUByte * pixelsData, 
						  const REImagePixelFormat pixelsFormat,
						  const REUInt32 width,
						  const REUInt32 height);
	virtual const REBOOL IsBlended() const { if (_texture) { return _texture->IsBlended(); } return false; }
	virtual void SetFilterType(const RETextureFilterType filter);
	virtual const RETextureFilterType GetFilterType() const;
	virtual void Bind() const { if (_texture) _texture->Bind(); }
	
	
	/* RETexture */
	/// Returns pointer or NULL of device specific texture.
	RERenderDeviceTextureObject * GetTexture() const { return _texture; }
	
	/// Returns frame with texture coordinates.
	const RETetragon & GetFrame() const { return _frame; }
	
	/// Settes frame with texture coordinates.
	void SetFrame(const RETetragon & newFrame);
	
	/// Settes frame with texture coordinates.
	/// Can be animated using REAnimation, call this method between REAnimation::Setup() and REAnimation::Execute().
	/// If animation is not settuping !REAnimation::IsSetuping() than SetFrame() will be called.
	void SetFrameAnimated(const RETetragon & newFrame);
	
	/// Stops all animations of object
	void StopAnimation(const REAnimationStopType stopType, REBOOL isNeedCallDelegate);
	
	/// Copy 'anotherTexture' texture to this one.
	RETextureObject & operator=(const RETextureObject & anotherTexture);
	
	/// Cleares texture object. Release device specific texture and sets texture frame to full rectangle.
	void Clear();
	
	/// Returns closed power of 2 value.
	static REUInt32 GetNearestPowerOfTwo(const REUInt32 inValue);
	
	/// Creates and returns new empty 'RETextureObject' or NULL on error.
	static RETextureObject * Create();
	
	/// Creates and returns new empty 'RETextureObject' with setting params or NULL on error.
	static RETextureObject * CreateWithBlankTexture(const REImagePixelFormat pixelsFormat,
													const REUInt32 width,
													const REUInt32 height,
													const RETextureFilterType filterType = RETextureFilterLinear);
	
	/// Creates and returns new 'RETextureObject' initialized with image data loaded from 'imageFilePath' or NULL on error.
	static RETextureObject * CreateWithImageFilePath(const REString & imageFilePath, 
													 const RETextureFilterType filterType = RETextureFilterLinear);
	
	/// Creates and returns new 'RETextureObject' initialized with image data from 'image' or NULL on error.
	static RETextureObject * CreateWithImage(const REImage & image, 
											 const RETextureFilterType filterType = RETextureFilterLinear);

	/// Returns XML key string for texture filter.
    static const char * GetXMLFilterKeyString();
	
	/// Returns XML key string for texture image path.
    static const char * GetXMLPathKeyString();
	
	/// Returns XML key string for frame.
    static const char * GetXMLFrameKeyString();
	
	/// Returns XML format string for frame.
    static const char * GetXMLFrameFormatString();
	
	/// Returns XML string of texture filter type.
    static const char * GetXMLFilterStringByType(const RETextureFilterType filterType);
};

#endif /* __RETEXTUREOBJECT_H__ */


