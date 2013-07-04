/*
 * Copyright (c) 2002-2009 Fenomen Games.  All Rights Reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * You may freely use or modify this code
 *
 * Support: pyro-support@fenomen-games.com
 *
 */

#ifndef _PYRO_SAMPLES_COMMON_OPENGL_H_INCLUDED
#define _PYRO_SAMPLES_COMMON_OPENGL_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

#include <assert.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif /* _WINDOWS */

#ifdef ANDROID
#include <GLES/gl.h>
#else
#include <GL/gl.h>
#endif

#endif /* __APPLE__ */

#include "Pyro.h"

namespace PyroParticles
{

class CGraphics_OGL;

class CTexture_OGL : public PyroParticles::PyroGraphics::ITexture
{
	int m_glTexture;

public:

	CTexture_OGL(PyroParticles::PyroGraphics::CBitmap *pBitmap, PyroDWord Flags);

	int GetOGLTexture() const { return m_glTexture; }

};

class CIndexBuffer_OGL : public PyroParticles::PyroGraphics::IIndexBuffer
{
	PyroByte *m_pBuffer;

public:

	CIndexBuffer_OGL(CGraphics_OGL *pGraphics, PyroDWord IndexType, PyroDWord nIndexes, PyroDWord Flags)
	{
		m_pBuffer = NULL;
	}

	virtual PyroResult Lock() { return PyroOK; }

	virtual PyroResult Unlock() { return PyroOK; }

	virtual void *GetBuffer() { return m_pBuffer; }

	virtual PyroResult Restore() { return PyroOK; }

	virtual PyroResult Invalidate() { return PyroOK; }

};

class CVertexBuffer_OGL : public PyroParticles::PyroGraphics::IVertexBuffer
{
	PyroByte *m_pBuffer;
	PyroDWord m_VertexFormat;
	PyroDWord m_VertexSize;

public:

	CVertexBuffer_OGL(CGraphics_OGL *pGraphics, PyroDWord nVertices, PyroDWord VertexSize, PyroDWord VertexFormat, PyroDWord Flags)
	{
		m_VertexSize = VertexSize;
		m_VertexFormat = VertexFormat;

		m_pBuffer = new PyroByte[nVertices * VertexSize];
	}

	~CVertexBuffer_OGL()
	{
		delete m_pBuffer;
	}

	PyroDWord GetVertexSize() const { return m_VertexSize; }
	PyroDWord GetVertexFormat() const { return m_VertexFormat; }

	virtual PyroResult Lock() { return PyroOK; }

	virtual PyroResult Unlock() { return PyroOK; }

	virtual void *GetBuffer() {	return m_pBuffer; }

	virtual PyroResult Restore() { return PyroOK; }

	virtual PyroResult Invalidate() { return PyroOK; }

};

class CGraphics_OGL : public PyroParticles::PyroGraphics::IDevice
{
	int m_glSrcBlendFunc;
	int m_glDestBlendFunc;

	bool m_bNullTexture[8];

public:

	virtual PyroResult SetBlendFunc(
		PyroParticles::PyroGraphics::IDevice::EBlend SrcBlend,
		PyroParticles::PyroGraphics::IDevice::EBlend DestBlend);

	virtual PyroResult SetRenderState(PyroDWord State, PyroDWord Value);

	virtual PyroResult SetWorldMatrix(float WorldMatrix[3][4]);

	virtual PyroResult SetTexture(PyroDWord Stage, PyroParticles::PyroGraphics::ITexture *pTexture);

	virtual PyroResult CreateVertexBuffer(
		PyroParticles::PyroGraphics::IVertexBuffer **ppVertexBuffer,
		PyroDWord Vertices,
		PyroDWord VertexSize,
		PyroDWord VertexFormat,
		PyroDWord Flags);

	virtual PyroResult CreateIndexBuffer(
		PyroParticles::PyroGraphics::IIndexBuffer **ppIndexBuffer,
		PyroDWord IndexType,
		PyroDWord nIndexes,
		PyroDWord Flags);

	virtual PyroResult CreateTexture(
		PyroParticles::PyroGraphics::ITexture **ppTexture,
		PyroParticles::PyroGraphics::CBitmap *pBitmap,
		PyroDWord Flags);
    
	virtual PyroResult CreateBitmap(
		PyroParticles::PyroGraphics::CBitmap **ppBitmap,
		PyroByte *pBuffer,
		PyroDWord Width,
		PyroDWord Height,
		PyroDWord Flags);

public:

	CGraphics_OGL()
	{
		m_glSrcBlendFunc = GL_ONE;
		m_glDestBlendFunc = GL_ONE;
	}

	virtual PyroResult RenderQuads(
		PyroParticles::PyroGraphics::IVertexBuffer *_pVertexBuffer,
		PyroDWord nFirst,
		PyroDWord nQuads,
		PyroDWord Flags);

	virtual PyroBool IsRGBA() const { return PyroTrue; }

};

}

#endif /* _PYRO_SAMPLES_COMMON_OPENGL_H_INCLUDED */
