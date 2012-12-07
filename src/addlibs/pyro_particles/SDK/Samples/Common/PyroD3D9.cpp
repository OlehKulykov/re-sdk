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

#include "Pyro.h"
#include "PyroD3D9.h"

PyroParticles::CGraphicsDevice_D3D9::CGraphicsDevice_D3D9(LPDIRECT3DDEVICE9 pd3dDevice, int nMaxQuads, BOOL b3D)
{
	m_pd3dDevice = pd3dDevice;
	m_b3D = b3D;

	CreateQuadIndexBuffer(nMaxQuads);
}

PyroParticles::CGraphicsDevice_D3D9::~CGraphicsDevice_D3D9()
{
	delete m_pQuadIndexBuffer;
}

PyroResult PyroParticles::CIndexBuffer_D3D9::Create(
	PyroParticles::PyroGraphics::IIndexBuffer **ppIB,
	CGraphicsDevice_D3D9 *pGraphics,
	PyroDWord ItemType,
	PyroDWord nItems, 
	PyroDWord Flags)
{
	*ppIB = NULL;

	LPDIRECT3DINDEXBUFFER9 pd3dIB = NULL;

	HRESULT hr;

	if (FAILED(hr = pGraphics->GetD3D9Device()->CreateIndexBuffer(
			nItems * 6 * sizeof(WORD),
			D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED,
			&pd3dIB, 0)))
		return hr;

	*ppIB = new CIndexBuffer_D3D9(pd3dIB);

	return PyroOK;
}

void PyroParticles::CGraphicsDevice_D3D9::CreateQuadIndexBuffer(DWORD nQuads)
{
	PyroParticles::PyroGraphics::IIndexBuffer *pIndexBuffer = NULL;

	HRESULT hr = CreateIndexBuffer(&pIndexBuffer, PyroParticles::PyroGraphics::IIndexBuffer::IF_WORD, nQuads * 6, 0);

	if (FAILED(hr))
		throw PyroParticles::PyroGraphics::CPyroGraphicsException(hr, "Can't create quad index buffer");

	pIndexBuffer->Lock();

	WORD *pDest = (WORD *) pIndexBuffer->GetBuffer();

	WORD nVertex = 0;

	for (DWORD i = 0; i < nQuads; i ++)
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

	pIndexBuffer->Unlock();

	m_pQuadIndexBuffer = pIndexBuffer;
}

PyroResult PyroParticles::CGraphicsDevice_D3D9::SetBlendFunc(
	PyroParticles::PyroGraphics::IDevice::EBlend SrcBlend,
	PyroParticles::PyroGraphics::IDevice::EBlend DestBlend)
{
	DWORD d3dSrcBlend;

	switch (SrcBlend)
	{
	case BLEND_ZERO:					d3dSrcBlend = D3DBLEND_ZERO; 			break;
	case BLEND_ONE:						d3dSrcBlend = D3DBLEND_ONE; 			break;
	case BLEND_DST_COLOR:				d3dSrcBlend = D3DBLEND_DESTCOLOR; 		break;
	case BLEND_ONE_MINUS_DST_COLOR:		d3dSrcBlend = D3DBLEND_INVDESTCOLOR; 	break;
	case BLEND_SRC_ALPHA:				d3dSrcBlend = D3DBLEND_SRCALPHA; 		break;
	case BLEND_ONE_MINUS_SRC_ALPHA:		d3dSrcBlend = D3DBLEND_INVSRCALPHA; 	break;
	case BLEND_DST_ALPHA:				d3dSrcBlend = D3DBLEND_DESTALPHA; 		break;
	case BLEND_ONE_MINUS_DST_ALPHA:		d3dSrcBlend = D3DBLEND_INVDESTALPHA; 	break;
	case BLEND_SRC_ALPHA_SATURATE:		d3dSrcBlend = D3DBLEND_SRCALPHASAT; 	break;
	default:							return S_OK;
	}

	DWORD d3dDestBlend;

	switch (DestBlend)
	{
	case BLEND_ZERO:					d3dDestBlend = D3DBLEND_ZERO; 			break;
	case BLEND_ONE:						d3dDestBlend = D3DBLEND_ONE; 			break;
	case BLEND_DST_COLOR:				d3dDestBlend = D3DBLEND_DESTCOLOR; 		break;
	case BLEND_ONE_MINUS_DST_COLOR:		d3dDestBlend = D3DBLEND_INVDESTCOLOR; 	break;
	case BLEND_SRC_ALPHA:				d3dDestBlend = D3DBLEND_SRCALPHA; 		break;
	case BLEND_ONE_MINUS_SRC_ALPHA:		d3dDestBlend = D3DBLEND_INVSRCALPHA; 	break;
	case BLEND_DST_ALPHA:				d3dDestBlend = D3DBLEND_DESTALPHA; 		break;
	case BLEND_ONE_MINUS_DST_ALPHA:		d3dDestBlend = D3DBLEND_INVDESTALPHA; 	break;
	case BLEND_SRC_ALPHA_SATURATE:		d3dDestBlend = D3DBLEND_SRCALPHASAT; 	break;
	default:							return S_OK;
	}

	HRESULT hResult = GetD3D9Device()->SetRenderState(D3DRS_SRCBLEND, d3dSrcBlend);
	if (FAILED(hResult))
		return hResult;

	return GetD3D9Device()->SetRenderState(D3DRS_DESTBLEND, d3dDestBlend);
}

