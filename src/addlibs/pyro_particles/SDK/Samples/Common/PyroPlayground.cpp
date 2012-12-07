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

#include "PyroPlayground.h"

void PyroParticles::CVertexBuffer_Playground::Convert(PyroDWord nFirst, PyroDWord nVertices)
{
	int XYZOffset = 0;
	int UVOffset = sizeof(float) * 5;
	int DiffuseOffset = sizeof(float) * 4;

	TTransformedLitVert *pOutVertex = m_pPlaygroundVertices;

	PyroByte *pInVertex = (PyroByte *) m_pVertices;
	pInVertex += nFirst * m_VertexSize;

	for (PyroDWord i = 0; i < nVertices; i ++)
	{
		pOutVertex->pos.x = *((float *)(pInVertex + XYZOffset));
		pOutVertex->pos.y = *((float *)(pInVertex + XYZOffset + sizeof(float)));
		pOutVertex->uv.x = *((float *)(pInVertex + UVOffset));
		pOutVertex->uv.y = *((float *)(pInVertex + UVOffset + sizeof(float)));
		pOutVertex->color = *((uint32_t *)(pInVertex + DiffuseOffset));

		pInVertex += m_VertexSize;
		pOutVertex ++;
	}
}

PyroParticles::CVertexBuffer_Playground::CVertexBuffer_Playground(PyroDWord nVertices, PyroDWord VertexSize, PyroDWord VertexFormat, PyroDWord Flags)
{
	m_pPlaygroundVertices = new TTransformedLitVert[nVertices];
	m_pPlaygroundVertexSet = new TVertexSet(m_pPlaygroundVertices, nVertices);

	m_pVertices = new PyroByte[nVertices * VertexSize];
	m_VertexSize = VertexSize;
	m_VertexFormat = VertexFormat;
}

PyroParticles::CVertexBuffer_Playground::~CVertexBuffer_Playground()
{
	delete m_pPlaygroundVertexSet;
	delete[] m_pPlaygroundVertices;
	delete[] m_pVertices;
}

PyroResult PyroParticles::CGraphics_Playground::CreateTexture(
	PyroParticles::PyroGraphics::ITexture **ppTexture,
	PyroParticles::PyroGraphics::CBitmap *pBitmap,
	PyroDWord Flags)
{
	TTextureRef PlaygroundTexture = TTexture::Create(pBitmap->GetWidth(), pBitmap->GetHeight(), true);

	TColor32 *pData;
	uint32_t PixelPitch;
	if (!PlaygroundTexture->Lock(&pData, &PixelPitch, TTexture::kLockWriteOnly))
		return PyroFailed;

	PyroByte *pSrc = (PyroByte *) pBitmap->GetBuffer();
	TColor32 *pDest = pData;

	for (PyroDWord y = 0; y < pBitmap->GetHeight(); y ++)
	{
		for (PyroDWord x = 0; x < pBitmap->GetWidth(); x ++)
		{
			pDest->SetRed(pSrc[0]);
			pDest->SetGreen(pSrc[1]);
			pDest->SetBlue(pSrc[2]);
			pDest->SetAlpha(pSrc[3]);

			pSrc += 4, pDest ++; 
		}

		pDest -= pBitmap->GetWidth();
		pDest += PixelPitch;
	}

	PlaygroundTexture->Unlock();

	*ppTexture = new CTexture_Playground(this, pBitmap, Flags, PlaygroundTexture);

	return PyroOK;
}

void PyroParticles::CGraphics_Playground::CreateQuadIndexList(uint16_t nQuads)
{
	m_pQuadIndexList = (uint16_t *) malloc(sizeof(uint16_t) * (nQuads * 6));

	uint16_t *pDest = m_pQuadIndexList;
	uint16_t nVertex = 0;

	for (int i = 0; i < nQuads; i ++)
	{
		pDest[0] = nVertex + 0;
		pDest[1] = nVertex + 2;
		pDest[2] = nVertex + 3;

		pDest[3] = nVertex + 0;
		pDest[4] = nVertex + 1;
		pDest[5] = nVertex + 2;

		pDest += 6;
		nVertex += 4;
	}
}

PyroResult PyroParticles::CGraphics_Playground::RenderQuads(
	PyroParticles::PyroGraphics::IVertexBuffer *_pVertexBuffer,
	PyroDWord nFirst,
	PyroDWord nQuads,
	PyroDWord Flags)
{
	CVertexBuffer_Playground *pVertexBuffer = (CVertexBuffer_Playground *) _pVertexBuffer;

	pVertexBuffer->Convert(nFirst * 4, nQuads * 4);

	TVertexSet *pVertexSet = pVertexBuffer->GetPlaygroundVertexSet();
	pVertexSet->SetCount(nQuads * 4);

	m_pRenderer->DrawIndexedVertices(
		TRenderer::kDrawTriangles, 
		*pVertexSet,
		m_pQuadIndexList,
		nQuads * 6
	);

	return PyroOK;
}
