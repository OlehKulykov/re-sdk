/*
 * Copyright (c) 2002-2008 Fenomen Games.  All Rights Reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * Support: pyro-support@fenomen-games.com
 *
 */

/* $ Revision 1.3.10 $ */

#ifndef _PYRO_GRAPHICS_H_INCLUDED
#define _PYRO_GRAPHICS_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

#ifndef ENGINE_FLASH

#include <assert.h>
#include <string.h>

#endif /* ENGINE_FLASH */

#include "PyroTypes.h"

namespace PyroParticles
{

namespace PyroGraphics
{
	class CPyroGraphicsException : public CPyroException
	{
		PyroResult m_hr;

	public:

		CPyroGraphicsException(PyroResult hr)
		{
			m_hr = hr;
		}

		CPyroGraphicsException(PyroResult hr, const char *pMessage) :
			CPyroException(pMessage)
		{
			m_hr = hr;
		}

		PyroResult GetPyroResult() const { return m_hr; }

	};

	class CBitmap
	{

	private:

		PyroByte *m_pBuffer;
		PyroDWord m_Width;
		PyroDWord m_Height;
        PyroDWord m_Flags;

	public:

		enum EBitmapFlags
		{
			BITMAP_ENCODED = 0x1
		};

		CBitmap(PyroByte *pBuffer, PyroDWord Width, unsigned int Height, PyroDWord Flags)
		{
			m_pBuffer = pBuffer;
			m_Width = Width;
			m_Height = Height;
			m_Flags = Flags;
		}

		virtual ~CBitmap()
		{
		}

		PyroDWord GetWidth() const { return m_Width; }
		PyroDWord GetHeight() const { return m_Height; }
		PyroByte *GetBuffer() const { return m_pBuffer; }
		PyroDWord GetFlags() const { return m_Flags; }
		PyroBool IsEncoded() const { return (m_Flags & BITMAP_ENCODED) ? PyroTrue : PyroFalse; }

	};

	class IDeviceObject
	{

	public:

		virtual PyroResult Restore() { return 0; }
		virtual PyroResult Invalidate() { return 0; }

	};

	/* Manipulates texture resources */
	class ITexture : public IDeviceObject
	{

	public:

		virtual ~ITexture() {}

		virtual void GetMemoryUsage(PyroDWord &SystemMemory, PyroDWord &VideoMemory)
		{
		}

	};

	/* Manipulates vertex buffer resources */
	class IVertexBuffer : public IDeviceObject
	{

	public:

		enum EVertexFormat
		{
			VF_POSITION 	= 0x00000001,
			VF_RHW 			= 0x00000004,
			VF_DIFFUSE 		= 0x00000008,
			VF_NORMAL 		= 0x00000010,
			VF_TEXTURE0  	= 0x00010000,
			VF_TEXTURE1  	= 0x00020000,
			VF_TEXTURE2 	= 0x00040000,
			VF_TEXTURE3  	= 0x00080000
		};

		/* Locks a range of vertex data and obtains a pointer to the vertex buffer memory */
		virtual PyroResult Lock() = 0;

		/* Unlocks vertex data */
		virtual PyroResult Unlock() = 0;

		/* Returns address of a pointer to vertex buffer memory */
		virtual void *GetBuffer() = 0;

		virtual ~IVertexBuffer() {}

	};

	/* Manipulates vertex buffer resources */
	class IVertexBuffer2 : public IDeviceObject
	{

	public:

		enum EVertexFormat
		{
			VF_POSITION 	= 0x00000001,
			VF_RHW 			= 0x00000004,
			VF_DIFFUSE 		= 0x00000008,
			VF_NORMAL 		= 0x00000010,
			VF_TEXTURE0  	= 0x00010000,
			VF_TEXTURE1  	= 0x00020000,
			VF_TEXTURE2 	= 0x00040000,
			VF_TEXTURE3  	= 0x00080000
		};

		virtual PyroResult CopyBufferSubData(void *pSrc, PyroDWord Offset, PyroDWord Size, PyroBool bDiscard) = 0;
		virtual ~IVertexBuffer2() {}

	};

	class CVertexBuffer2Adaptor : public IVertexBuffer2
	{
		IVertexBuffer *m_pVertexBuffer;

	public:

		CVertexBuffer2Adaptor(IVertexBuffer *pVertexBuffer)
		{
			m_pVertexBuffer = pVertexBuffer;
		}

		virtual PyroResult CopyBufferSubData(void *pSrc, PyroDWord Offset, PyroDWord Size, PyroBool bDiscard)
		{
			PyroResult hr;
			if ((hr = m_pVertexBuffer->Lock()) != PyroOK)
				return hr;

			memcpy(((PyroByte *) m_pVertexBuffer->GetBuffer()) + Offset, pSrc, Size);

			if ((hr = m_pVertexBuffer->Unlock()) != PyroOK)
				return hr;

			return PyroOK;
		}

