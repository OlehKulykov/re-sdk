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


#ifndef __RERENDERDEVICEDIRECTX9_H__
#define __RERENDERDEVICEDIRECTX9_H__


#include "../recore/RECommonHeader.h"


#ifdef __RE_USING_DIRECTX9__

#include "RERenderDevice.h"


#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

#define __RENDER_DEVICE_DX9_DEFAULT_BLEND_S_FACTOR__ D3DBLEND_SRCALPHA
#define __RENDER_DEVICE_DX9_DEFAULT_BLEND_D_FACTOR__ D3DBLEND_INVSRCALPHA


//	D3DFVF_XYZ | D3DFVF_DIFFUSE
typedef struct _RED3DVertexColorized
{
	FLOAT x, y, z;
	D3DCOLOR color;
} RED3DVertexColorized;


// D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1
typedef struct _RED3DVertexColorizedTextured
{
	FLOAT x, y, z;
	D3DCOLOR color;
	FLOAT u, v;
} RED3DVertexColorizedTextured;

typedef struct _RED3DDeviceRenderStateStruct
{
	D3DRENDERSTATETYPE state;
	unsigned int value;
} RED3DDeviceRenderStateStruct;

typedef struct _RED3DDeviceTextureStageStateStruct
{
	unsigned int stage;
	D3DTEXTURESTAGESTATETYPE type;
	unsigned int value;
} RED3DDeviceTextureStageStateStruct;

#if (defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) || defined(__USING_RECORE_DYNAMIC_LIBRARY__)) 
/// Warning message 4251: Class 'REArray<T>' needs to have dll-interface to be used by clients of class.
/// Why disable 4251: Class 'REArray<T>' defined as private field and no clents can access it.
/// To access must use public methods.
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif
#endif

class __RE_PUBLIC_CLASS_API__ RERenderDeviceDirectX9 : public RERenderDevice 
{
private:
	HWND _windowHandle;
	IDirect3D9 * _d3dInterface;
	IDirect3DDevice9 * _d3dDevice;
	unsigned int _blendSFactor;
	unsigned int _blendDFactor;
	
	LPDIRECT3DVERTEXBUFFER9 _rectColorVertexTextureBuffer1;
	LPDIRECT3DVERTEXBUFFER9 _rectColorVertexTextureBuffer2;
	LPDIRECT3DVERTEXBUFFER9 _rectColorVertexTextureBuffer3;

	REArray<RED3DDeviceRenderStateStruct> _currentRenderStates;
	REArray<RED3DDeviceTextureStageStateStruct> _currentTextureStageStates;

	REArray<REArray<RED3DDeviceRenderStateStruct>*> _savedRenderStates;
	REArray<REArray<RED3DDeviceTextureStageStateStruct>*> _savedTextureStageStates;

	RETextureFilterType _current0FilterType; 

	void ClearDX();
	REBOOL CreateDXDevice(D3DPRESENT_PARAMETERS * params);
	void SetDefaultPresentationParams(D3DPRESENT_PARAMETERS * params);
	void StoreCurrentRenderState(D3DRENDERSTATETYPE State, unsigned int Value);
	
