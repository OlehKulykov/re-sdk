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


#include "../../include/regui/private/RETextureDirectX9.h"
#include "../../include/regui/RERenderDevice.h"
#include "../../include/recore/REThread.h"

#ifdef __RE_USING_DIRECTX9__

/// Checks is texture generated from mipmaps.
REBOOL RETextureDirectX9::IsMipmaped() const
{
	return ((_filterType == RETextureFilterMipmapedStandart) || (_filterType == RETextureFilterMipmapedTrilinear));
}

void RETextureDirectX9::WritePixelsToDXBuffer(const REUByte * texImgData,
											  const REImagePixelFormat pixelsFormat,
											  REUInt32 pixelsCount,
											  REUByte * dst)
{
	switch (pixelsFormat)
	{
		case REImagePixelFormatR8G8B8:
			while (pixelsCount > 0)
			{
				*dst++ = texImgData[2]; // B8
				*dst++ = texImgData[1]; // G8
				*dst++ = texImgData[0]; // R8
				*dst++ = (REUByte)255;  // 0xFF
				texImgData += 3;
				pixelsCount--;
			}
			break;
		case REImagePixelFormatR8G8B8A8:
			while (pixelsCount > 0)
			{
				*dst++ = texImgData[2]; // B8
				*dst++ = texImgData[1]; // G8
				*dst++ = texImgData[0]; // R8
				*dst++ = texImgData[3]; // A8
				texImgData += 4;
				pixelsCount--;
			}
			break;
		case REImagePixelFormatLuminance8Alpha8:
			while (pixelsCount > 0)
			{
				*dst++ = texImgData[1];	// L8
				*dst++ = texImgData[0];	// A8
				texImgData += 2;
				pixelsCount--;
			}
			break;
		case REImagePixelFormatAlpha8:
			while (pixelsCount > 0)
			{
				*dst++ =(REUByte)255;	// 0xFF
				*dst++ = *texImgData++; // L8
				pixelsCount--;
			}
			break;
		default:
			break;
	}
}

/// Updates texture with unpacked pixels from image.
REBOOL RETextureDirectX9::Update(const REUByte * pixelsData, 
								 const REImagePixelFormat pixelsFormat,
								 const REUInt32 width,
								 const REUInt32 height)
{
	if ((width != (REUInt32)_initialWidth) && 
		(height != (REUInt32)_initialHeight) &&
		(pixelsFormat != (REImagePixelFormat)_initialPixelFormat))
	{
		RE_SAFE_RELEASE(_direct3DTexture);
	}
	
	if (_d3dDevice && pixelsData)
	{
		D3DFORMAT dxFormat = D3DFMT_UNKNOWN;
		REBOOL isTransparent = true;
		switch (pixelsFormat) 
		{
			case REImagePixelFormatR8G8B8:
				dxFormat = D3DFMT_X8R8G8B8; //D3DFMT_R8G8B8;
				isTransparent = false;
				break;
			case REImagePixelFormatR8G8B8A8:
				dxFormat = D3DFMT_A8R8G8B8;
				break;
			case REImagePixelFormatLuminance8Alpha8:
			case REImagePixelFormatAlpha8:
				dxFormat = D3DFMT_A8L8;
				break;	
			default:
				return false;
				break;
		}
		
		LPDIRECT3DTEXTURE9 dxTexture = _direct3DTexture;
		if (dxTexture == NULL)
		{
			HRESULT reslt = D3DXCreateTexture(_d3dDevice, 
											  (UINT)width,
											  (UINT)height, 
											  1, 
											  0, 
											  dxFormat, 
											  D3DPOOL_MANAGED,
											  &dxTexture);
			if (reslt != D3D_OK) { return false; }
		}
		
		D3DLOCKED_RECT lockedRect;
		if ( dxTexture->LockRect(0, &lockedRect, 0, 0) != D3D_OK ) 
		{
			dxTexture->Release();
			return false; 
		}
		
		RETextureDirectX9::WritePixelsToDXBuffer(pixelsData,
												 pixelsFormat,
												 (REUInt32)(width * height),
												 (REUByte *)lockedRect.pBits);
		
		if ( dxTexture->UnlockRect(0) != D3D_OK) 
		{
			dxTexture->Release();
			return false; 
		}	
		
		_isBlended = isTransparent;
		_direct3DTexture = dxTexture;
		_initialWidth = (REUInt16)width;
		_initialHeight = (REUInt16)height;
		_initialPixelFormat = (REUByte)pixelsFormat;
		
		return true;
		
	}
	return false;
}


/// Settes and stores filter type to texture.
/// On OpenGL/ES textures must set filter before update texture with image data.
void RETextureDirectX9::SetFilterType(const RETextureFilterType filter)
{
	_filterType = (REUByte)filter;
}

/// Returns texture type of setted or stored texture filter type.
const RETextureFilterType RETextureDirectX9::GetFilterType() const
{
	return (RETextureFilterType)_filterType;
}

RETextureDirectX9::RETextureDirectX9(IDirect3DDevice9 * d3dDevice) : RERenderDeviceTextureObject(), 
_d3dDevice(d3dDevice),
_direct3DTexture((LPDIRECT3DTEXTURE9)0),
_initialWidth(0),
_initialHeight(0),
_initialPixelFormat(0),
_filterType(0),
_isBlended(false)
{
	
}

void RETextureDirectX9::OnReleased()
{
	if (_direct3DTexture) 
	{
		_direct3DTexture->Release();
		_direct3DTexture = (LPDIRECT3DTEXTURE9)0;
	}
	
	RERenderDeviceTextureObject::OnReleased();
}

RETextureDirectX9::~RETextureDirectX9()
{
	
}

RETextureDirectX9 * RETextureDirectX9::CreateWithD3DDevice(IDirect3DDevice9 * d3dDevice)
{
	if (d3dDevice)
	{
		RETextureDirectX9 * newTexture = new RETextureDirectX9(d3dDevice);
		return newTexture;
	}
	return NULL;
}

#endif /* __RE_USING_DIRECTX9__ */


