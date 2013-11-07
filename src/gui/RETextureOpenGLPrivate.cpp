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


#include "../../include/regui/private/RETextureOpenGLPrivate.h"

#if defined(IS_RE_USING_OPENGL)


#if defined(HAVE_FUNCTION_GLUBUILD2DMIPMAPS) || defined(HAVE_FUNCTION_GLGENERATEMIPMAPOES)
#define IS_OPENGL_MIPMAPS 1
#endif

#include "../../include/recore/REThread.h"
#include "../../include/recore/REPtr.h"

void RETextureInternal::deleteTextureIdentifier(void * identifier)
{
	const GLuint * textures = (const GLuint *)identifier;
	glDeleteTextures(1, textures);
}

void RETextureInternal::setFilterType(const RETextureFilterType filter)
{
	if (REThread::isMainThread()) 
	{
		RETextureFilterType filterType = filter;
		RETextureInternal::filter(_identifier, &filterType);
		_filterType = filterType;
	}
	else
	{
		RETextureInternal::UpdateStruct s;
		RETextureFilterType filterType = filter;
		GLuint identifier = _identifier;
		s.identifierPtr = &identifier;
		s.filterTypePtr = &filterType;
		REThread::performFunctionOnMainThreadAndWaitUntilDone(RETextureInternal::filterS, &s);
		_filterType = filterType;
	}
}

REBOOL RETextureInternal::update(const REUByte * pixelsData, 
								 const REImagePixelFormat pixelsFormat,
								 const REUInt32 width,
								 const REUInt32 height)
{
	if (REThread::isMainThread()) 
	{
		const RETextureFilterType filterType = (RETextureFilterType)_filterType;
		RETextureInternal::update(_identifier, pixelsData, pixelsFormat, width, height, filterType);
		_isBlended = RETextureInternal::isBlended(pixelsFormat);
	}
	else
	{
		RETextureInternal::UpdateStruct s;
		RETextureFilterType filterType = (RETextureFilterType)_filterType;
		GLuint identifier = _identifier;
		s.identifierPtr = &identifier;
		s.filterTypePtr = &filterType;
		s.pixelsData = pixelsData;
		s.pixelsFormat = pixelsFormat;
		s.width = width;
		s.height = height;
		REThread::performFunctionOnMainThreadAndWaitUntilDone(RETextureInternal::updateS, &s);
		_isBlended = RETextureInternal::isBlended(pixelsFormat);
	}
	return true;
}

REBOOL RETextureInternal::isMipmaped() const
{
	return RETextureInternal::isMipmaped((RETextureFilterType)_filterType);
}

void RETextureInternal::cleanup()
{
	if (_identifier) 
	{
		GLuint identifier = _identifier;
		_identifier = 0;
		if (REThread::isMainThread()) 
		{
			RETextureInternal::deleteTextureIdentifier(&identifier); 
		}
		else 
		{
			REThread::performFunctionOnMainThreadAndWaitUntilDone(RETextureInternal::deleteTextureIdentifier, &identifier);
		}
	}
}

void RETextureInternal::updateS(void * updateStruct)
{
	RETextureInternal::UpdateStruct * s = (RETextureInternal::UpdateStruct *)updateStruct;
	RETextureInternal::update(*s->identifierPtr, s->pixelsData, s->pixelsFormat, s->width, s->height, *s->filterTypePtr);
}

void RETextureInternal::update(GLuint identifier,
							   const REUByte * pixelsData, 
							   const REImagePixelFormat pixelsFormat,
							   const REUInt32 width,
							   const REUInt32 height,
							   RETextureFilterType filterType)
{
	GLenum textureType = 0;
	switch (pixelsFormat) 
	{
		case REImagePixelFormatR8G8B8:
			textureType = GL_RGB;
			break;
		case REImagePixelFormatR8G8B8A8:
			textureType = GL_RGBA;
			break;
		case REImagePixelFormatLuminance8Alpha8:
			textureType = GL_LUMINANCE_ALPHA;
			break;	
		case REImagePixelFormatAlpha8:
			textureType = GL_ALPHA;
			break;	
		default:
			return;
			break;
	}
	glBindTexture(GL_TEXTURE_2D, identifier);
	
	
#if defined(IS_OPENGL_MIPMAPS)	
	/* MIPMAPS */
	
	const REBOOL isGenerateMipmaps = RETextureInternal::isMipmaped(filterType);
	if (isGenerateMipmaps) 
	{
#if defined(HAVE_FUNCTION_GLUBUILD2DMIPMAPS)		
		gluBuild2DMipmaps(GL_TEXTURE_2D, textureType, width, height, textureType, GL_UNSIGNED_BYTE, pixelsData);
#else
		
#if defined(HAVE_FUNCTION_GLTEXIMAGE2D)		
		glTexImage2D(GL_TEXTURE_2D, 0, textureType, width, height, 0, textureType, GL_UNSIGNED_BYTE, pixelsData);
#endif	
#if defined(HAVE_FUNCTION_GLGENERATEMIPMAPOES)
		glGenerateMipmapOES(GL_TEXTURE_2D);		
#endif	
		
#endif		
	}
	else
	{
#if defined(HAVE_FUNCTION_GLTEXIMAGE2D)		
		glTexImage2D(GL_TEXTURE_2D, 0, textureType, width, height, 0, textureType, GL_UNSIGNED_BYTE, pixelsData);
#endif	
	}
	
	/* MIPMAPS */
#else 
	/* NO MIPMAPS */
	
#if defined(HAVE_FUNCTION_GLTEXIMAGE2D)		
	glTexImage2D(GL_TEXTURE_2D, 0, textureType, width, height, 0, textureType, GL_UNSIGNED_BYTE, pixelsData);
#endif /* HAVE_FUNCTION_GLTEXIMAGE2D */
	
	/* NO MIPMAPS */
#endif	

}

