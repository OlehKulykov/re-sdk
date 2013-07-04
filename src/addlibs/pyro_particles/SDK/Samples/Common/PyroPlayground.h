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

#ifndef _PYRO_SAMPLES_COMMON_PLAYGROUND_H_INCLUDED
#define _PYRO_SAMPLES_COMMON_PLAYGROUND_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

#include "Pyro.h"
#include <pf/pflib.h>

namespace PyroParticles
{

class CGraphics_Playground;

class CTexture_Playground : public PyroParticles::PyroGraphics::ITexture
{
	CGraphics_Playground *m_pGraphics;
	TTextureRef m_PlaygroundTexture;

public:

	CTexture_Playground(CGraphics_Playground *pGraphics, PyroParticles::PyroGraphics::CBitmap *pBitmap, PyroDWord Flags, TTextureRef PlaygroundTexture)
	{
		m_pGraphics = pGraphics;
		m_PlaygroundTexture = PlaygroundTexture;
	}

	TTextureRef GetPlaygroundTexture() const { return m_PlaygroundTexture; }

};

class CVertexBuffer_Playground : public PyroParticles::PyroGraphics::IVertexBuffer
{
	void *m_pVertices;
	PyroDWord m_VertexFormat;
	PyroDWord m_VertexSize;

	TTransformedLitVert *m_pPlaygroundVertices;
	TVertexSet *m_pPlaygroundVertexSet;

public:

	CVertexBuffer_Playground(PyroDWord nVertices, PyroDWord VertexSize, PyroDWord VertexFormat, PyroDWord Flags);

	~CVertexBuffer_Playground();

	void Convert(PyroDWord nFirst, PyroDWord nVertices);

	TVertexSet *GetPlaygroundVertexSet() const { return m_pPlaygroundVertexSet; }

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



class CGraphics_Playground : public PyroParticles::PyroGraphics::IDevice
{

public:

	virtual PyroResult SetBlendFunc(
		PyroParticles::PyroGraphics::IDevice::EBlend SrcBlend,
		PyroParticles::PyroGraphics::IDevice::EBlend DestBlend)
	{
		TEffect::EBlendMode BlendMode = TEffect::kBlendNormal;

		if (SrcBlend == PyroParticles::PyroGraphics::IDevice::BLEND_SRC_ALPHA &&
			DestBlend == PyroParticles::PyroGraphics::IDevice::BLEND_ONE)
		{
			BlendMode = TEffect::kBlendAdditiveAlpha;
		}

		m_pRenderer->GetEffect()->SetBlendMode(BlendMode);

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
		CTexture_Playground *pTexture = (CTexture_Playground *) _pTexture;
		
		m_pRenderer->GetDefault2dEffect()->SetTexture(pTexture ? pTexture->GetPlaygroundTexture() : TTextureRef());

		return PyroOK;
	}

	virtual PyroResult CreateVertexBuffer(
		PyroParticles::PyroGraphics::IVertexBuffer **ppVertexBuffer,
		PyroDWord Vertices,
		PyroDWord VertexSize,
		PyroDWord VertexFormat,
		PyroDWord Flags)
	{
		*ppVertexBuffer = new CVertexBuffer_Playground(Vertices, VertexSize, VertexFormat, Flags);

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
		PyroDWord Flags);

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

	TRenderer *m_pRenderer;
	uint16_t *m_pQuadIndexList;

	void CreateQuadIndexList(uint16_t nQuads);

public:

	CGraphics_Playground(TRenderer *pRenderer, int nMaxQuads)
	{
		m_pRenderer = pRenderer;

		CreateQuadIndexList(nMaxQuads);
	}

	~CGraphics_Playground()
	{
		if (m_pQuadIndexList)
			free(m_pQuadIndexList);
	}

	TRenderer *GetPlaygroundRenderer() const { return m_pRenderer; }

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

#endif /* _PYRO_SAMPLES_COMMON_PLAYGROUND_H_INCLUDED */