	void EnableBlending() 
	{
		DWORD isAlpha = 0;
		if (_d3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &isAlpha) == D3D_OK)
		{
			if (!isAlpha)
			{
				_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			}
		}
	}
	
	void DisableBlending()
	{
		DWORD isAlpha = 0;
		if (_d3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &isAlpha) == D3D_OK)
		{
			if (isAlpha)
			{
				_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			}
		}
	}
	
	void DisableTexturing()
	{

	}

	void EnableTexturing()
	{

	}

	void SetTextureSamplerState0FilterType(const RETextureFilterType newFilterType)
	{
		if (newFilterType != _current0FilterType)
		{
			_current0FilterType = newFilterType;
			switch (newFilterType) 
			{
				case RETextureFilterNearest:
					_d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
					_d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
					_d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
					break;
				case RETextureFilterLinear:
					_d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
					_d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
					_d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
					break;
				case RETextureFilterMipmapedStandart:
					_d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
					_d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
					_d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
					break;
				case RETextureFilterMipmapedTrilinear:
					_d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
					_d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
					_d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
					break;
				default:
					return;
					break;
			}
		}
	}
	
	const RERenderDeviceBlendType GetBlendTypeFromDX9Value(const unsigned int e) const
	{
		switch (e) 
		{
			case D3DBLEND_SRCALPHA: return RERenderDeviceBlendType_SRC_ALPHA; break;
			case D3DBLEND_INVSRCALPHA: return RERenderDeviceBlendType_ONE_MINUS_SRC_ALPHA; break;
			case D3DBLEND_ONE: return RERenderDeviceBlendType_ONE; break;
			case D3DBLEND_DESTCOLOR: return RERenderDeviceBlendType_DST_COLOR; break;
			case D3DBLEND_INVDESTCOLOR: return RERenderDeviceBlendType_ONE_MINUS_DST_COLOR; break;
			case D3DBLEND_DESTALPHA: return RERenderDeviceBlendType_DST_ALPHA; break;
			case D3DBLEND_INVDESTALPHA: return RERenderDeviceBlendType_ONE_MINUS_DST_ALPHA; break;
			case D3DBLEND_SRCALPHASAT: return RERenderDeviceBlendType_SRC_ALPHA_SATURATE; break;
			default: break;
		}
		return RERenderDeviceBlendType_ZERO;
	}
	
	const unsigned int GetDX9BlendValueFromEnum(const RERenderDeviceBlendType t) const
	{
		switch (t) 
		{
			case RERenderDeviceBlendType_SRC_ALPHA: return D3DBLEND_SRCALPHA;  break;
			case RERenderDeviceBlendType_ONE_MINUS_SRC_ALPHA: return D3DBLEND_INVSRCALPHA;  break;
			case RERenderDeviceBlendType_ONE: return D3DBLEND_ONE; break;
			case RERenderDeviceBlendType_DST_COLOR: return D3DBLEND_DESTCOLOR; break;
			case RERenderDeviceBlendType_ONE_MINUS_DST_COLOR: return D3DBLEND_INVDESTCOLOR; break;
			case RERenderDeviceBlendType_DST_ALPHA: return D3DBLEND_DESTALPHA; break;
			case RERenderDeviceBlendType_ONE_MINUS_DST_ALPHA: return D3DBLEND_INVDESTALPHA; break;
			case RERenderDeviceBlendType_SRC_ALPHA_SATURATE: return D3DBLEND_SRCALPHASAT; break;
			default: break;
		}
		return D3DBLEND_ZERO;
	}
protected:
	void OnRenderSizeChanged();
	void OnScreenSizeChanged();
	void OnClearColorChanged();
	
public:
	IDirect3D9 * GetD3D9Interface() { return _d3dInterface; }
	IDirect3DDevice9 * GetD3D9Device() { return _d3dDevice; }
	REBOOL SetRenderState(D3DRENDERSTATETYPE State, unsigned int Value);
	REBOOL PushRenderStates();
	REBOOL PopRenderStates();

	REBOOL SetTextureStageState(unsigned int Stage, D3DTEXTURESTAGESTATETYPE Type, unsigned int Value);
	REBOOL PushTextureStageStates();
	REBOOL PopTextureStageStates();

	virtual const RERenderDeviceBlendType GetBlendSourceFactor() const;
	virtual const RERenderDeviceBlendType GetBlendDestinationFactor() const;
	virtual void SetBlendFunction(const RERenderDeviceBlendType newSFactor, const RERenderDeviceBlendType newDFactor);
	virtual void SetDefaultBlendFunction();
	void SetDX9BlendFunction(const unsigned int newSFactor, const unsigned int newDFactor);
	
	/* init */
	virtual REBOOL Initialize();
	virtual REBOOL InitializeWithWindow(void * windowHandle);

	/* setup */
	virtual void SetFullScreen(REBOOL isFullScreen);
	
	virtual void StartRendering();
	virtual void EndRendering();
	virtual void RenderRect(const RETetragon & tetragon, 
							const REColor & color, 
							const RETetragon & textureFrame, 
							IRETexture * texture);
	
	virtual void RenderRect(const RERect & rect, 
							const REColor & color, 
							IRETexture * texture);
	
	virtual void RenderRect(const RERect & rect, 
							const REColor & color, 
							const RETetragon & textureFrame, 
							IRETexture * texture);
	
	/* render mode */
	virtual void Enter2DMode() { }
	virtual void Leave2DMode() { }
	
	/* creates new empty device specific texture */
	virtual RERenderDeviceTextureObject * CreateNewTexture();

	RERenderDeviceDirectX9();
	virtual ~RERenderDeviceDirectX9();
};

#endif /* __RE_USING_DIRECTX9__ */

#endif /* __RERENDERDEVICEDIRECTX9_H__ */