void RETextureInternal::filterS(void * updateStruct)
{
	RETextureInternal::UpdateStruct * s = (RETextureInternal::UpdateStruct *)updateStruct;
	RETextureInternal::filter(*s->identifierPtr, s->filterTypePtr);
}

void RETextureInternal::filter(GLuint identifier, RETextureFilterType * filterType)
{
	glBindTexture(GL_TEXTURE_2D, identifier);
	RETextureFilterType actualFilter = *filterType;
	
	if ((!RETextureInternal::isMipmaped(actualFilter)) && 
		((actualFilter == RETextureFilterMipmapedStandart) || (actualFilter == RETextureFilterMipmapedTrilinear))) 
	{
		*filterType = actualFilter = RETextureFilterLinear;
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
}

void RETextureInternal::updateAll(GLuint * identifier,
							   const REUByte * pixelsData, 
							   const REImagePixelFormat pixelsFormat,
							   const REUInt32 width,
							   const REUInt32 height,
							   RETextureFilterType * filterType,
							   REBOOL * isOk)
{
	if (*identifier == 0)
	{
		glGenTextures(1, identifier);
		if (*identifier == 0) { *isOk = false; return; }
	}
	
	RETextureInternal::filter(*identifier, filterType);
	
	if (pixelsData) 
	{
		RETextureInternal::update(*identifier, pixelsData, pixelsFormat, width, height, *filterType);
	}
	
	*isOk = true;
}

void RETextureInternal::updateAllS(void * updateStruct)
{
	RETextureInternal::UpdateStruct * s = (RETextureInternal::UpdateStruct*)updateStruct;
	RETextureInternal::updateAll(s->identifierPtr, s->pixelsData, s->pixelsFormat, s->width, s->height, s->filterTypePtr, &s->isOk);
}

REBOOL RETextureInternal::initWithPixelsData(const REUByte * pixelsData, 
											 const REImagePixelFormat pixelsFormat,
											 const REUInt32 width,
											 const REUInt32 height,
											 const RETextureFilterType filterType)
{
	if (REThread::isMainThread()) 
	{
		REBOOL isOk = false;
		RETextureFilterType filter = filterType;
		GLuint identifier = 0;
		RETextureInternal::updateAll(&identifier, pixelsData, pixelsFormat, width, height, &filter, &isOk);
		if (isOk) 
		{
			_identifier = identifier;
			_filterType = filter;
			_isBlended = RETextureInternal::isBlended(pixelsFormat);
			return true;
		}
	}
	else 
	{
		RETextureInternal::UpdateStruct s;
		RETextureFilterType filter = filterType;
		GLuint identifier = 0;
		s.identifierPtr = &identifier;
		s.pixelsData = pixelsData;
		s.pixelsFormat = pixelsFormat;
		s.width = width;
		s.height = height;
		s.filterTypePtr = &filter;
		s.isOk = false;
		REThread::performFunctionOnMainThreadAndWaitUntilDone(RETextureInternal::updateAllS, &s);
		if (s.isOk) 
		{
			_identifier = identifier;
			_filterType = filter;
			_isBlended = RETextureInternal::isBlended(pixelsFormat);
			return true;
		}
	}
	
	return false;
}

RETextureInternal::RETextureInternal() :
	_identifier(0),
	_filterType(RETextureFilterNone),
	_isBlended(false)
{
	
}

RETextureInternal::~RETextureInternal()
{
	this->cleanup();
}

REBOOL RETextureInternal::isMipmaped(const RETextureFilterType type)
{
#if defined(IS_OPENGL_MIPMAPS) 
	return ((type == RETextureFilterMipmapedStandart) || (type == RETextureFilterMipmapedTrilinear));
#else
	return false;
#endif
}

REBOOL RETextureInternal::isBlended(const REImagePixelFormat pixelsFormat)
{
	return ((pixelsFormat == REImagePixelFormatR8G8B8A8) || 
			(pixelsFormat == REImagePixelFormatLuminance8Alpha8) || 
			(pixelsFormat == REImagePixelFormatAlpha8));
}

#endif


