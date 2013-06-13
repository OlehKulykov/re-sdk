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


#include "../../include/regui/RERenderDeviceDirectX9.h"

#ifdef __RE_USING_DIRECTX9__

#include "../../include/regui/private/RETextureDirectX9.h"

void RERenderDeviceDirectX9::StartRendering()
{
	//_d3dDevice->BeginScene();
	_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	_d3dDevice->BeginScene();
}

void RERenderDeviceDirectX9::EndRendering()
{
	_d3dDevice->EndScene();
	_d3dDevice->Present(NULL, NULL, NULL, NULL);
}

/*
http://msdn.microsoft.com/en-us/library/bb147263%28VS.85%29.aspx#General_Performance_Tips
*/
void RERenderDeviceDirectX9::RenderRect(const RETetragon & tetragon, 
										const REColor & color, 
										const RETetragon & textureFrame, 
										IRETexture * texture)
{
	//	RED3DVertexColorizedTextured
	//	D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1
	RED3DVertexColorizedTextured * vertices = NULL;
	_rectColorVertexTextureBuffer1->Lock(0, 0, (void**)&vertices, 0);

	//Setup vertices
	vertices[3].z = vertices[2].z = vertices[1].z = vertices[0].z = 0.0f;
	vertices[3].color = vertices[2].color = vertices[1].color = vertices[0].color = color.GetD3DColor();

	vertices[0].x = tetragon.topLeftX;
	vertices[0].y = tetragon.topLeftY;

	vertices[1].x = tetragon.topRightX;
	vertices[1].y = tetragon.topRightY;
	
	vertices[2].x = tetragon.bottomLeftX;
	vertices[2].y = tetragon.bottomLeftY;

	vertices[3].x = tetragon.bottomRightX;
	vertices[3].y = tetragon.bottomRightY;

	if (texture)
	{
		vertices[0].u = textureFrame.topLeftX;
		vertices[0].v = textureFrame.topLeftY;

		vertices[1].u = textureFrame.topRightX;
		vertices[1].v = textureFrame.topRightY;

		vertices[2].u = textureFrame.bottomLeftX;
		vertices[2].v = textureFrame.bottomLeftY;

		vertices[3].u = textureFrame.bottomRightX;
		vertices[3].v = textureFrame.bottomRightY;
		
		if (texture->IsBlended() || (color.alpha < 1.0f)) { this->EnableBlending(); }
		else { this->DisableBlending(); }

		this->SetTextureSamplerState0FilterType(texture->GetFilterType());
		
		texture->Bind();
	}
	else
	{
		if (color.alpha < 1.0f) { this->EnableBlending(); }
		else { this->DisableBlending(); }
	}

	_rectColorVertexTextureBuffer1->Unlock();
	_d3dDevice->SetStreamSource(0, _rectColorVertexTextureBuffer1, 0, sizeof(RED3DVertexColorizedTextured));
	_d3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	_d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//D3DPT_TRIANGLESTRIP
}

void RERenderDeviceDirectX9::RenderRect(const RERect & rect, 
										const REColor & color, 
										IRETexture * texture)
{
	//	RED3DVertexColorizedTextured
	//	D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1
	RED3DVertexColorizedTextured * vertices = NULL;
	_rectColorVertexTextureBuffer2->Lock(0, 0, (void**)&vertices, 0);

	//Setup vertices
	vertices[2].x = vertices[0].x = rect.x;
	vertices[1].y = vertices[0].y = rect.y;
	vertices[3].x = vertices[1].x = rect.x + rect.width;
	vertices[3].y = vertices[2].y = rect.y + rect.height;
	vertices[3].z = vertices[2].z = vertices[1].z = vertices[0].z = 0.0f;

	vertices[3].color = vertices[2].color = vertices[0].color = vertices[1].color = color.GetD3DColor();

	vertices[2].u = vertices[1].v = vertices[0].u = vertices[0].v = 0.0f;
	vertices[1].u = vertices[2].v = vertices[3].u = vertices[3].v = 1.0f;

	_rectColorVertexTextureBuffer2->Unlock();

	if (texture)
	{
		if (texture->IsBlended() || (color.alpha < 1.0f)) { this->EnableBlending(); }
		else { this->DisableBlending(); }

		this->SetTextureSamplerState0FilterType(texture->GetFilterType());
		
		texture->Bind();
	}
	else
	{
		if (color.alpha < 1.0f) { this->EnableBlending(); }
		else { this->DisableBlending(); }
	}

	_d3dDevice->SetStreamSource(0, _rectColorVertexTextureBuffer2, 0, sizeof(RED3DVertexColorizedTextured));
	_d3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	_d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//D3DPT_TRIANGLESTRIP
}

