
#include "PyroPopCap.h"

PyroParticles::CTexture_PopCap::CTexture_PopCap( CGraphics_PopCap *pGraphics, PyroParticles::PyroGraphics::CBitmap *pBitmap, PyroDWord Flags )
{
	m_pGraphics = pGraphics;

	m_pImage = new Sexy::DDImage(pGraphics->GetSexyApp()->mDDInterface);
	m_pImage->Create(pBitmap->GetWidth(), pBitmap->GetHeight());

	{
		PyroByte *pSrc = (PyroByte *) pBitmap->GetBuffer();
		PyroByte *pDest = (PyroByte *) m_pImage->GetBits();
		PyroDWord Count = pBitmap->GetWidth() * pBitmap->GetHeight();
        
		while (Count --)
		{
			pDest[0] = pSrc[2];
			pDest[1] = pSrc[1];
			pDest[2] = pSrc[0];
			pDest[3] = pSrc[3];

			pSrc += 4, pDest += 4;

		}
	}

	m_pImage->CommitBits();
}

PyroResult PyroParticles::CGraphics_PopCap::RenderQuads(
	PyroParticles::PyroGraphics::IVertexBuffer *_pVertexBuffer,
	PyroDWord nFirst,
	PyroDWord nQuads,
	PyroDWord Flags)
{
	CVertexBuffer_PopCap *pVertexBuffer = (CVertexBuffer_PopCap *) _pVertexBuffer;

	static Sexy::TriVertex VertexList[200][3];

	PyroDWord VertexSize = pVertexBuffer->GetVertexSize();

	PyroByte *pInVertex = (PyroByte *) pVertexBuffer->GetBuffer();

	pInVertex += nFirst * VertexSize * 4;

	int XYZOffset = 0;
	int UVOffset = sizeof(float) * 5;
	int DiffuseOffset = sizeof(float) * 4;
	int BufferedQuads = 0;

	Sexy::TriVertex *pOutVertex = &VertexList[0][0];

	for (DWORD i = 0; i < nQuads; i ++)
	{
		for (int j = 0; j < 4; j ++)
		{
			pOutVertex->x = *((float *)(pInVertex + XYZOffset));
			pOutVertex->y = *((float *)(pInVertex + XYZOffset + sizeof(float)));
			pOutVertex->u = *((float *)(pInVertex + UVOffset));
			pOutVertex->v = *((float *)(pInVertex + UVOffset + sizeof(float)));
			pOutVertex->color = *((DWORD *)(pInVertex + DiffuseOffset));

			pInVertex += VertexSize;
			pOutVertex ++;
		}

		pOutVertex[0] = pOutVertex[-4];
		pOutVertex[1] = pOutVertex[-2];
		pOutVertex += 2;
		BufferedQuads ++;

		if (BufferedQuads >= 100 || i == nQuads - 1)
		{
			m_pSexyGraphics->SetDrawMode(m_CurrentDrawMode);
			m_pSexyGraphics->DrawTrianglesTex(m_pCurrentTexture->GetSexyImage(), VertexList, BufferedQuads * 2);
			pOutVertex = &VertexList[0][0];
			BufferedQuads = 0;
		}
	}

	return PyroOK;
}