		IVertexBuffer *GetVertexBuffer() const { return m_pVertexBuffer; }

		virtual ~CVertexBuffer2Adaptor()
		{
			delete m_pVertexBuffer;
		}

	};

	/* Manipulates index buffer resources */
	class IIndexBuffer : public IDeviceObject
	{

	public:

		enum EIndexFormat
		{
			IF_WORD
		};

		/* Locks a range of index data and obtains a pointer to the index buffer memory */
		virtual PyroResult Lock() = 0;

		/* Unlocks index data */
		virtual PyroResult Unlock() = 0;

		/* Returns address of a pointer to index buffer memory */
		virtual void *GetBuffer() = 0;

		virtual ~IIndexBuffer() {}

	};

	/* Performs DrawPrimitive-based rendering, create resources, work with system-level variables */
	class IDevice
	{

	public:	

		enum ERenderStates
		{
			RS_ALPHABLENDENABLE = 0,
			RS_SRCBLEND,
			RS_DESTBLEND
		};

		enum EBlend
		{
			BLEND_ZERO,
			BLEND_ONE,
			BLEND_DST_COLOR,
			BLEND_ONE_MINUS_DST_COLOR,
			BLEND_SRC_ALPHA,
			BLEND_ONE_MINUS_SRC_ALPHA,
			BLEND_DST_ALPHA,
			BLEND_ONE_MINUS_DST_ALPHA,
			BLEND_SRC_ALPHA_SATURATE,
			BLEND_SRC_COLOR,
			BLEND_ONE_MINUS_SRC_COLOR
		};

		/* Sets a single device render-state parameter */
		virtual PyroResult SetRenderState(PyroDWord State, PyroDWord Value) = 0;

		virtual PyroResult SetBlendFunc(EBlend SrcBlend, EBlend DestBlend) = 0;

		/* Create a vertex buffer */
		virtual PyroResult CreateVertexBuffer(IVertexBuffer **ppVertexBuffer, PyroDWord Vertices, PyroDWord VertexSize, PyroDWord VertexFormat, PyroDWord Flags)
		{
			assert(0 && "You must implement CreateVertexBuffer method");

			return PyroFailed;
		}

		/* Create a vertex buffer */
		virtual PyroResult CreateVertexBuffer2(IVertexBuffer2 **ppVertexBuffer2, PyroDWord Vertices, PyroDWord VertexSize, PyroDWord VertexFormat, PyroDWord Flags)
		{
			IVertexBuffer *ppVertexBuffer;

			PyroResult hr;
			if ((hr = CreateVertexBuffer(&ppVertexBuffer, Vertices, VertexSize, VertexFormat, Flags)) != PyroOK)
				return hr;
			
			*ppVertexBuffer2 = new CVertexBuffer2Adaptor(ppVertexBuffer);

			return PyroOK;
		}

		/* Create a index buffer */
		virtual PyroResult CreateIndexBuffer(IIndexBuffer **ppIndexBuffer, PyroDWord Indexes, PyroDWord IndexFormat, PyroDWord Flags) = 0;

		/* Pointer to a bitmap from which texture will be created */
		virtual PyroResult CreateTexture(ITexture **ppTexture, CBitmap *pBitmap, PyroDWord Flags) = 0;

		virtual PyroResult CreateBitmap(CBitmap **ppBitmap, PyroByte *pBuffer, PyroDWord Width, PyroDWord Height, PyroDWord Flags) = 0;

		/* Modifies the current world transformation */
		virtual PyroResult SetWorldMatrix(float WorldMatrix[3][4]) = 0;

		/* Assigns a texture to a stage for a device */
		virtual PyroResult SetTexture(PyroDWord Stage, PyroParticles::PyroGraphics::ITexture *pTexture) = 0;

		virtual PyroResult RenderQuads(IVertexBuffer *pVertexBuffer, PyroDWord nFirstQuad, PyroDWord nQuads, PyroDWord Flags)
		{
			assert(0 && "You must implement RenderQuads method");

			return PyroFailed;
		}

		virtual PyroResult RenderQuads2(IVertexBuffer2 *pVertexBuffer, PyroDWord nFirstQuad, PyroDWord nQuads, PyroDWord Flags)
		{
			// assert(Engine::dyn_cast<CVertexBuffer2Adaptor *>(pVertexBuffer) && "You must implement RenderQuads2 method instead of RenderQuads");

			CVertexBuffer2Adaptor *pVertexBuffer2Adaptor = static_cast<CVertexBuffer2Adaptor *>(pVertexBuffer);

			return RenderQuads(pVertexBuffer2Adaptor->GetVertexBuffer(), nFirstQuad, nQuads, Flags);
		}

		virtual PyroBool IsRGBA() const = 0;

		virtual ~IDevice() {}

	}; /* IDevice */
    
} /* PyroGraphics */

} /* Particles */


#endif /* _PYRO_GRAPHICS_H_INCLUDED */
