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

//#if (defined(__RE_USING_OPENGL_ES__) || defined(__RE_USING_OPENGL__)) 
//
//#ifndef __RE_RENDER_DEVICE_OPENGL__
//#define __RE_RENDER_DEVICE_OPENGL__
//#endif
//class RERenderDeviceOpenGL;
//
//#elif defined(__RE_USING_DIRECTX9__) 
//
//#ifndef __RE_RENDER_DEVICE_DIRECTX9__
//#define __RE_RENDER_DEVICE_DIRECTX9__
//#endif
//class RERenderDeviceDirectX9;
//#endif


class __RE_PUBLIC_CLASS_API__ RERenderDevice
{
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
	
	REUInt16 _screenResolution;
	
	REBOOL _isInitialized;
	REBOOL _isFullScreen;
	
	void updateSizeRatio();

	virtual void onRenderSizeChanged() = 0;
	virtual void onScreenSizeChanged() = 0;
	virtual void onClearColorChanged() = 0;
	

public:
	/* blending */
	virtual const RERenderDeviceBlendType blendSourceFactor() const = 0;
	virtual const RERenderDeviceBlendType blendDestinationFactor() const = 0;
	virtual void setBlendFunction(const RERenderDeviceBlendType newSFactor, const RERenderDeviceBlendType newDFactor) = 0;
	virtual void setDefaultBlendFunction() = 0;
	
	/* size */
	const RESize & renderSize() { return _renderSize; }
	const RESize & screenSize() { return _screenSize; }
	void setRenderSize(const RESize & newRenderSize);
	void setRenderSize(const REFloat32 newWidth, const REFloat32 newHeight);
	void setScreenSize(const RESize & newScreenSize);
	void setScreenSize(const REFloat32 newWidth, const REFloat32 newHeight);
	const RESize & renderToScreenSizeRatio() { return _renderToScreenSizeRatio; }
	const RESize & screenToRenderSizeRatio() { return _screenToRenderSizeRatio; }
	const REFloat32 screenAspectRatio() { return _screenAspectRatio; }
	const REUInt16 screenResolution() const;
	void setScreenResolution(const REUInt16 newResolution);
	
	/* init */
	virtual REBOOL initialize() = 0;
	virtual REBOOL initializeWithWindow(void * windowHandle) = 0;
	REBOOL isInitialized() const { return _isInitialized; }
	
	/* setup */
	const REColor & clearColor() const { return _clearColor; }
	void setClearColor(const REColor & newClearColor) { _clearColor = newClearColor; this->onClearColorChanged(); }
	REBOOL isFullScreen() const { return _isFullScreen; }
	virtual void setFullScreen(REBOOL isFullScreen) = 0;
	
	RECamera * deviceCamera() const { return _deviceCamera; }
	REBOOL setDeviceCamera(RECamera * newCamera);
	
	const RERenderDeviceType deviceType() const { return _deviceType; }
	
	/* drawing */
	virtual void startRendering() = 0;
	virtual void endRendering() = 0;

	virtual void renderRect(const RETetragon & tetragon, 
							const REColor & color, 
							const RETetragon & textureFrame, 
							IRETexture * texture) = 0;
	
	virtual void renderRect(const RERect & rect, 
							const REColor & color, 
							IRETexture * texture) = 0;
	
	virtual void renderRect(const RERect & rect, 
							const REColor & color, 
							const RETetragon & textureFrame, 
							IRETexture * texture) = 0;
	
	void renderClippedRect(const RERect & bounds, 
						   const RERect & rect, 
						   const REColor & color, 
						   IRETexture * texture);
	
	void renderClippedRect(const RERect & bounds, 
						   const RERect & rect, 
						   const REColor & color, 
						   const RETetragon & textureFrame, 
						   IRETexture * texture);
	
	/* render mode */
	const RERenderDeviceMode renderMode() const { return _currentRenderMode; }
	virtual void enter2DMode() = 0;
	virtual void leave2DMode() = 0;
	const REBOOL is2DMode() const { return (_currentRenderMode == RERenderDeviceMode2D); }
	const REBOOL is3DMode() const { return (_currentRenderMode == RERenderDeviceMode3D); }
	

	static RERenderDevice * defaultDevice();
	
//#if defined(__RE_RENDER_DEVICE_OPENGL__) 
//	static RERenderDeviceOpenGL * defaultDeviceOpenGL();
//#elif defined(__RE_RENDER_DEVICE_DIRECTX9__) 
//	static RERenderDeviceDirectX9 * defaultDeviceDirectX9();
//#endif	
	
	static void releaseDefaultDevice();
};

#endif /* __RERENDERDEVICE_H__ */ 



