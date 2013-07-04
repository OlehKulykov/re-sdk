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


#include "../../include/regui/private/RETextureOpenGL.h"
#include "../../include/regui/RERenderDevice.h"
#include "../../include/recore/REThread.h"

#if (defined(__RE_USING_OPENGL_ES__) || defined(__RE_USING_OPENGL__))

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

class RETextureOpenGLSetFilterTypeMainThreadTaskPrivate : public REObject 
{
public:
	RETextureFilterType filterType;
	RETextureOpenGLSetFilterTypeMainThreadTaskPrivate() : REObject() { }
	virtual ~RETextureOpenGLSetFilterTypeMainThreadTaskPrivate() { }
};

void RETextureOpenGL::SetFilterTypeTaskMethod(REObject * taskObject)
{
	if (taskObject) 
	{
		RETextureOpenGLSetFilterTypeMainThreadTaskPrivate * task = 
		(RETextureOpenGLSetFilterTypeMainThreadTaskPrivate*)taskObject;
		this->SetFilterTypeMainThread(task->filterType);
	}
}

void RETextureOpenGL::SetFilterTypeMainThread(const RETextureFilterType filterType)
{
	glBindTexture(GL_TEXTURE_2D, _textureIdentifierOpenGL);
	RETextureFilterType actualFilter = filterType;
	
	if ((!this->IsMipmaped()) && 
		((filterType == RETextureFilterMipmapedStandart) || (filterType == RETextureFilterMipmapedTrilinear))) 
	{
		actualFilter = RETextureFilterLinear;
	}
	switch (actualFilter) 
	{
		case RETextureFilterNearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case RETextureFilterLinear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case RETextureFilterMipmapedStandart:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case RETextureFilterMipmapedTrilinear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		default:
			return;
			break;
	}
	_filterType = actualFilter;
}

REBOOL RETextureOpenGL::InitTextureIdentifierMainThread()
{
	_textureIdentifierOpenGL = this->CreateTextureIdentifierMainThread(); 
	if (_textureIdentifierOpenGL) 
	{	
		this->SetFilterTypeMainThread((RETextureFilterType)_filterType);
		// allready binded
		/*
		 GL_TEXTURE_WRAP_S	 
		 Sets the wrap parameter for texture coordinate s to either GL_CLAMP, GL_CLAMP_TO_EDGE, or GL_REPEAT. 
		 GL_CLAMP causes s coordinates to be clamped to the range [0, 1] and is useful 
		 for preventing wrapping artifacts when mapping a single image onto an object. 
		 GL_CLAMP_TO_EDGE causes s coordinates to be clamped to the range [ 1/2N , 1 - 1/2N ] , 
		 where N is the size of the texture in the direction of clamping. 
		 GL_REPEAT causes the integer part of the s coordinate to be ignored; 
		 the GL uses only the fractional part, thereby creating a repeating pattern. 
		 Border texture elements are accessed only if wrapping is set to GL_CLAMP. 
		 Initially, GL_TEXTURE_WRAP_S is set to GL_REPEAT.
		 */
		
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		return true;
	}
	return false;
}

class RETextureOpenGLUpdateMainThreadTaskPrivate : public REObject 
{
public:
	const REUByte * pixelsData;
	REImagePixelFormat pixelsFormat;
	REUInt32 width;
	REUInt32 height;
	REBOOL exeResult;
	RETextureOpenGLUpdateMainThreadTaskPrivate() : REObject() { }
	virtual ~RETextureOpenGLUpdateMainThreadTaskPrivate() { }
};

void RETextureOpenGL::UpdateTaskMethod(REObject * taskObject)
{
	if (taskObject) 
	{
		RETextureOpenGLUpdateMainThreadTaskPrivate * task = 
		(RETextureOpenGLUpdateMainThreadTaskPrivate*)taskObject;
		task->exeResult = this->UpdateMainThread(task->pixelsData, task->pixelsFormat, task->width, task->height);
	}
}

REBOOL RETextureOpenGL::UpdateMainThread(const REUByte * pixelsData, 
										 const REImagePixelFormat pixelsFormat,
										 const REUInt32 width,
										 const REUInt32 height)
{
	REBOOL isBinded = false;
	if (_textureIdentifierOpenGL == 0) 
	{
		if (this->InitTextureIdentifierMainThread()) { isBinded = true; }
		else { return false; }
	}
	
	if (_textureIdentifierOpenGL) 
	{ 
		GLenum textureType = 0;
		REBOOL isTransparent = false;
		switch (pixelsFormat) 
		{
			case REImagePixelFormatR8G8B8:
				textureType = GL_RGB;
				break;
			case REImagePixelFormatR8G8B8A8:
				textureType = GL_RGBA;
				isTransparent = true;
				break;
			case REImagePixelFormatLuminance8Alpha8:
				textureType = GL_LUMINANCE_ALPHA;
				isTransparent = true;
				break;	
			case REImagePixelFormatAlpha8:
				textureType = GL_ALPHA;
				isTransparent = true;
				break;	
			default:
				return false;
				break;
		}
		_isBlended = isTransparent;
		if (!isBinded) 
		{
			glBindTexture(GL_TEXTURE_2D, _textureIdentifierOpenGL);
		}
		
		const REBOOL isGenerateMipmaps = this->IsMipmaped();
		
#ifdef __RE_USING_OPENGL__
		if (isGenerateMipmaps) 
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D,
							  textureType,
							  width, 
							  height,
							  textureType,
							  GL_UNSIGNED_BYTE,
							  pixelsData);
		}
		else
		{
#endif 
			glTexImage2D(GL_TEXTURE_2D, 
						 0, 
						 textureType, 
						 width, 
						 height, 
						 0, 
						 textureType, 
						 GL_UNSIGNED_BYTE, 
						 pixelsData);
#ifdef __RE_USING_OPENGL__ 
		}
