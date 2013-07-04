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

#ifndef _PYRO_SAMPLES_COMMON_HGE_H_INCLUDED
#define _PYRO_SAMPLES_COMMON_HGE_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

#include "Pyro.h"
#include "HGE.h"

namespace PyroParticles
{

#pragma pack(1)
struct CHGEPyroVertex
{
	float x;
	float y;
	float z;
	float w;
	PyroDWord color;
	float tx;
	float ty;
};
#pragma pack()

class CGraphics_HGE;

class CTexture_HGE : public PyroParticles::PyroGraphics::ITexture
{
	CGraphics_HGE *m_pGraphics;
	HTEXTURE m_hTexture;

public:

	CTexture_HGE(CGraphics_HGE *pGraphics, PyroParticles::PyroGraphics::CBitmap *pBitmap, PyroDWord Flags, HTEXTURE hTexture)
	{
		m_pGraphics = pGraphics;
		m_hTexture = hTexture;
	}

	~CTexture_HGE();

	HTEXTURE GetHGETexture() const { return m_hTexture; }

};

class CVertexBuffer_HGE : public PyroParticles::PyroGraphics::IVertexBuffer
{
	PyroByte *m_pVertices;
	PyroDWord m_VertexFormat;
	PyroDWord m_VertexSize;

public:

	CVertexBuffer_HGE(PyroDWord nVertices, PyroDWord VertexSize, PyroDWord VertexFormat, PyroDWord Flags)
	{
		m_pVertices = new PyroByte[nVertices * VertexSize];
		m_VertexSize = VertexSize;
		m_VertexFormat = VertexFormat;
	}

	~CVertexBuffer_HGE()
	{
		delete m_pVertices;
	}

	PyroDWord GetVertexSize() const { return m_VertexSize; }

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

class CGraphics_HGE : public PyroParticles::PyroGraphics::IDevice
{
	CTexture_HGE *m_pCurrentTexture;
	HGE *m_pHGE;
	int m_BlendMode;

public:

	virtual PyroResult SetBlendFunc(
		PyroParticles::PyroGraphics::IDevice::EBlend SrcBlend,
		PyroParticles::PyroGraphics::IDevice::EBlend DestBlend)
	{
		if (SrcBlend == PyroParticles::PyroGraphics::IDevice::BLEND_SRC_ALPHA &&
			DestBlend == PyroParticles::PyroGraphics::IDevice::BLEND_ONE)
		{
			m_BlendMode = BLEND_ALPHAADD;
		}
		else
		{
			m_BlendMode = BLEND_ALPHABLEND;
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

	virtual PyroResult SetTexture(PyroDWord Stage, PyroParticles::PyroGraphics::ITexture *_pTexture)
	{
		CTexture_HGE *pTexture = (CTexture_HGE *) _pTexture;

		m_pCurrentTexture = pTexture;

		return PyroOK;
	}

	virtual PyroResult CreateVertexBuffer(
		PyroParticles::PyroGraphics::IVertexBuffer **ppVertexBuffer,
		PyroDWord Vertices,
		PyroDWord VertexSize,
		PyroDWord VertexFormat,
		PyroDWord Flags)
	{
		*ppVertexBuffer = new CVertexBuffer_HGE(Vertices, VertexSize, VertexFormat, Flags);

		return PyroOK;
	}

	virtual PyroResult CreateIndexBuffer(
		PyroParticles::PyroGraphics::IIndexBuffer **ppIndexBuffer,
		PyroDWord IndexType,
		PyroDWord nIndexes,
		PyroDWord Flags)
	{
		/* not supported */
		return PyroFailed;
	}

	virtual PyroResult CreateTexture(
		PyroParticles::PyroGraphics::ITexture **ppTexture,
		PyroParticles::PyroGraphics::CBitmap *pBitmap,
		PyroDWord Flags)
	{
		HTEXTURE hTexture = m_pHGE->Texture_Create(pBitmap->GetWidth(), pBitmap->GetHeight());

		PyroByte *pDest = (PyroByte *) m_pHGE->Texture_Lock(hTexture, false);
		PyroByte *pSrc = (PyroByte *) pBitmap->GetBuffer();
		PyroDWord Count = pBitmap->GetWidth() * pBitmap->GetHeight();

		while (Count --)
		{
			pDest[0] = pSrc[2];
			pDest[1] = pSrc[1];
			pDest[2] = pSrc[0];
			pDest[3] = pSrc[3];

			pSrc += 4, pDest += 4;
		}

		m_pHGE->Texture_Unlock(hTexture);

		*ppTexture = new CTexture_HGE(this, pBitmap, Flags, hTexture);

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

	CGraphics_HGE(HGE *pHGE)
	{
		m_pHGE = pHGE;
		m_pCurrentTexture = NULL;
		m_BlendMode = 0;
	}

	~CGraphics_HGE()
	{
	}

	HGE *GetHGEGraphics() const { return m_pHGE; }

	PyroResult RenderQuads(
		PyroParticles::PyroGraphics::IVertexBuffer *_pVertexBuffer,
		PyroDWord nFirst,
		PyroDWord nQuads,
		PyroDWord Flags);

	PyroBool IsRGBA() const
	{
		return PyroFalse;
	}

};

}

#endif /* _PYRO_SAMPLES_COMMON_HGE_H_INCLUDED */
