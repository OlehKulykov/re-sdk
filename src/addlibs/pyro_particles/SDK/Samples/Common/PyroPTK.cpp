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

#include "PyroPTK.h"

PyroResult PyroParticles::CGraphics_PTK::RenderQuads(
	PyroParticles::PyroGraphics::IVertexBuffer *_pVertexBuffer,
	PyroDWord nFirst,
	PyroDWord nQuads,
	PyroDWord Flags)
{
	if (!m_pCurrentTexture)
		return PyroOK;

	CVertexBuffer_PTK *pVertexBuffer = (CVertexBuffer_PTK *) _pVertexBuffer;

	PyroByte *pSrc = (PyroByte *) pVertexBuffer->GetBuffer();
	PyroDWord VertexSize = pVertexBuffer->GetVertexSize();

	pSrc += nFirst * VertexSize * 4;

	float ColorMult = 1.0f / 255.0f;

	KGraphic *pPTKTexture = m_pCurrentTexture->GetPTKTexture();

	float TextureWidth = pPTKTexture->getWidth();
	float TextureHeight = pPTKTexture->getHeight();

	pPTKTexture->setAlphaMode(m_AlphaMode);

	for (PyroDWord i = 0; i < nQuads; i ++)
	{
		CPTKPyroVertex *pVertex0 = (CPTKPyroVertex *) (pSrc);
		CPTKPyroVertex *pVertex1 = (CPTKPyroVertex *) (pSrc + VertexSize);
		CPTKPyroVertex *pVertex2 = (CPTKPyroVertex *) (pSrc + VertexSize * 2);
		CPTKPyroVertex *pVertex3 = (CPTKPyroVertex *) (pSrc + VertexSize * 3);

		PyroDWord Color = pVertex0->color;

		pPTKTexture->setBlitColor(
			((Color >> 16) & 0xff) * ColorMult,
			((Color >>  8) & 0xff) * ColorMult,
			((Color      ) & 0xff) * ColorMult,
			((Color >> 24) & 0xff) * ColorMult
		);

		pPTKTexture->blitArbitraryQuad(
			pVertex0->tx * TextureWidth, pVertex0->ty * TextureHeight,
			pVertex1->tx * TextureWidth, pVertex1->ty * TextureHeight,
			pVertex2->tx * TextureWidth, pVertex2->ty * TextureHeight,
			pVertex3->tx * TextureWidth, pVertex3->ty * TextureHeight,
			pVertex0->x, pVertex0->y,
			pVertex1->x, pVertex1->y,
			pVertex2->x, pVertex2->y,
			pVertex3->x, pVertex3->y
		);

		pSrc += VertexSize * 4;
	}

	return PyroOK;
}
