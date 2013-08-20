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


#include "../../include/regui/RERenderDevice.h"


#if defined(__RE_RENDER_DEVICE_OPENGL__) 

#include "../../include/regui/RERenderDeviceOpenGL.h"

__RE_PUBLIC_CLASS_API__ RERenderDeviceOpenGL * RERenderDevice::_defaultDevice = NULL;

#elif defined(__RE_RENDER_DEVICE_DIRECTX9__) 

#include "../../include/regui/RERenderDeviceDirectX9.h"

__RE_PUBLIC_CLASS_API__ RERenderDeviceDirectX9 * RERenderDevice::_defaultDevice = NULL;
#endif

void RERenderDevice::RenderClippedRect(const RERect & bounds, 
									   const RERect & rect, 
									   const REColor & color, 
									   IRETexture * texture)
{
	this->RenderClippedRect(bounds, rect, color, RETetragon(RERect(0.0f, 0.0f, 1.0f, 1.0f)), texture);
}

void RERenderDevice::RenderClippedRect(const RERect & bounds, 
									   const RERect & rect, 
									   const REColor & color, 
									   const RETetragon & textureFrame, 
									   IRETexture * texture)
{
	RERect interFrame( rect.intersectionRect(bounds) );
	if (interFrame.height > 0.0f) 
	{
		const REFloat32 oldTW = textureFrame.bottomRightX - textureFrame.topLeftX;
		const REFloat32 oldTH = textureFrame.bottomRightX - textureFrame.topLeftX;
	
		const REFloat32 rectKoefX = (interFrame.x - rect.x) / rect.width;
		const REFloat32 rectKoefY = (interFrame.y - rect.y) / rect.height;
		
		const REFloat32 tx = textureFrame.topLeftX + (oldTW * rectKoefX);
		const REFloat32 ty = textureFrame.topLeftY + (oldTH * rectKoefY);
		
		const REFloat32 rectKoefWidth = interFrame.width / rect.width;
		const REFloat32 rectKoefHeight = interFrame.height / rect.height;
		
		this->RenderRect(interFrame,
						 color, 
						 RETetragon(RERect(tx, ty, (oldTW * rectKoefWidth), (oldTH * rectKoefHeight))), 
						 texture);
	}
	else
	{
		this->RenderRect(rect, color, textureFrame, texture);
	}
}

void RERenderDevice::UpdateSizeRatio()
{
	if ( _renderSize.isNull() || _screenSize.isNull() ) 
	{
		_renderToScreenSizeRatio.setToNull();
		_screenToRenderSizeRatio.setToNull();
	}
	else 
	{
		_screenToRenderSizeRatio.width = (_renderSize.width / _screenSize.width);
		_screenToRenderSizeRatio.height = (_renderSize.height / _screenSize.height);
		
		_renderToScreenSizeRatio.width = (_screenSize.width / _renderSize.width);
		_renderToScreenSizeRatio.height = (_screenSize.height / _renderSize.height);

		_screenAspectRatio = ( _screenSize.width / _screenSize.height );
	}
}

void RERenderDevice::SetRenderSize(const RESize & newRenderSize)
{
	_renderSize = newRenderSize;
	this->UpdateSizeRatio();
	if (_isInitialized) 
	{
		this->OnRenderSizeChanged();
	}
}

void RERenderDevice::SetRenderSize(const REFloat32 newWidth, const REFloat32 newHeight)
{
	_renderSize.width = newWidth;
	_renderSize.height = newHeight;
	this->UpdateSizeRatio();
	if (_isInitialized) 
	{
		this->OnRenderSizeChanged();
	}
}

void RERenderDevice::SetScreenSize(const RESize & newScreenSize)
{
	_screenSize = newScreenSize;
	this->UpdateSizeRatio();
	if (_isInitialized) 
	{
		this->OnScreenSizeChanged();
	}
}

void RERenderDevice::SetScreenSize(const REFloat32 newWidth, const REFloat32 newHeight)
{
	_screenSize.width = newWidth;
	_screenSize.height = newHeight;
	this->UpdateSizeRatio();
	if (_isInitialized) 
	{
		this->OnScreenSizeChanged();
	}
}

REBOOL RERenderDevice::SetDeviceCamera(RECamera * newCamera)
{
	if (newCamera) 
	{
		if (_deviceCamera) 
		{
			_deviceCamera->release();
		}
		_deviceCamera = newCamera;
		_deviceCamera->retain();
		return true;
	}
	return false;
}

RERenderDevice::RERenderDevice() :
	_deviceCamera(NULL),
	_screenAspectRatio(0.0f),
	_currentRenderMode(RERenderDeviceModeNone),
	_deviceType(RERenderDeviceTypeNone),
	_isInitialized(false),
	_isFullScreen(false)
{
	_clearColor.alpha = 1.0f;
	_deviceCamera = new RECamera();
}

RERenderDevice * RERenderDevice::GetDefaultDevice()
{
	if (_defaultDevice) 
	{
		return _defaultDevice;
	}
	
#if defined(__RE_RENDER_DEVICE_OPENGL__) 
	_defaultDevice = new RERenderDeviceOpenGL();
#elif defined(__RE_RENDER_DEVICE_DIRECTX9__) 
	_defaultDevice = new RERenderDeviceDirectX9();
#endif
	
	return _defaultDevice;	
}

#ifdef __RE_RENDER_DEVICE_OPENGL__
RERenderDeviceOpenGL * RERenderDevice::GetDefaultDeviceOpenGL()
{
	if (_defaultDevice) 
	{
		return _defaultDevice;
	}
	_defaultDevice = new RERenderDeviceOpenGL();
	return _defaultDevice;
}
#elif defined(__RE_RENDER_DEVICE_DIRECTX9__) 
RERenderDeviceDirectX9 * RERenderDevice::GetDefaultDeviceDirectX9()
{
	if (_defaultDevice) 
	{
		return _defaultDevice;
	}
	_defaultDevice = new RERenderDeviceDirectX9();
	return _defaultDevice;
}
#endif	

void RERenderDevice::ReleaseDefaultDevice()
{
	if (_defaultDevice) 
	{
		delete _defaultDevice;
		_defaultDevice = NULL;
	}
}

