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

#ifndef _PYRO_SAMPLES_COMMON_D3D9_H_INCLUDED
#define _PYRO_SAMPLES_COMMON_D3D9_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

#include <d3d9.h>
#include <d3dx9.h>
#include <assert.h>
#include "Pyro.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(obj) { if (obj) { obj->Release(); obj = NULL; } }
#endif

namespace PyroParticles
{

class CGraphicsDevice_D3D9;

class CIndexBuffer_D3D9 : public PyroParticles::PyroGraphics::IIndexBuffer
{
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	void *m_pBuffer;

	CIndexBuffer_D3D9(LPDIRECT3DINDEXBUFFER9 pIB)
	{
		m_pIB = pIB;
	}

public:

	static PyroResult Create(
		PyroParticles::PyroGraphics::IIndexBuffer **ppIB,
		CGraphicsDevice_D3D9 *pGraphics,
		PyroDWord IndexType,
		PyroDWord nIndexes,
		PyroDWord Flags);

	virtual ~CIndexBuffer_D3D9()
	{
		SAFE_RELEASE(m_pIB);
	}

	LPDIRECT3DINDEXBUFFER9 GetD3D9IndexBuffer() const { return m_pIB; }

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

class CVertexBuffer_D3D9 : public PyroParticles::PyroGraphics::IVertexBuffer2
{
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;
	PyroDWord m_VertexFormat;
	PyroDWord m_VertexSize;

	static DWORD GetD3DFVF(DWORD VertexFormat);

	CVertexBuffer_D3D9(LPDIRECT3DVERTEXBUFFER9 pVB, LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration)
	{
		m_pVB = pVB;
		m_pVertexDeclaration = pVertexDeclaration;
	}

public:

	~CVertexBuffer_D3D9()
	{
		SAFE_RELEASE(m_pVB);
	}

	static PyroResult Create(
		PyroParticles::PyroGraphics::IVertexBuffer2 **ppVB,
		CGraphicsDevice_D3D9 *pGraphics,
		PyroDWord nVertices,
		PyroDWord VertexSize,
		PyroDWord VertexFormat,
		PyroDWord Flags,
		BOOL b3D);

	PyroDWord GetVertexSize() const { return m_VertexSize; }
	PyroDWord GetVertexFormat() const { return m_VertexFormat; }
	PyroDWord GetD3DVertexFormat() const { return GetD3DFVF(m_VertexFormat); }

	LPDIRECT3DVERTEXBUFFER9 GetD3D9VertexBuffer() const { return m_pVB; }

	LPDIRECT3DVERTEXDECLARATION9 GetD3D9VertexDeclaration() const { return m_pVertexDeclaration; }

	virtual PyroResult CopyBufferSubData(void *pSrc, PyroDWord Offset, PyroDWord Size, PyroBool bDiscard);

	virtual PyroResult Invalidate() { return S_OK; }

	virtual PyroResult Restore() { return S_OK; }

};

class CTexture_D3D9 : public PyroParticles::PyroGraphics::ITexture
{
	LPDIRECT3DTEXTURE9 m_pTexture;

	CTexture_D3D9(LPDIRECT3DTEXTURE9 pTexture)
	{
		m_pTexture = pTexture;
	}

public:

	~CTexture_D3D9();

	static PyroResult Create(
		PyroParticles::PyroGraphics::ITexture **ppTexture,
		CGraphicsDevice_D3D9 *pGraphics,
		PyroParticles::PyroGraphics::CBitmap *pBitmap,
		PyroDWord Flags);

	virtual PyroResult Invalidate() { return S_OK; }

	virtual PyroResult Restore() { return S_OK; }

	LPDIRECT3DTEXTURE9 GetD3D9Texture() const { return m_pTexture; }

};

class CGraphicsDevice_D3D9 : public PyroParticles::PyroGraphics::IDevice
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
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	BOOL m_b3D;

public:

	CGraphicsDevice_D3D9(LPDIRECT3DDEVICE9 pd3dDevice, int nMaxQuads, BOOL b3D);

	virtual ~CGraphicsDevice_D3D9();

	virtual PyroResult RenderQuads2(
		PyroParticles::PyroGraphics::IVertexBuffer2 *pVertexBuffer,
		PyroDWord nFirstQuad,
		PyroDWord nQuads,
		PyroDWord Flags);

	virtual PyroBool IsRGBA() const { return PyroFalse; }

	LPDIRECT3DDEVICE9 GetD3D9Device() const { return m_pd3dDevice; }

	void CreateQuadIndexBuffer(DWORD nQuads);

};

}

#endif /* _PYRO_SAMPLES_COMMON_D3D9_H_INCLUDED */