void RERenderDeviceDirectX9::RenderRect(const RERect & rect, 
							const REColor & color, 
							const RETetragon & textureFrame, 
							IRETexture * texture)
{
	//	RED3DVertexColorizedTextured
	//	D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1
	RED3DVertexColorizedTextured * vertices = NULL;
	_rectColorVertexTextureBuffer3->Lock(0, 0, (void**)&vertices, 0);

	//Setup vertices
	vertices[3].z = vertices[2].z = vertices[1].z = vertices[0].z = 0.0f;
	vertices[3].color = vertices[2].color = vertices[1].color = vertices[0].color = color.GetD3DColor();

	vertices[2].x = vertices[0].x = rect.x;
	vertices[1].y = vertices[0].y = rect.y;
	vertices[3].x = vertices[1].x = rect.x + rect.width;
	vertices[3].y = vertices[2].y = rect.y + rect.height;

	if (texture)
	{
		vertices[0].u = textureFrame.topLeftX;
		vertices[0].v = textureFrame.topLeftY;

		vertices[1].u = textureFrame.topRightX;
		vertices[1].v = textureFrame.topRightY;

		vertices[2].u = textureFrame.bottomLeftX;
		vertices[2].v = textureFrame.bottomLeftY;

		vertices[3].u = textureFrame.bottomRightX;
		vertices[3].v = textureFrame.bottomRightY;

		this->SetTextureSamplerState0FilterType(texture->GetFilterType());

		if (texture->IsBlended() || (color.alpha < 1.0f)) { this->EnableBlending(); }
		else { this->DisableBlending(); }

		texture->Bind();
	}
	else
	{
		if (color.alpha < 1.0f) { this->EnableBlending(); }
		else { this->DisableBlending(); }
	}
	
	_rectColorVertexTextureBuffer3->Unlock();

	_d3dDevice->SetStreamSource(0, _rectColorVertexTextureBuffer3, 0, sizeof(RED3DVertexColorizedTextured));
	_d3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	_d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//D3DPT_TRIANGLESTRIP
}

void RERenderDeviceDirectX9::ClearDX()
{
	_windowHandle = NULL;

	if (_d3dDevice)
	{
		_d3dDevice->Release();
		_d3dDevice = NULL;
	}

	if (_d3dInterface)
	{
		_d3dInterface->Release();
		_d3dInterface = NULL;
	}

	_currentRenderStates.Clear();
	_currentTextureStageStates.Clear();
}

void RERenderDeviceDirectX9::SetFullScreen(REBOOL isFullScreen)
{

	_isFullScreen = isFullScreen;
}

