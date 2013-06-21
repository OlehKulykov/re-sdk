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


#ifndef __RETEXTUREDIRECTX9_H__
#define __RETEXTUREDIRECTX9_H__

#include "../../recore/RECommonHeader.h"

#ifdef __RE_USING_DIRECTX9__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "../../recore/REObject.h"
#include "../../recore/RETetragon.h"
#include "../RERenderDeviceTextureObject.h"

class __RE_PUBLIC_CLASS_API__ RETextureDirectX9 : public RERenderDeviceTextureObject  
{
private:
	IDirect3DDevice9 * _d3dDevice;
	LPDIRECT3DTEXTURE9 _direct3DTexture;
	REUInt16 _initialWidth;
	REUInt16 _initialHeight;
	REUByte _initialPixelFormat;
	REUByte _filterType;
	REBOOL _isBlended;

	static void WritePixelsToDXBuffer(const REUByte * texImgData,
									  const REImagePixelFormat pixelsFormat,
									  REUInt32 pixelsCount,
									  REUByte * dst);
protected:
	RETextureDirectX9(IDirect3DDevice9 * d3dDevice);
	virtual ~RETextureDirectX9();
	
public:
	/* REObject */
	virtual void onReleased();
	
	/// Returns pointer to DirectX 9 device.
	virtual IDirect3DDevice9 * GetD3DDevice9() const { return _d3dDevice; }

	/// Returns pointer to DirectX 9 texture.
	virtual LPDIRECT3DTEXTURE9 GetDirect3DTexture9() const { return _direct3DTexture; }

	/// Checks for null.
	virtual REBOOL IsNull() const { return ( (_direct3DTexture == 0) && (_d3dDevice == NULL) ); }

	/// Checks is texture generated from mipmaps.
	virtual REBOOL IsMipmaped() const;
	
	/// Updates texture with unpacked pixels from image.
	/// On OpenGL/ES textures must set filter before update texture with image data.
	virtual REBOOL Update(const REUByte * pixelsData, 
						  const REImagePixelFormat pixelsFormat,
						  const REUInt32 width,
						  const REUInt32 height);

	/// Checks is texture updated from image data that contained alpha channel.
	virtual const REBOOL IsBlended() const { return _isBlended; }

	/// Settes and stores filter type to texture.
	/// On OpenGL/ES textures must set filter before update texture with image data.
	virtual void SetFilterType(const RETextureFilterType filter);

	/// Returns texture type of setted or stored texture filter type.
	virtual const RETextureFilterType GetFilterType() const;

	/// Binds i.e. settes texture as current.
	virtual void Bind() const { if (_direct3DTexture && _d3dDevice) { _d3dDevice->SetTexture(0, _direct3DTexture); } }

	
	static RETextureDirectX9 * CreateWithD3DDevice(IDirect3DDevice9 * d3dDevice);
};



#endif /* __RE_USING_DIRECTX9__ */ 

#endif /* __RETEXTUREDIRECTX9_H__ */ 