PyroResult PyroParticles::CGraphicsDevice_D3D9::SetRenderState(PyroDWord State, PyroDWord Value)
{
	D3DRENDERSTATETYPE d3dRS;

	switch (State)
	{
	case RS_ALPHABLENDENABLE:		d3dRS = D3DRS_ALPHABLENDENABLE; break;
	default:						return S_OK;
	}

	return GetD3D9Device()->SetRenderState(d3dRS, Value);
}

PyroResult PyroParticles::CGraphicsDevice_D3D9::CreateVertexBuffer2(
	PyroParticles::PyroGraphics::IVertexBuffer2 **ppVB,
	PyroDWord Vertices,
	PyroDWord VertexSize,
	PyroDWord VertexFormat,
	PyroDWord Flags)
{
	return CVertexBuffer_D3D9::Create(ppVB, this, Vertices, VertexSize, VertexFormat, Flags, m_b3D);
}

PyroResult PyroParticles::CGraphicsDevice_D3D9::CreateIndexBuffer(
	PyroParticles::PyroGraphics::IIndexBuffer **ppIB,
	PyroDWord IndexType,
	PyroDWord nIndexes,
	PyroDWord Flags)
{
	return CIndexBuffer_D3D9::Create(ppIB, this, IndexType, nIndexes, Flags);
}

PyroResult PyroParticles::CGraphicsDevice_D3D9::CreateTexture(
	PyroParticles::PyroGraphics::ITexture **pTexture,
	PyroParticles::PyroGraphics::CBitmap *pBitmap,
	PyroDWord Flags)
{
	return CTexture_D3D9::Create(pTexture, this, pBitmap, Flags);
}

PyroResult PyroParticles::CGraphicsDevice_D3D9::CreateBitmap(
	PyroParticles::PyroGraphics::CBitmap **pBitmap,
	PyroByte *pBuffer,
	PyroDWord Width,
	PyroDWord Height,
	PyroDWord Flags)
{
	*pBitmap = new PyroParticles::PyroGraphics::CBitmap(pBuffer, Width, Height, Flags);
	return S_OK;
}

PyroResult PyroParticles::CGraphicsDevice_D3D9::RenderQuads2(
	PyroParticles::PyroGraphics::IVertexBuffer2 *pVertexBuffer,
	PyroDWord nFirstQuad,
	PyroDWord nQuads,
	PyroDWord Flags)
{
	if (nQuads == 0)
		return PyroOK;

	CIndexBuffer_D3D9 *pd3dIndexBuffer = (CIndexBuffer_D3D9 *) m_pQuadIndexBuffer;

	CVertexBuffer_D3D9 *pVertexBuffer_D3D9 = (CVertexBuffer_D3D9 *) pVertexBuffer;

	GetD3D9Device()->SetStreamSource(0, pVertexBuffer_D3D9->GetD3D9VertexBuffer(), 0, pVertexBuffer_D3D9->GetVertexSize());

	if (pVertexBuffer_D3D9->GetD3D9VertexDeclaration())
	{
		GetD3D9Device()->SetVertexDeclaration(pVertexBuffer_D3D9->GetD3D9VertexDeclaration());
	}
	else
	{
		GetD3D9Device()->SetFVF(pVertexBuffer_D3D9->GetD3DVertexFormat());
	}

	GetD3D9Device()->SetIndices(pd3dIndexBuffer->GetD3D9IndexBuffer());
	GetD3D9Device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, nFirstQuad * 4, 0, nQuads * 6, 0, nQuads * 2);

	return PyroOK;
}

PyroResult PyroParticles::CGraphicsDevice_D3D9::SetTexture(
	PyroDWord Stage,
	PyroParticles::PyroGraphics::ITexture *pTexture)
{
	CTexture_D3D9 *pTexture_D3D9 = (CTexture_D3D9 *) pTexture;
	return GetD3D9Device()->SetTexture(Stage, pTexture_D3D9 ? pTexture_D3D9->GetD3D9Texture() : NULL);
}

