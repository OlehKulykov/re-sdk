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

#include "PyroOpenGL.h"

PyroParticles::CTexture_OGL::CTexture_OGL(PyroParticles::PyroGraphics::CBitmap *pBitmap, PyroDWord Flags)
{
	glGenTextures(1, (GLuint *) &m_glTexture);
	glBindTexture(GL_TEXTURE_2D, m_glTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
	             pBitmap->GetWidth(), pBitmap->GetHeight(),  0, GL_RGBA,
		         GL_UNSIGNED_BYTE, pBitmap->GetBuffer());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

PyroResult PyroParticles::CGraphics_OGL::RenderQuads(
	PyroParticles::PyroGraphics::IVertexBuffer *_pVertexBuffer,
	PyroDWord nFirst,
	PyroDWord nQuads,
	PyroDWord Flags)
{
	if (nQuads == 0)
		return PyroOK;

	PyroDWord nCount = nQuads * 4;

	CVertexBuffer_OGL *pVertexBuffer = (CVertexBuffer_OGL *) _pVertexBuffer;

	CIndexBuffer_OGL *pIndexBuffer = NULL;

	int glPrimType = GL_QUADS;

	PyroDWord VertexSize = pVertexBuffer->GetVertexSize();

	PyroDWord Format = pVertexBuffer->GetVertexFormat();
	pVertexBuffer->Lock();
	PyroByte *pVertex = (PyroByte *) pVertexBuffer->GetBuffer();
	pVertexBuffer->Unlock();
    
	if (Format & PyroParticles::PyroGraphics::IVertexBuffer::VF_POSITION)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
	 	glVertexPointer(3, GL_FLOAT, VertexSize, pVertex);
	 	pVertex += sizeof(float) * 3;
	}

	if (Format & PyroParticles::PyroGraphics::IVertexBuffer::VF_RHW)
		pVertex += sizeof(float);
	
	if (Format & PyroParticles::PyroGraphics::IVertexBuffer::VF_NORMAL)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
	 	glNormalPointer(GL_FLOAT, VertexSize, pVertex);
	 	pVertex += sizeof(float) * 3;
	}

	if (Format & PyroParticles::PyroGraphics::IVertexBuffer::VF_DIFFUSE)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, VertexSize, pVertex);
		pVertex += 4;
	}

	if (Format & PyroParticles::PyroGraphics::IVertexBuffer::VF_TEXTURE0)
	{	
		if (!m_bNullTexture[0])
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, VertexSize, pVertex);
		}

		pVertex += sizeof(float) * 2;
	}

	if (!pIndexBuffer)
	{
		glDrawArrays(glPrimType, nFirst * 4, nCount);
	}
	else
	{
///			glDrawElements(glPrimType, nCount, (pIndexBuffer->GetType() == G_WORD) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, pIndexBuffer_OGL->GetBuffer());
	}
    
	if (Format & PyroParticles::PyroGraphics::IVertexBuffer::VF_POSITION)
		glDisableClientState(GL_VERTEX_ARRAY);

	if (Format & PyroParticles::PyroGraphics::IVertexBuffer::VF_NORMAL)
		glDisableClientState(GL_NORMAL_ARRAY);

	if (Format & PyroParticles::PyroGraphics::IVertexBuffer::VF_DIFFUSE)
		glDisableClientState(GL_COLOR_ARRAY);

	if (Format & PyroParticles::PyroGraphics::IVertexBuffer::VF_TEXTURE0)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  	return PyroOK; 
}

