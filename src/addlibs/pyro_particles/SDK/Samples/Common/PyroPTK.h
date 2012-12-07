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

#ifndef _PYRO_SAMPLES_COMMON_PTK_H_INCLUDED
#define _PYRO_SAMPLES_COMMON_PTK_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

#include "Pyro.h"
#include "KPTK.h"

namespace PyroParticles
{

#pragma pack(1)
struct CPTKPyroVertex
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

class CGraphics_PTK;

class CTexture_PTK : public PyroParticles::PyroGraphics::ITexture
{
	CGraphics_PTK *m_pGraphics;
	KGraphic *m_pPTKTexture;

public:

	CTexture_PTK(CGraphics_PTK *pGraphics, PyroParticles::PyroGraphics::CBitmap *pBitmap, PyroDWord Flags, KGraphic *pPTKTexture)
	{
		m_pGraphics = pGraphics;
		m_pPTKTexture = pPTKTexture;
	}

	~CTexture_PTK()
	{
		delete m_pPTKTexture;
	}

	KGraphic *GetPTKTexture() const { return m_pPTKTexture; }

};

class CVertexBuffer_PTK : public PyroParticles::PyroGraphics::IVertexBuffer
{
	PyroByte *m_pVertices;
	PyroDWord m_VertexFormat;
	PyroDWord m_VertexSize;

public:

	CVertexBuffer_PTK(PyroDWord nVertices, PyroDWord VertexSize, PyroDWord VertexFormat, PyroDWord Flags)
	{
		m_pVertices = new PyroByte[nVertices * VertexSize];
		m_VertexSize = VertexSize;
		m_VertexFormat = VertexFormat;
	}

	~CVertexBuffer_PTK()
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

class CGraphics_PTK : public PyroParticles::PyroGraphics::IDevice
{
	CTexture_PTK *m_pCurrentTexture;
	KWindow *m_pPTKWindow;
	long m_AlphaMode;

public:

	virtual PyroResult SetBlendFunc(
		PyroParticles::PyroGraphics::IDevice::EBlend SrcBlend,
		PyroParticles::PyroGraphics::IDevice::EBlend DestBlend)
	{
		if (SrcBlend == PyroParticles::PyroGraphics::IDevice::BLEND_SRC_ALPHA &&
			DestBlend == PyroParticles::PyroGraphics::IDevice::BLEND_ONE)
		{
			m_AlphaMode = 4;
		}
		else
		{
			m_AlphaMode = 1;
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
		CTexture_PTK *pTexture = (CTexture_PTK *) _pTexture;

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
		*ppVertexBuffer = new CVertexBuffer_PTK(Vertices, VertexSize, VertexFormat, Flags);

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
		KGraphic *pPTKTexture = KPTK::createKGraphic();

		pPTKTexture->makePictureFromArray(
			(unsigned long *) pBitmap->GetBuffer(),
			pBitmap->GetWidth(), pBitmap->GetHeight(), true);

		*ppTexture = new CTexture_PTK(this, pBitmap, Flags, pPTKTexture);

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

	CGraphics_PTK(KWindow *pWindow)
	{
		m_pPTKWindow = pWindow;
		m_pCurrentTexture = NULL;
		m_AlphaMode = 0;
	}

	~CGraphics_PTK()
	{
	}

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


#endif /* _PYRO_SAMPLES_COMMON_PTK_H_INCLUDED */