#endif 
		
#ifdef __RE_USING_OPENGL_ES__ 
		if (isGenerateMipmaps) 
		{
			// ES
			glGenerateMipmapOES(GL_TEXTURE_2D);		
		}
#endif 

		return true;
	}
	
	return false;
}

REBOOL RETextureOpenGL::Update(const REUByte * pixelsData, 
							   const REImagePixelFormat pixelsFormat,
							   const REUInt32 width,
							   const REUInt32 height)
{
	// don't need check _textureIdentifierOpenGL. It's will create if needed.
	if (REThread::isMainThread()) 
	{
		return this->UpdateMainThread(pixelsData, pixelsFormat, width, height);
	}
	else
	{
		RETextureOpenGLUpdateMainThreadTaskPrivate task;
		task.pixelsData = pixelsData;
		task.pixelsFormat = pixelsFormat;
		task.width = width;
		task.height = height;
		task.exeResult = false;
		REThread::performMethodOnMainThreadAndWaitUntilDone(NEW_CLASS_METHOD(RETextureOpenGL, this, UpdateTaskMethod),
															&task);
		return task.exeResult;
	}
	return false;
	//RERenderDeviceTextureManager * m = RERenderDevice::GetDefaultDevice()->GetTextureManager();
	//return m->Update(this, pixelsData, pixelsFormat, width, height);
}

void RETextureOpenGL::SetFilterType(const RETextureFilterType filter)
{
	_filterType = (REUByte)filter;
	
	if (_textureIdentifierOpenGL) 
	{
		if (REThread::isMainThread()) 
		{
			this->SetFilterTypeMainThread(filter);
		}
		else
		{
			RETextureOpenGLSetFilterTypeMainThreadTaskPrivate task;
			task.filterType = filter;
			REThread::performMethodOnMainThreadAndWaitUntilDone(NEW_CLASS_METHOD(RETextureOpenGL, this, SetFilterTypeTaskMethod), 
																&task);
		}
	}
}

const RETextureFilterType RETextureOpenGL::GetFilterType() const
{
	return (RETextureFilterType)_filterType;
}

REBOOL RETextureOpenGL::IsMipmaped() const
{
#ifndef __RE_OS_ANDROID__	
	return ((_filterType == RETextureFilterMipmapedStandart) || (_filterType == RETextureFilterMipmapedTrilinear));
#endif
	
#ifdef __RE_OS_ANDROID__
	return false;
#endif	
}

class RETextureOpenGLDeleteTextureIdentifierMainThreadTaskPrivate : public REObject 
{
public:
	GLuint textureIdentifier;
	RETextureOpenGLDeleteTextureIdentifierMainThreadTaskPrivate() : REObject() { }
	virtual ~RETextureOpenGLDeleteTextureIdentifierMainThreadTaskPrivate() { }
};

void RETextureOpenGL::DeleteTextureIdentifierTaskMethod(REObject * taskObject)
{
	if (taskObject) 
	{
		RETextureOpenGLDeleteTextureIdentifierMainThreadTaskPrivate * task =
		(RETextureOpenGLDeleteTextureIdentifierMainThreadTaskPrivate*)taskObject;
		this->DeleteTextureIdentifierMainThread(task->textureIdentifier);
	}
}

RETextureOpenGL::RETextureOpenGL() : RERenderDeviceTextureObject(),
	_textureIdentifierOpenGL(0),
	_filterType(0),
	_isBlended(false)
{

}

void RETextureOpenGL::onReleased()
{
	if (_textureIdentifierOpenGL) 
	{
		if (REThread::isMainThread()) 
		{
			this->DeleteTextureIdentifierMainThread(_textureIdentifierOpenGL);
		}
		else
		{
			RETextureOpenGLDeleteTextureIdentifierMainThreadTaskPrivate task;
			task.textureIdentifier = _textureIdentifierOpenGL;
			REThread::performMethodOnMainThreadAndWaitUntilDone(NEW_CLASS_METHOD(RETextureOpenGL, this, DeleteTextureIdentifierTaskMethod),
																&task);
		}
		_textureIdentifierOpenGL = 0;
	}
	
	RERenderDeviceTextureObject::onReleased();
}

RETextureOpenGL::~RETextureOpenGL() 
{ 
	
}

RETextureOpenGL * RETextureOpenGL::Create()
{
	RETextureOpenGL * newOpenGLTexture = new RETextureOpenGL();
	return newOpenGLTexture;
}

#endif /* __RE_USING_OPENGL_ES__ || __RE_USING_OPENGL__ */


