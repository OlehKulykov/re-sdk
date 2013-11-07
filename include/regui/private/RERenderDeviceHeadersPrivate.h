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


#ifndef __RERENDERDEVICEHEADERSPRIVATE_H__
#define __RERENDERDEVICEHEADERSPRIVATE_H__

#include "../../recore/RECommonHeader.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

#if !defined(IS_RE_USING_OPENGL) && !defined(IS_RE_USING_DIRECTX)

#if defined(HAVE_GL_GL_H)
#include <gl/gl.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_GL_GLU_H)
#include <gl/glu.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_OPENGL_OPENGL_H)
#include <OpenGL/OpenGL.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_OPENGL_GLU_H)
#include <OpenGL/glu.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_OPENGLES_ES1_GL_H)
#include <OpenGLES/ES1/gl.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_OPENGLES_ES1_GLEXT_H)
#include <OpenGLES/ES1/glext.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_OPENGLES_ES2_GL_H)
#include <OpenGLES/ES2/gl.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_OPENGLES_ES2_GLEXT_H)
#include <OpenGLES/ES2/glext.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE__GL_GL_H)
#include <GL/gl.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE__GL_GLU_H)
#include <GL/glu.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE__GL_GLUT_H)
#include <GL/glut.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_GLES_GL_H)
#include <GLES/gl.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_GLES_GLEXT_H)
#include <GLES/glext.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_GL_H)
#include <gl.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_GLEXT_H)
#include <glext.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif

#if defined(HAVE_GLMACRO_H)
#include <glmacro.h>
#ifndef IS_RE_USING_OPENGL
#define IS_RE_USING_OPENGL 1
#endif
#endif



#endif /* !defined(IS_RE_USING_OPENGL) && !defined(IS_RE_USING_DIRECTX) */

#endif /* __RERENDERDEVICEHEADERSPRIVATE_H__ */ 