void RERenderDeviceDirectX9::SetDefaultPresentationParams(D3DPRESENT_PARAMETERS * params)
{
	if ( params )
	{
		ZeroMemory(params, sizeof(D3DPRESENT_PARAMETERS));

		// Set all parameters that can be shared between windowed and full screen mode
		params->hDeviceWindow = _windowHandle; // Handle to window
		params->BackBufferCount = 2; // Number of render surfaces
		params->SwapEffect = D3DSWAPEFFECT_FLIP; // We want to treat the back buffers as a circular
		// queue and rotate through them
		params->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Draw everything immediately, don't
		// wait for V-sync
		params->EnableAutoDepthStencil = TRUE; // We want a Z-buffer
		params->AutoDepthStencilFormat = D3DFMT_D16; // 16-bit Z-buffer (should be safe on most cards...)

		if(_isFullScreen)
		{
			D3DDISPLAYMODE dispMode = { 0 };

			// Get the current configuration of the primary monitor
			if ( _d3dInterface->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dispMode) != D3D_OK)
			{
				return;
			}

			// Fill in the rest of the parameters with the primary display device's parameters
			params->Windowed = FALSE;
			params->BackBufferWidth = dispMode.Width;
			params->BackBufferHeight = dispMode.Height;
			params->FullScreen_RefreshRateInHz = dispMode.RefreshRate;
			params->BackBufferFormat = dispMode.Format; // Use the current color depth of the monitor	
		}
		else
		{
			// Get client rect
			RECT rect = { 0 };
			GetClientRect(_windowHandle, &rect);

			params->Windowed = true; // We want a window
			params->BackBufferWidth = rect.right; // Backbuffer's width equals client rect's width
			params->BackBufferHeight = rect.bottom; // Backbuffer's height equals client rect's height
			params->BackBufferFormat = D3DFMT_UNKNOWN; // Use whatever the current color depth of the
			// monitor is for the back buffer
		}
	}
}

REBOOL RERenderDeviceDirectX9::CreateDXDevice(D3DPRESENT_PARAMETERS * params)
{
	if ( _d3dInterface && params && (_d3dDevice == NULL) )
	{
		HRESULT reslt = _d3dInterface->CreateDevice(D3DADAPTER_DEFAULT,
													D3DDEVTYPE_HAL,
													_windowHandle,
													D3DCREATE_HARDWARE_VERTEXPROCESSING,
													params,
													&_d3dDevice);

		if ( reslt == D3D_OK )
		{
			return true;
		}
		else
		{
			//RELog::Warning("Can't create D3D9Device with D3DCREATE_HARDWARE_VERTEXPROCESSING. Try create D3DCREATE_SOFTWARE_VERTEXPROCESSING.");
			reslt = _d3dInterface->CreateDevice(D3DADAPTER_DEFAULT,
												D3DDEVTYPE_HAL,
												_windowHandle,
												D3DCREATE_SOFTWARE_VERTEXPROCESSING,
												params,
												&_d3dDevice);
			if (reslt == D3D_OK)
			{
				return true;
			}
			else
			{
				//RELog::Error("Can't create D3D9Device with D3DCREATE_SOFTWARE_VERTEXPROCESSING.\nNo Render device.");
			}
		}
	}

	return false;
}

void RERenderDeviceDirectX9::StoreCurrentRenderState(D3DRENDERSTATETYPE State, unsigned int Value)
{
	for (REUInt32 i = 0; i < _currentRenderStates.Count(); i++)
	{
		if (_currentRenderStates[i].state == State)
		{
			_currentRenderStates[i].value = Value;
			return;
		}
	}

	RED3DDeviceRenderStateStruct renderState;
	renderState.state = State;
	renderState.value = Value;
	_currentRenderStates.Add(renderState);
}

REBOOL RERenderDeviceDirectX9::SetRenderState(D3DRENDERSTATETYPE State, unsigned int Value)
{
	if (_d3dDevice)
	{
		if ( _d3dDevice->SetRenderState(State, Value) == D3D_OK ) 
		{
			this->StoreCurrentRenderState(State, Value);
			return true;
		}
	}
	return false;
}

REBOOL RERenderDeviceDirectX9::PushRenderStates()
{
	if (_currentRenderStates.Count() > 0)
	{
		REArray<RED3DDeviceRenderStateStruct> * arr = new REArray<RED3DDeviceRenderStateStruct>(_currentRenderStates.Count() + 1);
		if (arr)
		{
			/*
			if (_d3dDevice)
			{
				unsigned long v = 0;
				if (_d3dDevice->GetRenderState(D3DRS_SRCBLEND, &v) == D3D_OK )
				{
					this->StoreCurrentRenderState(D3DRS_SRCBLEND, v);
				}
				if (_d3dDevice->GetRenderState(D3DRS_DESTBLEND, &v) == D3D_OK )
				{
					this->StoreCurrentRenderState(D3DRS_DESTBLEND, v);
				}
				if (_d3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &v) == D3D_OK )
				{
					this->StoreCurrentRenderState(D3DRS_ALPHABLENDENABLE, v);
				}
			}
			*/

			(*arr) = _currentRenderStates;
			if ( _savedRenderStates.Add(arr) )
			{
				return true;
			}
			delete arr;
		}
	}
	return false;
}

