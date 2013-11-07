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


#ifndef __RETEXTUREOPENGLPRIVATE_H__
#define __RETEXTUREOPENGLPRIVATE_H__

#include "../../recore/RECommonHeader.h"
#include "../../regui/IRETexture.h"
#include "RERenderDeviceHeadersPrivate.h"

#if defined(IS_RE_USING_OPENGL)

#if defined(__RE_DEBUG_MODE__)
#warning "OpenGL internal texture"
#endif

class __RE_PUBLIC_CLASS_API__ RETextureInternal
#if defined(__RE_DEBUG_MODE__)
: public IRETexture
#endif
{	
private:
	GLuint _identifier;
	REByte _filterType;
	REBOOL _isBlended;
	
	typedef struct _updateStruct
	{
		GLuint * identifierPtr;
		const REUByte * pixelsData;
		RETextureFilterType * filterTypePtr;
		REUInt32 width;
		REUInt32 height;
		REImagePixelFormat pixelsFormat;
		REBOOL isOk;
	}UpdateStruct;
	
	static void deleteTextureIdentifier(void * identifier);

	static void updateS(void * updateStruct);
	static void update(GLuint identifier,
					   const REUByte * pixelsData, 
					   const REImagePixelFormat pixelsFormat,
					   const REUInt32 width,
					   const REUInt32 height,
					   RETextureFilterType filterType);
	
	static void filterS(void * updateStruct);
	static void filter(GLuint identifier, RETextureFilterType * filterType);
	
	static void updateAllS(void * updateStruct);
	static void updateAll(GLuint * identifier,
						  const REUByte * pixelsData, 
						  const REImagePixelFormat pixelsFormat,
						  const REUInt32 width,
						  const REUInt32 height,
						  RETextureFilterType * filterType,
						  REBOOL * isOk);
public:
#if defined(__RE_DEBUG_MODE__)
	virtual
#endif	
	const GLuint texureIdentifier() const { return _identifier; }


#if defined(__RE_DEBUG_MODE__)	
	virtual 
#endif	
	REBOOL isNull() const { return (_identifier == 0); }
	
	
#if defined(__RE_DEBUG_MODE__)
	virtual 
#endif	
	REBOOL isMipmaped() const;
	
	
#if defined(__RE_DEBUG_MODE__)
	virtual 
#endif	
	REBOOL update(const REUByte * pixelsData, 
				  const REImagePixelFormat pixelsFormat,
				  const REUInt32 width,
				  const REUInt32 height);
	
	
#if defined(__RE_DEBUG_MODE__)
	virtual 
#endif	
	const REBOOL isBlended() const { return _isBlended; }
	
	
#if defined(__RE_DEBUG_MODE__)
	virtual 
#endif	
	void setFilterType(const RETextureFilterType filter);
	
	
#if defined(__RE_DEBUG_MODE__)
	virtual 
#endif	
	const RETextureFilterType filterType() const { return (RETextureFilterType)_filterType; }
	
	
#if defined(__RE_DEBUG_MODE__)
	virtual 
#endif	
	void bind() const { glBindTexture(GL_TEXTURE_2D, _identifier); }
	
	
	void cleanup();
	
	/// create new identifier, set filter and update pixels
	REBOOL initWithPixelsData(const REUByte * pixelsData, 
							  const REImagePixelFormat pixelsFormat,
							  const REUInt32 width,
							  const REUInt32 height,
							  const RETextureFilterType filterType);
	
	RETextureInternal();

	
#if defined(__RE_DEBUG_MODE__)	
	virtual 
#endif	
	~RETextureInternal();
	
	static REBOOL isMipmaped(const RETextureFilterType type);
	static REBOOL isBlended(const REImagePixelFormat pixelsFormat);
};


#endif /* IS_RE_USING_OPENGL */


#endif /* __RETEXTUREOPENGLPRIVATE_H__ */ 


