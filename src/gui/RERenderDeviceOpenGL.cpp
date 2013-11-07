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


#include "../../include/regui/private/RERenderDeviceOpenGL.h"

#if defined(IS_RE_USING_OPENGL) 

void RERenderDeviceOpenGL::startRendering()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glViewport(0, 0, _viewPortWidth, _viewPortHeight); 
	glLoadIdentity();
}

void RERenderDeviceOpenGL::endRendering()
{
	
}

void RERenderDeviceOpenGL::renderRect(const RETetragon & tetragon, 
									  const REColor & color, 
									  const RETetragon & textureFrame, 
									  IRETexture * texture)
{
	if (texture) 
	{
		if (texture->isBlended() || (color.alpha < 1.0f)) { this->enableBlending(); }
		else { this->disableBlending(); }
		this->enableTexturing();
		glColor4f(color.red, color.green, color.blue, color.alpha);
		texture->bind();
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, textureFrame.arr);
		glEnableClientState(GL_VERTEX_ARRAY); 
		glVertexPointer(2, GL_FLOAT, 0, tetragon.arr);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY); 
	}
	else 
	{ 
		this->disableTexturing();
		if (color.alpha < 1.0f) { this->enableBlending(); }
		else { this->disableBlending(); }
		glColor4f(color.red, color.green, color.blue, color.alpha);
		glEnableClientState(GL_VERTEX_ARRAY); 
		glVertexPointer(2, GL_FLOAT, 0, tetragon.arr);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void RERenderDeviceOpenGL::renderRect(const RERect & rect, const REColor & color, IRETexture * texture)
{
	this->renderRect(rect, color, RETetragon(RERect(0.0f, 0.0f, 1.0f, 1.0f)), texture);
}

void RERenderDeviceOpenGL::renderRect(const RERect & rect, 
									  const REColor & color, 
									  const RETetragon & textureFrame, 
									  IRETexture * texture)
{	
	const GLfloat x_width = (rect.x + rect.width);
	const GLfloat y_height = (rect.y + rect.height);
	const GLfloat verticesArray[8] = { rect.x, rect.y, x_width, rect.y, rect.x, y_height, x_width, y_height };
	if (texture) 
	{
		if (texture->isBlended() || (color.alpha < 1.0f)) { this->enableBlending(); }
		else { this->disableBlending(); }
		this->enableTexturing();
		glColor4f(color.red, color.green, color.blue, color.alpha);
		texture->bind();
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, textureFrame.arr);
		glEnableClientState(GL_VERTEX_ARRAY); 
		glVertexPointer(2, GL_FLOAT, 0, verticesArray);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY); 
	}
	else 
	{ 
		this->disableTexturing();
		if (color.alpha < 1.0f) { this->enableBlending(); }
		else { this->disableBlending(); }
		glColor4f(color.red, color.green, color.blue, color.alpha);
		glEnableClientState(GL_VERTEX_ARRAY); 
		glVertexPointer(2, GL_FLOAT, 0, verticesArray);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void RERenderDeviceOpenGL::setFullScreen(REBOOL isFullScreen)
{
	
	_isFullScreen = isFullScreen;
}

REBOOL RERenderDeviceOpenGL::initializeWithWindow(void * windowHandle)
{
	return this->initialize();
}

REBOOL RERenderDeviceOpenGL::initialize()
{
	if ( !_isInitialized )
	{
		glShadeModel(GL_SMOOTH);
		this->setDefaultBlendFunction();
		glAlphaFunc(GL_GREATER, 0.1f);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearColor(_clearColor.red, _clearColor.green, _clearColor.blue, _clearColor.alpha);
		
#if	defined(HAVE_FUNCTION_GLCOLOR3F)
		glColor3f(1.0f, 1.0f, 1.0f);
#endif	
	
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		
#if defined(HAVE_FUNCTION_GLORTHO)		
		glOrtho(0.0f, _renderSize.width, _renderSize.height, 0.0f, -1.0f, 1.0f);
#elif defined(HAVE_FUNCTION_GLORTHOF)		
		glOrthof(0.0f, _renderSize.width, _renderSize.height, 0.0f, -1.0f, 1.0f);
#endif		
		
		_isInitialized = true;
		
		_currentRenderMode = RERenderDeviceMode3D;
	}
	return _isInitialized;
}

void RERenderDeviceOpenGL::changeOrtho()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, _viewPortWidth, _viewPortHeight); 
	
