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


#ifndef __RERENDERDEVICE_H__
#define __RERENDERDEVICE_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REImage.h"
#include "../recore/REImageManager.h"
#include "../recore/RESize.h"
#include "../recore/REColor.h"
#include "../recore/RERect.h"
#include "../recore/RETetragon.h"
#include "IRETexture.h"
#include "RERenderDeviceTextureObject.h"
#include "RECamera.h"

typedef enum _reRenderDeviceMode
{
	RERenderDeviceModeNone = 0,
	RERenderDeviceMode2D = 2,
    RERenderDeviceMode3D = 3
} RERenderDeviceMode;

typedef enum _reRenderDeviceType
{
	RERenderDeviceTypeNone = 0,
	RERenderDeviceTypeOpenGL = 1,
    RERenderDeviceTypeDirectX9 = 2
} RERenderDeviceType;


typedef enum _reRenderDeviceBlendType
{
	RERenderDeviceBlendType_ZERO = 0,
	RERenderDeviceBlendType_ONE = 1,
	RERenderDeviceBlendType_DST_COLOR = 2,
	RERenderDeviceBlendType_ONE_MINUS_DST_COLOR = 3,
	RERenderDeviceBlendType_SRC_ALPHA = 4,
	RERenderDeviceBlendType_ONE_MINUS_SRC_ALPHA = 5,
	RERenderDeviceBlendType_DST_ALPHA = 6,
	RERenderDeviceBlendType_ONE_MINUS_DST_ALPHA = 7,
	RERenderDeviceBlendType_SRC_ALPHA_SATURATE = 8
} RERenderDeviceBlendType;

#if (defined(__RE_USING_OPENGL_ES__) || defined(__RE_USING_OPENGL__)) 

#ifndef __RE_RENDER_DEVICE_OPENGL__
#define __RE_RENDER_DEVICE_OPENGL__
#endif
class RERenderDeviceOpenGL;

#elif defined(__RE_USING_DIRECTX9__) 

#ifndef __RE_RENDER_DEVICE_DIRECTX9__
#define __RE_RENDER_DEVICE_DIRECTX9__
#endif
class RERenderDeviceDirectX9;
#endif


class __RE_PUBLIC_CLASS_API__ RERenderDevice
{
private:
#if defined(__RE_RENDER_DEVICE_OPENGL__) 
	static RERenderDeviceOpenGL * _defaultDevice;
#elif defined(__RE_RENDER_DEVICE_DIRECTX9__) 
	static RERenderDeviceDirectX9 * _defaultDevice;
#endif	
	
protected:
	RERenderDevice();
	virtual ~RERenderDevice() { }
	
	/* size */
	RECamera * _deviceCamera;
	
	REColor _clearColor;
	
	RESize _renderSize;
	RESize _screenSize;
	RESize _renderToScreenSizeRatio;
	RESize _screenToRenderSizeRatio;
	
	REFloat32 _screenAspectRatio;
	RERenderDeviceMode _currentRenderMode;
	RERenderDeviceType _deviceType;
	
	REBOOL _isInitialized;
	REBOOL _isFullScreen;
	
	void UpdateSizeRatio();

	virtual void OnRenderSizeChanged() = 0;
	virtual void OnScreenSizeChanged() = 0;
	virtual void OnClearColorChanged() = 0;
	

public:
	/* blending */
	virtual const RERenderDeviceBlendType GetBlendSourceFactor() const = 0;
	virtual const RERenderDeviceBlendType GetBlendDestinationFactor() const = 0;
	virtual void SetBlendFunction(const RERenderDeviceBlendType newSFactor, const RERenderDeviceBlendType newDFactor) = 0;
	virtual void SetDefaultBlendFunction() = 0;
	
	/* size */
	const RESize & GetRenderSize() { return _renderSize; }
	const RESize & GetScreenSize() { return _screenSize; }
	void SetRenderSize(const RESize & newRenderSize);
	void SetRenderSize(const REFloat32 newWidth, const REFloat32 newHeight);
	void SetScreenSize(const RESize & newScreenSize);
	void SetScreenSize(const REFloat32 newWidth, const REFloat32 newHeight);
	const RESize & GetRenderToScreenSizeRatio() { return _renderToScreenSizeRatio; }
	const RESize & GetScreenToRenderSizeRatio() { return _screenToRenderSizeRatio; }
	const REFloat32 GetScreenAspectRatio() { return _screenAspectRatio; }

	/* init */
	virtual REBOOL Initialize() = 0;
	virtual REBOOL InitializeWithWindow(void * windowHandle) = 0;
	REBOOL IsInitialized() const { return _isInitialized; }
	
	/* setup */
	const REColor & GetClearColor() const { return _clearColor; }
	void SetClearColor(const REColor & newClearColor) { _clearColor = newClearColor; this->OnClearColorChanged(); }
	REBOOL IsFullScreen() const { return _isFullScreen; }
	virtual void SetFullScreen(REBOOL isFullScreen) = 0;
	
	RECamera * GetDeviceCamera() const { return _deviceCamera; }
	REBOOL SetDeviceCamera(RECamera * newCamera);
	
	const RERenderDeviceType GetDeviceType() const { return _deviceType; }
	
	/* drawing */
	virtual void StartRendering() = 0;
	virtual void EndRendering() = 0;

	virtual void RenderRect(const RETetragon & tetragon, 
							const REColor & color, 
							const RETetragon & textureFrame, 
							IRETexture * texture) = 0;
	
	virtual void RenderRect(const RERect & rect, 
							const REColor & color, 
							IRETexture * texture) = 0;
	
	virtual void RenderRect(const RERect & rect, 
							const REColor & color, 
							const RETetragon & textureFrame, 
							IRETexture * texture) = 0;
	
	void RenderClippedRect(const RERect & bounds, 
						   const RERect & rect, 
						   const REColor & color, 
						   IRETexture * texture);
	
	void RenderClippedRect(const RERect & bounds, 
						   const RERect & rect, 
						   const REColor & color, 
						   const RETetragon & textureFrame, 
						   IRETexture * texture);
	
	/* render mode */
	const RERenderDeviceMode GetRenderMode() const { return _currentRenderMode; }
	virtual void Enter2DMode() = 0;
	virtual void Leave2DMode() = 0;
	const REBOOL Is2DMode() const { return (_currentRenderMode == RERenderDeviceMode2D); }
	const REBOOL Is3DMode() const { return (_currentRenderMode == RERenderDeviceMode3D); }
	

	/* creates new empty device specific texture */
	virtual RERenderDeviceTextureObject * CreateNewTexture() = 0;
	
	static RERenderDevice * GetDefaultDevice();
	
#if defined(__RE_RENDER_DEVICE_OPENGL__) 
	static RERenderDeviceOpenGL * GetDefaultDeviceOpenGL();
#elif defined(__RE_RENDER_DEVICE_DIRECTX9__) 
	static RERenderDeviceDirectX9 * GetDefaultDeviceDirectX9();
#endif	
	
	static void ReleaseDefaultDevice();
};

#endif /* __RERENDERDEVICE_H__ */ 