PyroResult PyroParticles::CTexture_D3D9::Create(
	PyroParticles::PyroGraphics::ITexture **ppTexture,
	CGraphicsDevice_D3D9 *pGraphics,
	PyroParticles::PyroGraphics::CBitmap *pBitmap,
	PyroDWord Flags)
{
	LPDIRECT3DTEXTURE9 pd3dTexture = NULL;

	HRESULT hr = pGraphics->GetD3D9Device()->CreateTexture(
		pBitmap->GetWidth(),
		pBitmap->GetHeight(), 1, 
		0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
		&pd3dTexture, 0);

	if (FAILED(hr))
		return hr;

	D3DLOCKED_RECT d3dlr;
	if (FAILED(hr = pd3dTexture->LockRect( 0, &d3dlr, 0, 0 )))
	{
		SAFE_RELEASE(pd3dTexture);
		return hr;
	}

	{
		BYTE *pSrc = (BYTE *) pBitmap->GetBuffer();
		BYTE *pDest = (BYTE *) d3dlr.pBits;
		DWORD Height = pBitmap->GetHeight();

		while (Height --)
		{
			DWORD Width = pBitmap->GetWidth();

			while (Width --)
			{
				pDest[0] = pSrc[2];
				pDest[1] = pSrc[1];
				pDest[2] = pSrc[0];
				pDest[3] = pSrc[3];

				pSrc += 4, pDest += 4;
			}

			pDest -= pBitmap->GetWidth() * 4;
			pDest += d3dlr.Pitch;
		}
	}

	pd3dTexture->UnlockRect(0);

	*ppTexture = new CTexture_D3D9(pd3dTexture);

	return hr;	
}

PyroParticles::CTexture_D3D9::~CTexture_D3D9()
{
	SAFE_RELEASE(m_pTexture);
}

PyroResult PyroParticles::CIndexBuffer_D3D9::Lock()
{
	return m_pIB->Lock(0, 0, (void **) &m_pBuffer, D3DLOCK_DISCARD);
}

PyroResult PyroParticles::CIndexBuffer_D3D9::Unlock()
{
	HRESULT hr;

	if (FAILED(hr = m_pIB->Unlock()))
		return hr;

	m_pBuffer = NULL;

	return hr;
}

DWORD PyroParticles::CVertexBuffer_D3D9::GetD3DFVF(DWORD VertexFormat)
{
	DWORD d3dFVF = 0;

	assert(VertexFormat & VF_POSITION);

	if (VertexFormat & VF_RHW)      d3dFVF |= D3DFVF_XYZRHW;
	else
	if (VertexFormat & VF_POSITION) d3dFVF |= D3DFVF_XYZ;

	if (VertexFormat & VF_DIFFUSE ) d3dFVF |= D3DFVF_DIFFUSE;
	if (VertexFormat & VF_TEXTURE0) d3dFVF |= D3DFVF_TEX1;
	if (VertexFormat & VF_TEXTURE1) d3dFVF |= D3DFVF_TEX2;
	if (VertexFormat & VF_TEXTURE2) d3dFVF |= D3DFVF_TEX3;
	if (VertexFormat & VF_TEXTURE3) d3dFVF |= D3DFVF_TEX4;

	return d3dFVF;
}

PyroResult PyroParticles::CVertexBuffer_D3D9::Create(
	PyroParticles::PyroGraphics::IVertexBuffer2 **ppVB,
	CGraphicsDevice_D3D9 *pGraphics,
	PyroDWord nVertices,
	PyroDWord VertexSize,
	PyroDWord VertexFormat,
	PyroDWord Flags,
	BOOL b3D)
{
	*ppVB = NULL;

	HRESULT hr;

	LPDIRECT3DVERTEXBUFFER9 pd3dVB = NULL;

	if (FAILED(hr = pGraphics->GetD3D9Device()->CreateVertexBuffer(nVertices * VertexSize,
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, GetD3DFVF(VertexFormat), D3DPOOL_DEFAULT, &pd3dVB, 0)))
			return hr;

	LPDIRECT3DVERTEXDECLARATION9 pd3dVertexDeclaration = NULL;

	D3DVERTEXELEMENT9 Declaration[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		{ 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	if (b3D && FAILED(hr = pGraphics->GetD3D9Device()->CreateVertexDeclaration(Declaration, &pd3dVertexDeclaration)))
		return hr;

	CVertexBuffer_D3D9 *pVB = new CVertexBuffer_D3D9(pd3dVB, pd3dVertexDeclaration);

	pVB->m_VertexFormat = VertexFormat;
	pVB->m_VertexSize = VertexSize;

	*ppVB = pVB;

	return PyroOK;
}

PyroResult PyroParticles::CVertexBuffer_D3D9::CopyBufferSubData(void *pSrc, PyroDWord Offset, PyroDWord Size, PyroBool bDiscard)
{
	HRESULT hr;

	PVOID pBuffer;
	if (FAILED(hr = m_pVB->Lock(Offset, Size, &pBuffer, bDiscard ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE)))
		return hr;

	memcpy(pBuffer, pSrc, Size);

	if (FAILED(hr = m_pVB->Unlock()))
		return hr;

	return PyroOK;
}