PyroResult PyroParticles::CGraphics_OGL::SetBlendFunc(
	PyroParticles::PyroGraphics::IDevice::EBlend SrcBlend,
	PyroParticles::PyroGraphics::IDevice::EBlend DestBlend)
{
	int glSrcBlend;

	switch (SrcBlend)
	{
	case BLEND_ZERO:					glSrcBlend = GL_ZERO;	 				break;
	case BLEND_ONE:						glSrcBlend = GL_ONE; 					break;
	case BLEND_DST_COLOR:				glSrcBlend = GL_DST_COLOR; 				break;
	case BLEND_ONE_MINUS_DST_COLOR:		glSrcBlend = GL_ONE_MINUS_DST_COLOR; 	break;
	case BLEND_SRC_ALPHA:				glSrcBlend = GL_SRC_ALPHA; 				break;
	case BLEND_ONE_MINUS_SRC_ALPHA:		glSrcBlend = GL_ONE_MINUS_SRC_ALPHA; 	break;
	case BLEND_DST_ALPHA:				glSrcBlend = GL_DST_ALPHA; 				break;
	case BLEND_ONE_MINUS_DST_ALPHA:		glSrcBlend = GL_ONE_MINUS_DST_ALPHA; 	break;
	case BLEND_SRC_ALPHA_SATURATE:		glSrcBlend = GL_SRC_ALPHA_SATURATE; 	break;
	default:							return PyroOK;
	}

	int glDestBlend;

	switch (DestBlend)
	{
	case BLEND_ZERO:					glDestBlend = GL_ZERO;	 				break;
	case BLEND_ONE:						glDestBlend = GL_ONE; 					break;
	case BLEND_DST_COLOR:				glDestBlend = GL_DST_COLOR; 			break;
	case BLEND_ONE_MINUS_DST_COLOR:		glDestBlend = GL_ONE_MINUS_DST_COLOR; 	break;
	case BLEND_SRC_ALPHA:				glDestBlend = GL_SRC_ALPHA; 			break;
	case BLEND_ONE_MINUS_SRC_ALPHA:		glDestBlend = GL_ONE_MINUS_SRC_ALPHA; 	break;
	case BLEND_DST_ALPHA:				glDestBlend = GL_DST_ALPHA; 			break;
	case BLEND_ONE_MINUS_DST_ALPHA:		glDestBlend = GL_ONE_MINUS_DST_ALPHA; 	break;
	case BLEND_SRC_ALPHA_SATURATE:		glDestBlend = GL_SRC_ALPHA_SATURATE; 	break;
	default:							return PyroOK;
	}

	glBlendFunc(glSrcBlend, glDestBlend);

	return PyroOK;
}

PyroResult PyroParticles::CGraphics_OGL::SetRenderState(PyroDWord State, PyroDWord Value)
{
	int glState;

	switch (State)
	{
	case RS_ALPHABLENDENABLE:		glState = GL_BLEND; break;
	default:						return PyroOK;
	}

	if (Value)
			glEnable(glState);
	else	glDisable(glState);

	return PyroOK;
}

PyroResult PyroParticles::CGraphics_OGL::SetWorldMatrix(float WorldMatrix[3][4])
{
	return PyroOK;
}

PyroResult PyroParticles::CGraphics_OGL::CreateVertexBuffer(
	PyroParticles::PyroGraphics::IVertexBuffer **ppVertexBuffer,
	PyroDWord Vertices,
	PyroDWord VertexSize,
	PyroDWord VertexFormat,
	PyroDWord Flags)
{
	*ppVertexBuffer = new CVertexBuffer_OGL(this, Vertices, VertexSize, VertexFormat, Flags);

	return PyroOK;
}

PyroResult PyroParticles::CGraphics_OGL::CreateIndexBuffer(PyroParticles::PyroGraphics::IIndexBuffer **ppIndexBuffer, PyroDWord ItemType, PyroDWord nItems, PyroDWord Flags)
{
	*ppIndexBuffer = new CIndexBuffer_OGL(this, ItemType, nItems, Flags);

	return PyroOK;
}

PyroResult PyroParticles::CGraphics_OGL::CreateTexture(PyroParticles::PyroGraphics::ITexture **ppTexture, PyroParticles::PyroGraphics::CBitmap *pBitmap, PyroDWord Flags)
{
	*ppTexture = new CTexture_OGL(pBitmap, Flags);

	return PyroOK;
}

PyroResult PyroParticles::CGraphics_OGL::CreateBitmap(PyroParticles::PyroGraphics::CBitmap **ppBitmap, PyroByte *pBuffer, PyroDWord Width, PyroDWord Height, PyroDWord Flags)
{
	*ppBitmap = new PyroParticles::PyroGraphics::CBitmap(pBuffer, Width, Height, Flags);

	return PyroOK;
}

PyroResult PyroParticles::CGraphics_OGL::SetTexture(PyroDWord Stage, PyroParticles::PyroGraphics::ITexture *pTexture)
{
	assert(Stage == 0);

	if (pTexture)
	{
		CTexture_OGL *pTexture_OGL = (CTexture_OGL *) pTexture;

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pTexture_OGL->GetOGLTexture());

		m_bNullTexture[Stage] = false;
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
		m_bNullTexture[Stage] = true;
	}

	return PyroOK;
}