REBOOL RERenderDeviceDirectX9::PopRenderStates()
{
	if (_savedRenderStates.Count() > 0)
	{
		REArray<RED3DDeviceRenderStateStruct> * arr = _savedRenderStates.LastObject();
		_savedRenderStates.RemoveLast();
		_currentRenderStates.Clear();
		for (REUInt32 i = 0; i < arr->Count(); i++)
		{
			RED3DDeviceRenderStateStruct renderState = (*arr)[i];
			this->SetRenderState(renderState.state, renderState.value);
		}
		delete arr;
		return true;
	}
	return false;
}

REBOOL RERenderDeviceDirectX9::SetTextureStageState(unsigned int Stage, D3DTEXTURESTAGESTATETYPE Type, unsigned int Value)
{
	if (_d3dDevice)
	{
		if ( _d3dDevice->SetTextureStageState(Stage, Type, Value) == D3D_OK ) 
		{ 
			for (REUInt32 i = 0; i < _currentTextureStageStates.Count(); i++)
			{
				if ((_currentTextureStageStates[i].stage == Stage) &&
					(_currentTextureStageStates[i].type == Type))
				{
					_currentTextureStageStates[i].value = Value;
					return true;
				}
			}

			RED3DDeviceTextureStageStateStruct textStageStr;
			textStageStr.stage = Stage;
			textStageStr.type = Type;
			textStageStr.value = Value;
			_currentTextureStageStates.Add(textStageStr);
			return true;
		}
	}

	return false;
}

REBOOL RERenderDeviceDirectX9::PushTextureStageStates()
{
	if (_currentRenderStates.Count() > 0)
	{
		REArray<RED3DDeviceTextureStageStateStruct> * arr = new REArray<RED3DDeviceTextureStageStateStruct>(_currentTextureStageStates.Count() + 1);
		if (arr)
		{
			(*arr) = _currentTextureStageStates;
			if ( _savedTextureStageStates.Add(arr) )
			{
				return true;
			}
			delete arr;
		}
	}
	return false;
}

REBOOL RERenderDeviceDirectX9::PopTextureStageStates()
{
	if (_savedTextureStageStates.Count() > 0)
	{
		REArray<RED3DDeviceTextureStageStateStruct> * arr = _savedTextureStageStates.LastObject();
		_savedTextureStageStates.RemoveLast();
		_currentTextureStageStates.Clear();
		for (REUInt32 i = 0; i < arr->Count(); i++)
		{
			RED3DDeviceTextureStageStateStruct st = (*arr)[i];
			this->SetTextureStageState(st.stage, st.type, st.value);
		}
		delete arr;
		return true;
	}
	return false;
}