#if defined(HAVE_FUNCTION_GLUPERSPECTIVE)	
	if (_screenSize.height != 0.0f) 
	{
		gluPerspective(45.0f, (GLfloat)_screenSize.width / (GLfloat)_screenSize.height, 0.1f, 100.0f);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

#else
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#if defined(HAVE_FUNCTION_GLORTHO)
	glOrtho(0.0f, _renderSize.width, _renderSize.height, 0.0f, -1.0f, 1.0f);
#elif defined(HAVE_FUNCTION_GLORTHOF)	
	glOrthof(0.0f, _renderSize.width, _renderSize.width, 0.0f, -1.0f, 1.0f);
#endif	
	
#endif	
	
}

void RERenderDeviceOpenGL::onRenderSizeChanged()
{
	_viewPortWidth = (GLsizei)_screenSize.width;
	_viewPortHeight = (GLsizei)_screenSize.height;
	this->changeOrtho();
}

void RERenderDeviceOpenGL::onScreenSizeChanged()
{
	_viewPortWidth = (GLsizei)_screenSize.width;
	_viewPortHeight = (GLsizei)_screenSize.height;
	this->changeOrtho();
}

void RERenderDeviceOpenGL::onClearColorChanged()
{
	glClearColor(_clearColor.red, _clearColor.green, _clearColor.blue, _clearColor.alpha);
}

/* render mode */
void RERenderDeviceOpenGL::enter2DMode()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
#if defined(HAVE_FUNCTION_GLORTHO)
	glOrtho(0.0f, _renderSize.width, _renderSize.height, 0.0f, -1.0f, 1.0f);
#elif defined(HAVE_FUNCTION_GLORTHOF)	
	glOrthof(0.0f, _renderSize.width, _renderSize.height, 0.0f, -1.0f, 1.0f);
#endif
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	
	_currentRenderMode = RERenderDeviceMode2D;
}

void RERenderDeviceOpenGL::leave2DMode()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	
	_currentRenderMode = RERenderDeviceMode3D;
}

const RERenderDeviceBlendType RERenderDeviceOpenGL::blendSourceFactor() const
{
	return this->blendTypeFromGLValue(_blendSFactor);
}

const RERenderDeviceBlendType RERenderDeviceOpenGL::blendDestinationFactor() const
{
	return this->blendTypeFromGLValue(_blendDFactor);
}

void RERenderDeviceOpenGL::setBlendFunction(const RERenderDeviceBlendType newSFactor, const RERenderDeviceBlendType newDFactor)
{
	const GLenum sf = this->GLBlendValueFromEnum(newSFactor);
	const GLenum df = this->GLBlendValueFromEnum(newDFactor);
	this->setGLBlendFunction(sf, df);
}

void RERenderDeviceOpenGL::setDefaultBlendFunction()
{
	this->setGLBlendFunction(__RENDER_DEVICE_OPENGL_DEFAULT_BLEND_S_FACTOR__, __RENDER_DEVICE_OPENGL_DEFAULT_BLEND_D_FACTOR__);
}

RERenderDeviceOpenGL::RERenderDeviceOpenGL() : RERenderDevice() ,
	_viewPortWidth(0),
	_viewPortHeight(0),
	_blendSFactor(0),
	_blendDFactor(0),
	_isTexture2DEnable(false),
	_isBlendingEnable(false)
{
	_currentRenderMode = RERenderDeviceModeNone;
	_deviceType = RERenderDeviceTypeOpenGL;
}

RERenderDeviceOpenGL::~RERenderDeviceOpenGL()
{
	
}


#endif /* __RE_USING_OPENGL_ES__ || __RE_USING_OPENGL__ */ 


