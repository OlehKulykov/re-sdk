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


#ifndef __RERENDERDEVICEOPENGL_H__
#define __RERENDERDEVICEOPENGL_H__

#include "../../recore/RECommonHeader.h"
#include "RERenderDeviceHeadersPrivate.h"


#if defined(IS_RE_USING_OPENGL)

#include "../RERenderDevice.h"

#define __RENDER_DEVICE_OPENGL_DEFAULT_BLEND_S_FACTOR__ GL_SRC_ALPHA
#define __RENDER_DEVICE_OPENGL_DEFAULT_BLEND_D_FACTOR__ GL_ONE_MINUS_SRC_ALPHA

class __RE_PUBLIC_CLASS_API__ RERenderDeviceOpenGL : public RERenderDevice
{
private:
	GLsizei _viewPortWidth;
	GLsizei _viewPortHeight;
	GLenum _blendSFactor;
	GLenum _blendDFactor;
	REBOOL _isTexture2DEnable;
	REBOOL _isBlendingEnable;
	
	void changeOrtho();
	void enableBlending() 
	{ 
		if (!_isBlendingEnable) 
		{
			_isBlendingEnable = true; 
			glEnable(GL_BLEND); 
		}
	}
	
	void disableBlending()
	{
		if (_isBlendingEnable) 
		{
			_isBlendingEnable = false;
			glDisable(GL_BLEND); 
		}
	}
	
	void enableTexturing()
	{
		if (!_isTexture2DEnable) 
		{
			_isTexture2DEnable = true; 
			glEnable(GL_TEXTURE_2D); 
		}
	}
	
	void disableTexturing()
	{
		if (_isTexture2DEnable) 
		{
			_isTexture2DEnable = false; 
			glDisable(GL_TEXTURE_2D); 
		}
	}
	
	const RERenderDeviceBlendType blendTypeFromGLValue(const GLenum e) const
	{
		switch (e) 
		{
			case GL_SRC_ALPHA: return RERenderDeviceBlendType_SRC_ALPHA; break;
			case GL_ONE_MINUS_SRC_ALPHA: return RERenderDeviceBlendType_ONE_MINUS_SRC_ALPHA; break;
			case GL_ONE: return RERenderDeviceBlendType_ONE; break;
			case GL_DST_COLOR: return RERenderDeviceBlendType_DST_COLOR; break;
			case GL_ONE_MINUS_DST_COLOR: return RERenderDeviceBlendType_ONE_MINUS_DST_COLOR; break;
			case GL_DST_ALPHA: return RERenderDeviceBlendType_DST_ALPHA; break;
			case GL_ONE_MINUS_DST_ALPHA: return RERenderDeviceBlendType_ONE_MINUS_DST_ALPHA; break;
			case GL_SRC_ALPHA_SATURATE: return RERenderDeviceBlendType_SRC_ALPHA_SATURATE; break;
			default: break;
		}
		return RERenderDeviceBlendType_ZERO;
	}
	
	const GLenum GLBlendValueFromEnum(const RERenderDeviceBlendType t) const
	{
		switch (t) 
		{
			case RERenderDeviceBlendType_SRC_ALPHA: return GL_SRC_ALPHA;  break;
			case RERenderDeviceBlendType_ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;  break;
			case RERenderDeviceBlendType_ONE: return GL_ONE; break;
			case RERenderDeviceBlendType_DST_COLOR: return GL_DST_COLOR; break;
			case RERenderDeviceBlendType_ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR; break;
			case RERenderDeviceBlendType_DST_ALPHA: return GL_DST_ALPHA; break;
			case RERenderDeviceBlendType_ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA; break;
			case RERenderDeviceBlendType_SRC_ALPHA_SATURATE: return GL_SRC_ALPHA_SATURATE; break;
			default: break;
		}
		return GL_ZERO;
	}
protected:
	virtual void onRenderSizeChanged();
	virtual void onScreenSizeChanged();
	virtual void onClearColorChanged();
	
public:
	virtual const RERenderDeviceBlendType blendSourceFactor() const;
	virtual const RERenderDeviceBlendType blendDestinationFactor() const;
	virtual void setBlendFunction(const RERenderDeviceBlendType newSFactor, const RERenderDeviceBlendType newDFactor);
	virtual void setDefaultBlendFunction();
	void setGLBlendFunction(const GLenum newSFactor, const GLenum newDFactor)
	{
		if ( (_blendSFactor != newSFactor) || (_blendDFactor != newDFactor) ) 
		{
			glBlendFunc(newSFactor, newDFactor);
			_blendSFactor = newSFactor;
			_blendDFactor = newDFactor;
		}
	}
	
	/* init */
	virtual REBOOL initialize();
	virtual REBOOL initializeWithWindow(void * windowHandle);

	/* setup */
	virtual void setFullScreen(REBOOL isFullScreen);
	
	virtual void startRendering();
	virtual void endRendering();
	
	virtual void renderRect(const RETetragon & tetragon, 
							const REColor & color, 
							const RETetragon & textureFrame, 
							IRETexture * texture);
	virtual void renderRect(const RERect & rect, const REColor & color, IRETexture * texture);
	virtual void renderRect(const RERect & rect, 
							const REColor & color, 
							const RETetragon & textureFrame, 
							IRETexture * texture);

	/* render mode */
	virtual void enter2DMode();
	virtual void leave2DMode();
	
	
	RERenderDeviceOpenGL();
	virtual ~RERenderDeviceOpenGL();
};

#endif /* __RE_USING_OPENGL_ES__ || __RE_USING_OPENGL__ */

#endif /* __RERENDERDEVICEOPENGL_H__ */