REBOOL RERenderDeviceDirectX9::InitializeWithWindow(void * windowHandle)
{
	if ( (!_isInitialized) && (windowHandle != NULL) )
	{
		_windowHandle = (HWND)windowHandle;

		_d3dInterface = Direct3DCreate9(D3D_SDK_VERSION);
		
		// Error Check
		if(_d3dInterface == NULL)
		{
			this->ClearDX();
			return _isInitialized;
		}
	
		D3DPRESENT_PARAMETERS params = { 0 }; // Presentation parameters to be filled
		RERenderDeviceDirectX9::SetDefaultPresentationParams(&params);

		if ( !this->CreateDXDevice(&params) )
		{
			this->ClearDX();
			return _isInitialized;
		}

		D3DXMATRIX matIdentity;
		D3DXMatrixIdentity(&matIdentity);
		if ( _d3dDevice->SetTransform(D3DTS_WORLD, &matIdentity) != D3D_OK ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}

		if ( _d3dDevice->SetTransform(D3DTS_VIEW, &matIdentity) != D3D_OK ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}

		if ( _d3dDevice->SetTransform(D3DTS_PROJECTION, &matIdentity) != D3D_OK ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}

		/*
		Specifies how back-facing triangles are culled, if at all. 
		This can be set to one member of the D3DCULL enumerated type. 
		The default value is D3DCULL_CCW. 
   
		D3DCULL_NONE
		Do not cull back faces.  т.е. обратная сторона треугольника БУДЕТ отображаться.   
		*/
		if ( !this->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE) )
		{
			this->ClearDX(); return _isInitialized; 
		}

		//
		if ( !this->SetRenderState(D3DRS_LIGHTING, FALSE) ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}

		// Disable depth buffering.
		if ( !this->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE) ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}
		
		if ( !this->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}
		
		this->SetDefaultBlendFunction();
		
		if ( !this->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_SRCALPHA) ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}
		if ( !this->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}


		if ( !this->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE) ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}
		if ( !this->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE) ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}
		if ( !this->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE) ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}
		if ( !this->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE) ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}
		if ( !this->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE) ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}
		if ( !this->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE) ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}
		

		D3DCAPS9 caps; // Struct to hold the device's capabilites
		if ( _d3dInterface->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps) != D3D_OK ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}

		// Loop through and set the mag/min filters for all the possible texture stages
		for(unsigned int i = 0; i < caps.MaxTextureBlendStages; ++i)
		{
			// Set magnification filter
			if ( _d3dDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR) != D3D_OK ) 
			{ 
				this->ClearDX(); return _isInitialized; 
			}

			// Set minification filter, 
			if ( _d3dDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR) != D3D_OK ) 
			{ 
				this->ClearDX(); return _isInitialized; 
			}

			// Enable mipmapping in general
			if ( _d3dDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR) != D3D_OK ) 
			{ 
				this->ClearDX(); return _isInitialized; 
			}

			if ( _d3dDevice->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP) != D3D_OK ) 
			{ 
				this->ClearDX(); return _isInitialized; 
			}

			if ( _d3dDevice->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP) != D3D_OK ) 
			{ 
				this->ClearDX(); return _isInitialized; 
			}
		}

		if ( _d3dDevice->CreateVertexBuffer(4 * sizeof(RED3DVertexColorizedTextured), 
											D3DUSAGE_WRITEONLY, 
											D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, 
											D3DPOOL_MANAGED, 
											&_rectColorVertexTextureBuffer1, 
											NULL) != D3D_OK ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}

		if ( _d3dDevice->CreateVertexBuffer(4 * sizeof(RED3DVertexColorizedTextured), 
											D3DUSAGE_WRITEONLY, 
											D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, 
											D3DPOOL_MANAGED, 
											&_rectColorVertexTextureBuffer2, 
											NULL) != D3D_OK ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}

		if ( _d3dDevice->CreateVertexBuffer(4 * sizeof(RED3DVertexColorizedTextured), 
											D3DUSAGE_WRITEONLY, 
											D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, 
											D3DPOOL_MANAGED, 
											&_rectColorVertexTextureBuffer3, 
											NULL) != D3D_OK ) 
		{ 
			this->ClearDX(); return _isInitialized; 
		}

		//RELog::Log("D3DDevice is initialized");
		_isInitialized = true;
	}
	return _isInitialized;
}

REBOOL RERenderDeviceDirectX9::Initialize()
{
	return _isInitialized;
}

void RERenderDeviceDirectX9::OnRenderSizeChanged()
{
	if ( _d3dDevice )
	{
		D3DXMATRIX matWorld, matView, matOrtho;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matView);

		D3DXMatrixOrthoOffCenterLH(
			&matOrtho, 
			0.0f,
			_renderSize.width,
			_renderSize.height,
			0.0f,
			-1.0f,
			1.0f);

		_d3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		_d3dDevice->SetTransform(D3DTS_VIEW, &matView);
		_d3dDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
	}
}

