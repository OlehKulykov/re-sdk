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


#include "../../include/regui/RERenderDeviceOpenGL.h"

#if (defined(__RE_USING_OPENGL_ES__) || defined(__RE_USING_OPENGL__))

#include "../../include/regui/private/RETextureOpenGL.h"

void RERenderDeviceOpenGL::StartRendering()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glViewport(0, 0, _screenSize.width, _screenSize.height);
	glLoadIdentity();
}

void RERenderDeviceOpenGL::EndRendering()
{
	
}

void RERenderDeviceOpenGL::RenderRect(const RETetragon & tetragon, 
									  const REColor & color, 
									  const RETetragon & textureFrame, 
									  IRETexture * texture)
{
	if (texture) 
	{
		if (texture->IsBlended() || (color.alpha < 1.0f)) { this->EnableBlending(); }
		else { this->DisableBlending(); }
		this->EnableTexturing();
		glColor4f(color.red, color.green, color.blue, color.alpha);
		texture->Bind();
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
		this->DisableTexturing();
		if (color.alpha < 1.0f) { this->EnableBlending(); }
		else { this->DisableBlending(); }
		glColor4f(color.red, color.green, color.blue, color.alpha);
		glEnableClientState(GL_VERTEX_ARRAY); 
		glVertexPointer(2, GL_FLOAT, 0, tetragon.arr);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void RERenderDeviceOpenGL::RenderRect(const RERect & rect, const REColor & color, IRETexture * texture)
{
	this->RenderRect(rect, color, RETetragon(RERect(0.0f, 0.0f, 1.0f, 1.0f)), texture);
}

void RERenderDeviceOpenGL::RenderRect(const RERect & rect, 
									  const REColor & color, 
									  const RETetragon & textureFrame, 
									  IRETexture * texture)
{	
	const GLfloat x_width = (rect.x + rect.width);
	const GLfloat y_height = (rect.y + rect.height);
	const GLfloat verticesArray[8] = { rect.x, rect.y, x_width, rect.y, rect.x, y_height, x_width, y_height };
	if (texture) 
	{
		if (texture->IsBlended() || (color.alpha < 1.0f)) { this->EnableBlending(); }
		else { this->DisableBlending(); }
		this->EnableTexturing();
		glColor4f(color.red, color.green, color.blue, color.alpha);
		texture->Bind();
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
		this->DisableTexturing();
		if (color.alpha < 1.0f) { this->EnableBlending(); }
		else { this->DisableBlending(); }
		glColor4f(color.red, color.green, color.blue, color.alpha);
		glEnableClientState(GL_VERTEX_ARRAY); 
		glVertexPointer(2, GL_FLOAT, 0, verticesArray);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void RERenderDeviceOpenGL::SetFullScreen(REBOOL isFullScreen)
{
	
	_isFullScreen = isFullScreen;
}

REBOOL RERenderDeviceOpenGL::InitializeWithWindow(void * windowHandle)
{
	return this->Initialize();
}

REBOOL RERenderDeviceOpenGL::Initialize()
{
	if ( !_isInitialized )
	{
		glShadeModel(GL_SMOOTH);
		this->SetDefaultBlendFunction();
		glAlphaFunc(GL_GREATER, 0.1f);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearColor(_clearColor.red, _clearColor.green, _clearColor.blue, _clearColor.alpha);
		
#ifndef __RE_USING_OPENGL_ES__
		glColor3f(1.0f, 1.0f, 1.0f);
#endif
		
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		
#ifndef __RE_USING_OPENGL_ES__
		glOrtho(0.0f, _renderSize.width, _renderSize.height, 0.0f, -1.0f, 1.0f);
#endif

#ifdef __RE_USING_OPENGL_ES__
		glOrthof(0.0f, _renderSize.width, _renderSize.height, 0.0f, -1.0f, 1.0f);
#endif
		_isInitialized = true;
		
		_currentRenderMode = RERenderDeviceMode3D;
	}
	return _isInitialized;
}

void RERenderDeviceOpenGL::ChangeOrtho()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei)_screenSize.width, (GLsizei)_screenSize.height); 
	
#ifndef __RE_USING_OPENGL_ES__
	if (_screenSize.height != 0.0f) 
	{
		gluPerspective(45.0f, (GLfloat)_screenSize.width / (GLfloat)_screenSize.height, 0.1f, 100.0f);
	}
#endif
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
#ifndef __RE_USING_OPENGL_ES__
	glOrtho(0.0f, _renderSize.width, _renderSize.height, 0.0f, -1.0f, 1.0f);
#endif
	
#ifdef __RE_USING_OPENGL_ES__
	glOrthof(0.0f, _renderSize.width, _renderSize.width, 0.0f, -1.0f, 1.0f);
#endif
}

void RERenderDeviceOpenGL::OnRenderSizeChanged()
{
	this->ChangeOrtho();
}

void RERenderDeviceOpenGL::OnScreenSizeChanged()
{
	this->ChangeOrtho();
}

void RERenderDeviceOpenGL::OnClearColorChanged()
{
	glClearColor(_clearColor.red, _clearColor.green, _clearColor.blue, _clearColor.alpha);
}

/* render mode */
void RERenderDeviceOpenGL::Enter2DMode()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
#if (defined(__RE_OS_WINDOWS__) || defined(__RE_OS_MACOSX__) || defined(__RE_OS_LINUX__))
	glOrtho(0.0f, _renderSize.width, _renderSize.height, 0.0f, -1.0f, 1.0f);
#endif
	
#if defined(__RE_OS_IPHONE__)
	glOrthof(0.0f, _renderSize.width, _renderSize.height, 0.0f, -1.0f, 1.0f);
#endif
	
#ifdef __RE_OS_ANDROID__
	glOrthof(0.0f, _renderSize.width, _renderSize.height, 0.0f, -1.0f, 1.0f);
#endif	
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	
	_currentRenderMode = RERenderDeviceMode2D;
}

void RERenderDeviceOpenGL::Leave2DMode()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	
	_currentRenderMode = RERenderDeviceMode3D;
}

RERenderDeviceTextureObject * RERenderDeviceOpenGL::CreateNewTexture()
{
	RETextureOpenGL * openGLTexture = RETextureOpenGL::Create();
	return openGLTexture;
}

const RERenderDeviceBlendType RERenderDeviceOpenGL::GetBlendSourceFactor() const
{
	return this->GetBlendTypeFromGLValue(_blendSFactor);
}

const RERenderDeviceBlendType RERenderDeviceOpenGL::GetBlendDestinationFactor() const
{
	return this->GetBlendTypeFromGLValue(_blendDFactor);
}

void RERenderDeviceOpenGL::SetBlendFunction(const RERenderDeviceBlendType newSFactor, const RERenderDeviceBlendType newDFactor)
{
	const GLenum sf = this->GetGLBlendValueFromEnum(newSFactor);
	const GLenum df = this->GetGLBlendValueFromEnum(newDFactor);
	this->SetGLBlendFunction(sf, df);
}

void RERenderDeviceOpenGL::SetDefaultBlendFunction()
{
	this->SetGLBlendFunction(__RENDER_DEVICE_OPENGL_DEFAULT_BLEND_S_FACTOR__, __RENDER_DEVICE_OPENGL_DEFAULT_BLEND_D_FACTOR__);
}

RERenderDeviceOpenGL::RERenderDeviceOpenGL() : RERenderDevice() ,
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


