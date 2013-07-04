/*
* Copyright (c) 2002-2008 Fenomen Games.  All Rights Reserved.
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

#ifndef _PYRO_SAMPLES_COMMON_POPCAP_H_INCLUDED
#define _PYRO_SAMPLES_COMMON_POPCAP_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

#include "SexyAppFramework/SexyAppBase.h"
#include "SexyAppFramework/TriVertex.h"
#include "SexyAppFramework/Image.h"
#include "SexyAppFramework/DDImage.h"
#include "SexyAppFramework/Graphics.h"

#include "Pyro.h"

namespace PyroParticles
{

class CGraphics_PopCap;

class CTexture_PopCap : public PyroParticles::PyroGraphics::ITexture
{
	Sexy::DDImage *m_pImage;
	CGraphics_PopCap *m_pGraphics;

public:

	CTexture_PopCap(CGraphics_PopCap *pGraphics, PyroParticles::PyroGraphics::CBitmap *pBitmap, PyroDWord Flags);

	~CTexture_PopCap()
	{
		delete m_pImage;
	}

	Sexy::DDImage *GetSexyImage() const { return m_pImage; }

};

class CVertexBuffer_PopCap : public PyroParticles::PyroGraphics::IVertexBuffer
{
	void *m_pVertices;
	PyroDWord m_VertexFormat;
	PyroDWord m_VertexSize;

public:

	CVertexBuffer_PopCap(PyroDWord nVertices, PyroDWord VertexSize, PyroDWord VertexFormat, PyroDWord Flags)
	{
		m_pVertices = new PyroByte[nVertices * VertexSize];
		m_VertexSize = VertexSize;
		m_VertexFormat = VertexFormat;
	}

	~CVertexBuffer_PopCap()
	{
		delete m_pVertices;
	}

	inline PyroDWord GetVertexFormat() const { return m_VertexFormat; }
	inline PyroDWord GetVertexSize() const { return m_VertexSize; }

	PyroResult Lock()
	{
		return PyroOK;
	}

	PyroResult Unlock()
	{
		return PyroOK;
	}

	void *GetBuffer()
	{
		return m_pVertices;
	}

};

class CGraphics_PopCap : public PyroParticles::PyroGraphics::IDevice
{
	Sexy::SexyAppBase *m_pApp;
	Sexy::Graphics *m_pSexyGraphics;
	CTexture_PopCap *m_pCurrentTexture;
	int m_CurrentDrawMode;	

public:

	virtual PyroResult SetBlendFunc(
		PyroParticles::PyroGraphics::IDevice::EBlend SrcBlend,
		PyroParticles::PyroGraphics::IDevice::EBlend DestBlend)
	{
		if (SrcBlend == PyroParticles::PyroGraphics::IDevice::BLEND_SRC_ALPHA &&
			DestBlend == PyroParticles::PyroGraphics::IDevice::BLEND_ONE)
		{
			m_CurrentDrawMode = Sexy::Graphics::DRAWMODE_ADDITIVE;
		}
		else
		{
			m_CurrentDrawMode = Sexy::Graphics::DRAWMODE_NORMAL;
		}

		return PyroOK;
	}

	virtual PyroResult SetRenderState(PyroDWord State, PyroDWord Value)
	{
		return PyroOK;
	}

	virtual PyroResult SetWorldMatrix(float WorldMatrix[3][4])
	{
		return PyroOK;
	}

	virtual PyroResult SetTexture(PyroDWord Stage, PyroParticles::PyroGraphics::ITexture *pTexture)
	{
		m_pCurrentTexture = (CTexture_PopCap *) pTexture;
		return PyroOK;
	}

	virtual PyroResult CreateVertexBuffer(
		PyroParticles::PyroGraphics::IVertexBuffer **ppVertexBuffer,
		PyroDWord Vertices,
		PyroDWord VertexSize,
		PyroDWord VertexFormat,
		PyroDWord Flags)
	{
		*ppVertexBuffer = new CVertexBuffer_PopCap(Vertices, VertexSize, VertexFormat, Flags);

		return PyroOK;
	}

	virtual PyroResult CreateIndexBuffer(
		PyroParticles::PyroGraphics::IIndexBuffer **ppIndexBuffer,
		PyroDWord IndexType,
		PyroDWord nIndexes,
		PyroDWord Flags)
	{
		/* not supported */
		return ~0;
	}

	virtual PyroResult CreateTexture(
		PyroParticles::PyroGraphics::ITexture **ppTexture,
		PyroParticles::PyroGraphics::CBitmap *pBitmap,
		PyroDWord Flags)
	{
		*ppTexture = new CTexture_PopCap(this, pBitmap, Flags);

		return PyroOK;
	}

	virtual PyroResult CreateBitmap(
		PyroParticles::PyroGraphics::CBitmap **ppBitmap,
		PyroByte *pBuffer,
		PyroDWord Width,
		PyroDWord Height,
		PyroDWord Flags)
	{
		*ppBitmap = new PyroParticles::PyroGraphics::CBitmap(pBuffer, Width, Height, Flags);

		return PyroOK;
	}

public:

	CGraphics_PopCap(Sexy::SexyAppBase *pApp)
	{
		m_pApp = pApp;
		m_CurrentDrawMode = Sexy::Graphics::DRAWMODE_NORMAL;
		m_pCurrentTexture = NULL;
		m_pSexyGraphics = NULL;
	}

	PyroResult RenderQuads(
		PyroParticles::PyroGraphics::IVertexBuffer *_pVertexBuffer,
		PyroDWord nFirst,
		PyroDWord nQuads,
		PyroDWord Flags);

	void SetSexyGraphics(Sexy::Graphics *pSexyGraphics) { m_pSexyGraphics = pSexyGraphics; }
	Sexy::Graphics *GetSexyGraphics() const { return m_pSexyGraphics; }

	Sexy::SexyAppBase *GetSexyApp() const { return m_pApp; }

	PyroBool IsRGBA() const
	{
		return PyroFalse;
	}

};

}


#endif /* _PYRO_SAMPLES_COMMON_POPCAP_H_INCLUDED */
