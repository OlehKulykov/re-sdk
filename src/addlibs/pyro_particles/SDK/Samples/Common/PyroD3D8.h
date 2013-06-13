/*
 * Copyright (c) 2002-2007 Fenomen Games.  All Rights Reserved.
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

#ifndef _PYRO_SAMPLES_COMMON_D3D8_H_INCLUDED
#define _PYRO_SAMPLES_COMMON_D3D8_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

#include <d3d8.h>
#include <d3dx8.h>
#include <assert.h>
#include "Pyro.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(obj) { if (obj) { obj->Release(); obj = NULL; } }
#endif

namespace PyroParticles
{

class CGraphicsDevice_D3D8;

class CIndexBuffer_D3D8 : public PyroParticles::PyroGraphics::IIndexBuffer
{
	LPDIRECT3DINDEXBUFFER8 m_pIB;
	void *m_pBuffer;

	CIndexBuffer_D3D8(LPDIRECT3DINDEXBUFFER8 pIB)
	{
		m_pIB = pIB;
	}

public:

	static PyroResult Create(
		PyroParticles::PyroGraphics::IIndexBuffer **ppIB,
		CGraphicsDevice_D3D8 *pGraphics,
		PyroDWord IndexType,
		PyroDWord nIndexes,
		PyroDWord Flags);

	virtual ~CIndexBuffer_D3D8()
	{
		SAFE_RELEASE(m_pIB);
	}

	LPDIRECT3DINDEXBUFFER8 GetD3D8IndexBuffer() const { return m_pIB; }

	virtual PyroResult Lock();

	virtual PyroResult Unlock();

	virtual PyroResult Invalidate() { return S_OK; }

	virtual PyroResult Restore() { return S_OK; }

	virtual void *GetBuffer()
	{
		assert(m_pIB != NULL);

		return m_pBuffer;
	}

};

class CVertexBuffer_D3D8 : public PyroParticles::PyroGraphics::IVertexBuffer2
{
	LPDIRECT3DVERTEXBUFFER8 m_pVB;
	PyroDWord m_VertexFormat;
	PyroDWord m_VertexSize;

	static DWORD GetD3DFVF(DWORD VertexFormat);

	CVertexBuffer_D3D8(LPDIRECT3DVERTEXBUFFER8 pVB)
	{
		m_pVB = pVB;
	}

public:

	~CVertexBuffer_D3D8()
	{
		SAFE_RELEASE(m_pVB);
	}

	static PyroResult Create(
		PyroParticles::PyroGraphics::IVertexBuffer2 **ppVB,
		CGraphicsDevice_D3D8 *pGraphics,
		PyroDWord nVertices,
		PyroDWord VertexSize,
		PyroDWord VertexFormat,
		PyroDWord Flags);

	PyroDWord GetVertexSize() const { return m_VertexSize; }
	PyroDWord GetVertexFormat() const { return m_VertexFormat; }
	PyroDWord GetD3DVertexFormat() const { return GetD3DFVF(m_VertexFormat); }

	LPDIRECT3DVERTEXBUFFER8 GetD3D8VertexBuffer() const { return m_pVB; }

	virtual PyroResult CopyBufferSubData(void *pSrc, PyroDWord Offset, PyroDWord Size, PyroBool bDiscard);

	virtual PyroResult Invalidate() { return PyroOK; }

	virtual PyroResult Restore() { return PyroOK; }

};

class CTexture_D3D8 : public PyroParticles::PyroGraphics::ITexture
{
	LPDIRECT3DTEXTURE8 m_pTexture;

	CTexture_D3D8(LPDIRECT3DTEXTURE8 pTexture)
	{
		m_pTexture = pTexture;
	}

public:

	~CTexture_D3D8();

	static PyroResult Create(
		PyroParticles::PyroGraphics::ITexture **ppTexture,
		CGraphicsDevice_D3D8 *pGraphics,
		PyroParticles::PyroGraphics::CBitmap *pBitmap,
		PyroDWord Flags);

	virtual PyroResult Invalidate() { return PyroOK; }

	virtual PyroResult Restore() { return PyroOK; }

	LPDIRECT3DTEXTURE8 GetD3D8Texture() const { return m_pTexture; }

};

class CGraphicsDevice_D3D8 : public PyroParticles::PyroGraphics::IDevice
{

public:

	virtual PyroResult SetBlendFunc(
		PyroParticles::PyroGraphics::IDevice::EBlend SrcBlend,
		PyroParticles::PyroGraphics::IDevice::EBlend DestBlend);

	virtual PyroResult SetRenderState(PyroDWord State, PyroDWord Value);

	virtual PyroResult SetWorldMatrix(float WorldMatrix[3][4]) { return S_OK; }

	virtual PyroResult SetTexture(PyroDWord Stage, PyroParticles::PyroGraphics::ITexture *pTexture);

	virtual PyroResult CreateVertexBuffer2(
		PyroParticles::PyroGraphics::IVertexBuffer2 **pVB,
		PyroDWord Vertices,
		PyroDWord VertexSize,
		PyroDWord VertexFormat,
		PyroDWord Flags);

	virtual PyroResult CreateIndexBuffer(
		PyroParticles::PyroGraphics::IIndexBuffer **pIB,
		PyroDWord IndexType,
		PyroDWord Indexes,
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

private:

	PyroParticles::PyroGraphics::IIndexBuffer *m_pQuadIndexBuffer;
	LPDIRECT3DDEVICE8 m_pd3dDevice;

public:

	CGraphicsDevice_D3D8(LPDIRECT3DDEVICE8 pd3dDevice, int nMaxQuads);

	virtual ~CGraphicsDevice_D3D8();

	virtual PyroResult RenderQuads2(
		PyroParticles::PyroGraphics::IVertexBuffer2 *pVertexBuffer,
		PyroDWord nFirstQuad,
		PyroDWord nQuads,
		PyroDWord Flags);

	virtual PyroBool IsRGBA() const { return PyroFalse; }

	LPDIRECT3DDEVICE8 GetD3D8Device() const { return m_pd3dDevice; }

	void CreateQuadIndexBuffer(DWORD nQuads);

};

}

#endif /* _PYRO_SAMPLES_COMMON_D3D8_H_INCLUDED */
