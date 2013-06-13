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


#ifndef __RETEXTUREOPENGL_H__
#define __RETEXTUREOPENGL_H__

#include "../../recore/RECommonHeader.h"

#if (defined(__RE_USING_OPENGL_ES__) || defined(__RE_USING_OPENGL__))

#include "../../recore/REObject.h"
#include "../../recore/RETetragon.h"
#include "../RERenderDeviceTextureObject.h"


class __RE_PUBLIC_CLASS_API__ RETextureOpenGL : public RERenderDeviceTextureObject 
{
private:
	GLuint _textureIdentifierOpenGL;
	REUByte _filterType;
	REBOOL _isBlended;

	const GLuint CreateTextureIdentifierMainThread() const
	{
		GLuint oneTextureArray[1] = { 0 };
		GLuint * texturesArray = oneTextureArray;
		glGenTextures(1, texturesArray);
		return oneTextureArray[0];
	}
	
	void DeleteTextureIdentifierMainThread(const GLuint ti) const
	{
		GLuint oneTextureArray[1] = { ti };
		GLuint * delArray = oneTextureArray;
		glDeleteTextures(1, delArray);
	}
	
	void DeleteTextureIdentifierTaskMethod(REObject * taskObject);
	
	REBOOL InitTextureIdentifierMainThread();
	void UpdateTaskMethod(REObject * taskObject);
	REBOOL UpdateMainThread(const REUByte * pixelsData, 
							const REImagePixelFormat pixelsFormat,
							const REUInt32 width,
							const REUInt32 height);
	void SetFilterTypeTaskMethod(REObject * taskObject);
	void SetFilterTypeMainThread(const RETextureFilterType filterType);
	
protected:
	RETextureOpenGL();
	virtual ~RETextureOpenGL();
	
public:
	/* IRETexture */
	virtual const GLuint GetTexureIdentifier() const { return _textureIdentifierOpenGL; }
	virtual REBOOL IsNull() const { return (_textureIdentifierOpenGL == 0); }
	virtual REBOOL IsMipmaped() const;
	virtual REBOOL Update(const REUByte * pixelsData, 
						  const REImagePixelFormat pixelsFormat,
						  const REUInt32 width,
						  const REUInt32 height);
	virtual const REBOOL IsBlended() const { return _isBlended; }
	virtual void SetFilterType(const RETextureFilterType filter);
	virtual const RETextureFilterType GetFilterType() const;
	virtual void Bind() const { glBindTexture(GL_TEXTURE_2D, _textureIdentifierOpenGL); }
	
	/* REObject */
	virtual void OnReleased();
	
	static RETextureOpenGL * Create();
};


#endif /* __RE_USING_OPENGL__ || __RE_USING_OPENGL_ES__ */

#endif /* __RETEXTUREOPENGL_H__ */ 


