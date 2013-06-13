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

#include "PyroHGE.h"

PyroParticles::CTexture_HGE::~CTexture_HGE()
{
	m_pGraphics->GetHGEGraphics()->Texture_Free(m_hTexture);
}

PyroResult PyroParticles::CGraphics_HGE::RenderQuads(
	PyroParticles::PyroGraphics::IVertexBuffer *_pVertexBuffer,
	PyroDWord nFirst,
	PyroDWord nQuads,
	PyroDWord Flags)
{
	CVertexBuffer_HGE *pVertexBuffer = (CVertexBuffer_HGE *) _pVertexBuffer;
	
	PyroByte *pSrc = (PyroByte *) pVertexBuffer->GetBuffer();
	PyroDWord VertexSize = pVertexBuffer->GetVertexSize();

	pSrc += nFirst * VertexSize * 4;

	int MaxPrim = 0;
	hgeVertex *pDest = NULL;
	int nBatchedQuads = 1;

	HTEXTURE hTexture = NULL;

	if (m_pCurrentTexture)
		hTexture = m_pCurrentTexture->GetHGETexture();

	for (PyroDWord i = 0; i < nQuads; i ++)
	{
		if (nBatchedQuads >= MaxPrim)
		{
			if (i > 0)
				m_pHGE->Gfx_FinishBatch(nBatchedQuads);

			pDest = m_pHGE->Gfx_StartBatch(HGEPRIM_QUADS, hTexture, m_BlendMode, &MaxPrim);
			nBatchedQuads = 0;
		}

		for (int j = 0; j < 4; j ++)
		{
			CHGEPyroVertex *pVertex = (CHGEPyroVertex *) pSrc;

			pDest->x = pVertex->x;
			pDest->y = pVertex->y;
			pDest->z = pVertex->z;
			pDest->col = pVertex->color;
			pDest->tx = pVertex->tx;
			pDest->ty = pVertex->ty;

			pSrc += VertexSize;
			pDest ++;
		}

		nBatchedQuads ++;
	}

	if (nBatchedQuads)
		m_pHGE->Gfx_FinishBatch(nBatchedQuads);

	return PyroOK;
}