void RERenderDeviceDirectX9::OnScreenSizeChanged()
{
		
}

void RERenderDeviceDirectX9::OnClearColorChanged()
{

}

/* textures */
/*
RETextureBase * RERenderDeviceDirectX9::CreateNewTexture(const REUByte * pixelsData, REImagePixelFormat pixelFormat, 
								 REUInt32 width, REUInt32 height, RETextureFilterType filterType)
{
	if ( (pixelsData == NULL) || (pixelFormat == REImagePixelFormatNONE) ) 
	{
		return NULL;
	}

	REBOOL isTransparent = false;
	D3DFORMAT dxFormat = D3DFMT_X8R8G8B8; //D3DFMT_R8G8B8;
	REUInt32 srcColorComponents = 3;
	REBOOL isNeedConvertPixels = false;

	if ( pixelFormat == REImagePixelFormatR8G8B8A8 )
	{
		dxFormat = D3DFMT_A8R8G8B8;
		isTransparent = true;
		srcColorComponents = 4;
		isNeedConvertPixels = true;
	}
	else if ( pixelFormat == REImagePixelFormatLuminance8Alpha8 )
	{
		dxFormat = D3DFMT_A8L8;
		isTransparent = true;
		srcColorComponents = 2;
		isNeedConvertPixels = true;
	}

	REUInt32 normalWidth = RETexture::GetNearestPowerOfTwo(width);
	REUInt32 normalHeight = RETexture::GetNearestPowerOfTwo(height);
	REUByte * normalPixelsData = NULL;

	if ( (normalWidth != width) || (normalHeight != height) ) 
	{
		normalPixelsData = (REUByte *)malloc(normalWidth * normalHeight * srcColorComponents);
		if ( normalPixelsData ) 
		{
			if ( !REImage::ScaleImageData(	pixelsData,
											width, 
											height, 
											srcColorComponents, 
											normalPixelsData, 
											normalWidth, 
											normalHeight) )
			{
				free(normalPixelsData);
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}
	

	const REUByte * texImgData = pixelsData;
	if ( normalPixelsData )
	{
		texImgData = normalPixelsData;
	}


	LPDIRECT3DTEXTURE9 dxTexture = NULL;

	//CheckDeviceFormat()

	//_d3dDevice->CreateTexture()

	HRESULT reslt = D3DXCreateTexture(_d3dDevice, 
		(UINT)normalWidth,
		(UINT)normalHeight, 
		1, 
		0, 
		dxFormat, 
		D3DPOOL_MANAGED,
		&dxTexture);

	if (reslt != D3D_OK)
	{
		if ( normalPixelsData ) { free(normalPixelsData); }
		return NULL;
	}

	D3DLOCKED_RECT lockedRect;
	if ( dxTexture->LockRect(0, &lockedRect, 0, 0) != D3D_OK )
	{
		dxTexture->Release();
		if ( normalPixelsData ) { free(normalPixelsData); }
		return NULL;
	}

	if ( isNeedConvertPixels )
	{
		UINT pixelsCount = (normalWidth * normalHeight);
		REUByte * dst = (REUByte *)lockedRect.pBits;
		if ( pixelFormat == REImagePixelFormatR8G8B8A8 )
		{
			//memcpy(lockedRect.pBits, texImgData, pixelsCount * 4); 
			while (pixelsCount > 0)
			{
				*dst++ = texImgData[2]; // B8
				//dst++;

				*dst++ = texImgData[1]; // G8
				//dst++;
				
				*dst++ = texImgData[0]; // R8
				//dst++;
				
				*dst++ = texImgData[3]; // A8
				//dst++;

				texImgData += 4;
				pixelsCount--;
			}
		}
		else if ( pixelFormat == REImagePixelFormatLuminance8Alpha8 )
		{
			while (pixelsCount > 0)
			{
				*dst++ = texImgData[1];	// L8
				//dst++;

				*dst++ = texImgData[0];	// A8
				//dst++;

				texImgData += 2;
				pixelsCount--;
			}
		}
	}
	else
	{
		
		UINT pixelsCount = (normalWidth * normalHeight);
		REUByte * dst = (REUByte *)lockedRect.pBits;
		while (pixelsCount > 0)
		{
			*dst++ = texImgData[2]; // B8
			//dst++;

			*dst++ = texImgData[1]; // G8
			//dst++;
	
			*dst++ = texImgData[0]; // R8
			//dst++;
	
			*dst++ = (REUByte)255;
			//dst++;

			texImgData += 3;
			pixelsCount--;
		}
	}

	if ( dxTexture->UnlockRect(0) != D3D_OK)
	{
		dxTexture->Release();
		if ( normalPixelsData ) { free(normalPixelsData); }
		return NULL;
	}	

	if ( normalPixelsData ) 
	{ 
		free(normalPixelsData); 
	}

	bool isMipmaped = false;
	RETextureBase * t = new RETextureDirectX9(_d3dDevice, dxTexture, isMipmaped);
	if ( t )
	{
		t->SetHeight(normalHeight);
		t->SetWidth(normalWidth);
		t->SetFilterType(filterType);
		t->SetIsTransparent(isTransparent);
	}
	return t;
}
*/
RERenderDeviceTextureObject * RERenderDeviceDirectX9::CreateNewTexture()
{
	RETextureDirectX9 * dxTexture = RETextureDirectX9::CreateWithD3DDevice(_d3dDevice);
	if (dxTexture)
	{
		if (dxTexture->IsNull())
		{
			dxTexture->Release();
			return NULL;
		}
		return dxTexture;
	}
	return NULL;
}

const RERenderDeviceBlendType RERenderDeviceDirectX9::GetBlendSourceFactor() const
{
	return this->GetBlendTypeFromDX9Value(_blendSFactor);
}

const RERenderDeviceBlendType RERenderDeviceDirectX9::GetBlendDestinationFactor() const
{
	return this->GetBlendTypeFromDX9Value(_blendDFactor);
}

void RERenderDeviceDirectX9::SetBlendFunction(const RERenderDeviceBlendType newSFactor, const RERenderDeviceBlendType newDFactor)
{
	const unsigned int sf = this->GetDX9BlendValueFromEnum(newSFactor);
	const unsigned int df = this->GetDX9BlendValueFromEnum(newDFactor);
	this->SetDX9BlendFunction(sf, df);
}

void RERenderDeviceDirectX9::SetDefaultBlendFunction()
{
	this->SetDX9BlendFunction(__RENDER_DEVICE_DX9_DEFAULT_BLEND_S_FACTOR__, __RENDER_DEVICE_DX9_DEFAULT_BLEND_D_FACTOR__);
}

void RERenderDeviceDirectX9::SetDX9BlendFunction(const unsigned int newSFactor, const unsigned int newDFactor)
{
	if ( (newSFactor != _blendSFactor) || (newDFactor != _blendDFactor) ) 
	{
		if ( !this->SetRenderState(D3DRS_SRCBLEND , newSFactor) ) { return; }
		if ( !this->SetRenderState(D3DRS_DESTBLEND, newDFactor) ) { return; }
		_blendSFactor = newSFactor;
		_blendDFactor = newDFactor;
	}
}

RERenderDeviceDirectX9::RERenderDeviceDirectX9() : 
	_windowHandle(NULL),
	_d3dInterface(NULL),
	_d3dDevice(NULL),
	_blendSFactor(0),
	_blendDFactor(0),
	_rectColorVertexTextureBuffer1(NULL),
	_rectColorVertexTextureBuffer2(NULL),
	_rectColorVertexTextureBuffer3(NULL),
	_current0FilterType(RETextureFilterNone)
{
	
}

RERenderDeviceDirectX9::~RERenderDeviceDirectX9()
{
	this->ClearDX();
}


#endif /* __RE_USING_DIRECTX9__ */


