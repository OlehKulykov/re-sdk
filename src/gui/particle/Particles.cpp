/*
 * Copyright (c) 2002-2008 Fenomen Games.  All Rights Reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * REPRODUCTION IN WHOLE OR IN PART IS PROHIBITED WITHOUT THE WRITTEN
 * CONSENT OF THE COPYRIGHT OWNER.
 *
 */

/* $ Revision 1.3.10 $ */

//#include "StdAfx.H"

#ifndef ENGINE_NO_PYRO

#ifndef ENGINE_FLASH

#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
// #include "../StdAfx.H"
#include "Particles.h"
#include "PyroTypes.h"
#include "Random.h"
#include "Timer.h"

/*
 PyroParticles::CPyroException::CPyroException() { m_pMessage = NULL; }
 
 PyroParticles::CPyroException::CPyroException(char *pMessage)
 {
 m_pMessage = strdup(pMessage);
 }
 
 PyroParticles::CPyroException::CPyroException(const CPyroException &Exception)
 {
 m_pMessage = strdup(Exception.m_pMessage);
 }
 
 PyroParticles::CPyroException::CPyroException(const char *pError, ...)
 {
 va_list arg_list;
 va_start(arg_list, pError);
 
 Engine::CString Msg = Engine::CString::FormatStatic(pError, arg_list);
 
 m_pMessage = strdup(Msg.GetBuffer());
 
 va_end(arg_list);
 }
 
 PyroParticles::CPyroException::~CPyroException()
 {
 if (m_pMessage)
 {
 free(m_pMessage);
 m_pMessage = NULL;
 }
 }
 
 const char *PyroParticles::CPyroException::GetExceptionMessage() const { return m_pMessage; }
 */

PyroParticles::CPyroAse::CPyroAse()
{
	m_pBitmapList = NULL;
	m_nBitmaps = 0;
	
	m_pMaterialList = NULL;
	m_nMaterials = 0;
	
	m_pObjectList = NULL;
	m_nObjects = 0;
}

PyroParticles::CPyroAse::~CPyroAse()
{
	if (m_pObjectList)
	{
		for (int i = 0; i < m_nObjects; i ++)
			delete m_pObjectList[i];
		
		delete m_pObjectList;
	}
	
	if (m_pBitmapList)
		delete[] m_pBitmapList;
	
	if (m_pMaterialList)
		delete[] m_pMaterialList;
}

void PyroParticles::CPyroAse::CBitmap::Serialize(Engine::CArchive &ar, int nVersion)
{
	ar >> m_ID;
	
	Engine::CString FileName;
	ar >> FileName;
	
	ar >> m_nFileBytes;
	
	if (m_nFileBytes)
	{
		m_pFile = new BYTE[m_nFileBytes];
		ar.Read(m_pFile, m_nFileBytes);
	}
}

void PyroParticles::CPyroAse::CBitmap::CreateTexture(CPyroParticleLibrary *pLibrary)
{
	if (m_pTexture)
		return;
	
	CMemoryFile MemFile(m_pFile, m_nFileBytes);
	CBitmapFile BitmapFile(MemFile);
	
	CBitmapIO BitmapIO;
	
	BitmapIO.Load(BitmapFile);
	
	HRESULT hr = pLibrary->GetGraphicsDevice()->CreateBitmap(&m_pBitmap, (BYTE *) BitmapIO.GetImage(), BitmapIO.GetWidth(), BitmapIO.GetHeight(), 0);
	if (FAILED(hr))
	{
		return;
	}
	
	BitmapIO.SetImage(NULL);
	
	// FIXME
	m_pTexture = NULL; // pLibrary->GetTextureLocalManager().CreateTexture(m_pBitmap);
}

PyroParticles::CPyroAse::CBitmap *PyroParticles::CPyroAse::FindBitmap(REUInt32 ID)
{
	for (int i = 0; i < m_nBitmaps; i ++)
		if (m_pBitmapList[i].m_ID == ID)
			return m_pBitmapList + i;
	
	return NULL;
}

void PyroParticles::CPyroAse::CTexture::Serialize(Engine::CArchive &ar, int nVersion)
{
	ar >> m_Angle;
	ar >> m_UTiling;
	ar >> m_VTiling;
	ar >> m_UOffset;
	ar >> m_VOffset;
	
	REUInt32 BitmapID;
	ar >> BitmapID;
	
	m_pBitmap = m_pAse->FindBitmap(BitmapID);
	assert(m_pBitmap);
}

void PyroParticles::CPyroAse::CMaterial::Serialize(Engine::CArchive &ar, int nVersion)
{
	ar >> m_ID;
	
	ar >> m_Ambient.r;
	ar >> m_Ambient.g;
	ar >> m_Ambient.b;
	
	ar >> m_Diffuse.r;
	ar >> m_Diffuse.g;
	ar >> m_Diffuse.b;
	
	ar >> m_Specular.r;
	ar >> m_Specular.g;
	ar >> m_Specular.b;
	
	ar >> m_SpecularLevel;
	ar >> m_Glossiness;
	ar >> m_Opacity;
	ar >> m_SelfIllumination;
	
	bool bUseDiffuseTexture = ar.DeserializeBOOL();
	
	if (bUseDiffuseTexture)
	{
		m_pDiffuseTexture = m_pAse->CreateTexture();
		m_pDiffuseTexture->Serialize(ar, nVersion);
	}
	
	/*bool bUseReflectionTexture = */ar.DeserializeBOOL();
	
	if (m_pReflectionTexture)
	{
		m_pReflectionTexture = m_pAse->CreateTexture();
		m_pReflectionTexture->Serialize(ar, nVersion);
	}
}

PyroParticles::CPyroAse::CMaterial *PyroParticles::CPyroAse::FindMaterial(REUInt32 ID)
{
	for (int i = 0; i < m_nMaterials; i ++)
		if (m_pMaterialList[i].m_ID == ID)
			return m_pMaterialList + i;
	
	return NULL;
}

void PyroParticles::CPyroAse::CreateTextures(CPyroParticleLibrary *pLibrary)
{
	for (int i = 0; i < m_nBitmaps; i ++)
		m_pBitmapList[i].CreateTexture(pLibrary);
}

void PyroParticles::CPyroAse::CGeomObject::Serialize(Engine::CArchive &ar, int nVersion)
{
	CObject::Serialize(ar, nVersion);
	m_Mesh.Serialize(ar, nVersion);
	
	REUInt32 MaterialID;
	ar >> MaterialID;
	
	if (MaterialID)
	{
		m_pMaterial = m_pAse->FindMaterial(MaterialID);
		assert(m_pMaterial);
	}
}

PyroParticles::CPyroAse::CObject *PyroParticles::CPyroAse::FindObject(REUInt32 ID)
{
	for (int i = 0; i < m_nObjects; i ++)
		if (m_pObjectList[i]->m_ID == ID)
			return m_pObjectList[i];
	
	return NULL;
}

void PyroParticles::CPyroAse::CObject::Serialize(Engine::CArchive &ar, int nVersion)
{
	ar >> m_ID;
	
	Engine::CString Name;
	ar >> Name;
	
	for (int i = 0; i < 3; i ++)
		for (int j = 0; j < 4; j ++)
			ar >> m_NodeTM.m_Matrix[i][j];
}

void PyroParticles::CPyroAse::CMeshMappingChannel::Serialize(Engine::CArchive &ar, int nVersion)
{
	ar >> m_nTVertices;
	
	if (m_nTVertices)
	{
		m_pTVertexList = new CTVertex[m_nTVertices];
		
        for (int i = 0; i < m_nTVertices; i ++)
		{
			ar >> m_pTVertexList[i].u;
			ar >> m_pTVertexList[i].v;
			// ar >> m_pTVertexList[i].w;
		}
	}
	
	ar >> m_nTFaces;
	
	if (m_nTFaces)
	{
		m_pTFaceList = new CTFace[m_nTFaces];
		
		for (int i = 0; i < m_nTFaces; i ++)
        {
			ar >> m_pTFaceList[i].v0;
			ar >> m_pTFaceList[i].v1;
			ar >> m_pTFaceList[i].v2;
        }
	}
}

void PyroParticles::CPyroAse::CMesh::CreateVertexAndIndexBuffers(PyroGraphics::IDevice *pGraphicsDevice)
{
	CMeshMappingChannel &MapChannel = m_MapChannels[0];
	
	if (0)
	{
		REUInt32 VertexFormat = PyroParticles::PyroGraphics::IVertexBuffer::VF_POSITION |
		PyroParticles::PyroGraphics::IVertexBuffer::VF_TEXTURE0 |
		PyroParticles::PyroGraphics::IVertexBuffer::VF_DIFFUSE;
		
		HRESULT hr = pGraphicsDevice->CreateVertexBuffer(&m_pVB, m_nVertices, sizeof(CPyroParticleMeshVertex), VertexFormat, 0);
		if (FAILED(hr))
		{
			return;
		}
		
		pGraphicsDevice->CreateIndexBuffer(&m_pIB, m_nFaces * 3, PyroParticles::PyroGraphics::IIndexBuffer::IF_WORD, 0);
		if (FAILED(hr))
		{				
			return;
		}
		
		hr = m_pVB->Lock();
		if (FAILED(hr))
		{
			return;
		}
		
		CPyroParticleMeshVertex *pVertex = (CPyroParticleMeshVertex *) m_pVB->GetBuffer();
		
		int i;
		
		for (i = 0; i < m_nVertices; i ++)
		{
			pVertex[i].x = m_pVertexList[i].x;
			pVertex[i].y = m_pVertexList[i].y;
			pVertex[i].z = m_pVertexList[i].z;
			
			pVertex[i].tu = 0.5f;
			pVertex[i].tv = 0.5f;
			
			pVertex[i].diffuse = 0xffffffff;
		}
		
		hr = m_pVB->Unlock();
		if (FAILED(hr))
		{
			return;
		}
		
		hr = m_pIB->Lock();
		if (FAILED(hr))
		{
			return;
		}
		
		REUInt16 *pVertexIndex = (REUInt16 *) m_pIB->GetBuffer();
		for (i = 0; i < m_nFaces; i ++)
		{
			pVertexIndex[i * 3 + 0] = (REUInt16) m_pFaceList[i].v0;
			pVertexIndex[i * 3 + 1] = (REUInt16) m_pFaceList[i].v1;
			pVertexIndex[i * 3 + 2] = (REUInt16) m_pFaceList[i].v2;
		}
		
		hr = m_pIB->Unlock();
		if (FAILED(hr))
		{
			return;
		}
	}
	else
	{
		REUInt32 VertexFormat = PyroParticles::PyroGraphics::IVertexBuffer::VF_POSITION |
		PyroParticles::PyroGraphics::IVertexBuffer::VF_TEXTURE0 |
		PyroParticles::PyroGraphics::IVertexBuffer::VF_DIFFUSE;
		
		HRESULT hr = pGraphicsDevice->CreateVertexBuffer(&m_pVB, m_nFaces * 3, sizeof(CPyroParticleMeshVertex), VertexFormat, 0);
		if (FAILED(hr))
		{
			return;
		}
		
		hr = m_pVB->Lock();
		if (FAILED(hr))
		{
			return;
		}
		
		CPyroParticleMeshVertex *pVertex = (CPyroParticleMeshVertex *) m_pVB->GetBuffer();
		
		int i;
		
		for (i = 0; i < m_nFaces; i ++)
		{
			CFace &Face = m_pFaceList[i];
			CTFace &TFace = MapChannel.m_pTFaceList[i];
			
			pVertex[i * 3 + 0].x = m_pVertexList[Face.v0].x;
			pVertex[i * 3 + 0].y = m_pVertexList[Face.v0].y;
			pVertex[i * 3 + 0].z = m_pVertexList[Face.v0].z;
			
			pVertex[i * 3 + 0].tu = MapChannel.m_pTVertexList[TFace.v0].u;
			pVertex[i * 3 + 0].tv = MapChannel.m_pTVertexList[TFace.v0].v;
			
			pVertex[i * 3 + 0].diffuse = 0xffffffff;
			
			pVertex[i * 3 + 1].x = m_pVertexList[Face.v1].x;
			pVertex[i * 3 + 1].y = m_pVertexList[Face.v1].y;
			pVertex[i * 3 + 1].z = m_pVertexList[Face.v1].z;
			
			pVertex[i * 3 + 1].tu = MapChannel.m_pTVertexList[TFace.v1].u;
			pVertex[i * 3 + 1].tv = MapChannel.m_pTVertexList[TFace.v1].v;
			
			pVertex[i * 3 + 1].diffuse = 0xffffffff;
			
			pVertex[i * 3 + 2].x = m_pVertexList[Face.v2].x;
			pVertex[i * 3 + 2].y = m_pVertexList[Face.v2].y;
			pVertex[i * 3 + 2].z = m_pVertexList[Face.v2].z;
			
			pVertex[i * 3 + 2].tu = MapChannel.m_pTVertexList[TFace.v2].u;
			pVertex[i * 3 + 2].tv = MapChannel.m_pTVertexList[TFace.v2].v;
			
			pVertex[i * 3 + 2].diffuse = 0xffffffff;
		}
		
		hr = m_pVB->Unlock();
		if (FAILED(hr))
		{
			return;
		}
	}
}

void PyroParticles::CPyroAse::CMesh::Serialize(Engine::CArchive &ar, int nVersion)
{
	ar >> m_nVertices;
	
	if (m_nVertices)
	{
		m_pVertexList = new CVertex[m_nVertices];
		
		for (int i = 0; i < m_nVertices; i ++)
		{
			ar >> m_pVertexList[i].x;
			ar >> m_pVertexList[i].y;
			ar >> m_pVertexList[i].z;
		}
	}
	
	ar >> m_nFaces;
	
	if (m_nFaces)
	{
		m_pFaceList = new CFace[m_nFaces];
		
		for (int i = 0; i < m_nFaces; i ++)
		{
			ar >> m_pFaceList[i].v0;
			ar >> m_pFaceList[i].v1;
			ar >> m_pFaceList[i].v2;
		}
	}
	
	int nMapChannels;
	ar >> nMapChannels;
	
	for (int i = 0; i < nMapChannels; i ++)
		m_MapChannels[i].Serialize(ar, nVersion);
}

void PyroParticles::CPyroAse::Serialize(Engine::CArchive &ar, int nVersion)
{
	ar >> m_nBitmaps;
	
	if (m_nBitmaps)
	{
		m_pBitmapList = new CBitmap[m_nBitmaps];
		
		for (int i = 0; i < m_nBitmaps; i ++)
		{
			m_pBitmapList->m_pAse = this;
			m_pBitmapList[i].Serialize(ar, nVersion);
		}
	}
	
	ar >> m_nMaterials;
	
	if (m_nMaterials)
	{
		m_pMaterialList = new CMaterial[m_nMaterials];
		
		for (int i = 0; i < m_nMaterials; i ++)
        {
			m_pMaterialList[i].m_pAse = this;
			m_pMaterialList[i].Serialize(ar, nVersion);
		}
	}
	
	ar >> m_nObjects;
	
	if (m_nObjects)
	{
		m_pObjectList = new CObject*[m_nObjects];
		
		for (int i = 0; i < m_nObjects; i ++)
		{
			int ObjectType;
			ar >> ObjectType;
			
			CObject *pObject = NULL;
			
			switch (ObjectType)
			{
				case PYRO_ASE_GEOM_OBJECT:    pObject = new CGeomObject; break;
			}
			
			assert(pObject);
			
			pObject->m_pAse = this;
			m_pObjectList[i] = pObject;
			
			pObject->Serialize(ar, nVersion);
		}
	}
}

void PyroParticles::CPyroParticleMesh::Serialize(Engine::CArchive &ar, int nVersion)
{
	ar >> m_NormalizationVector[0];
	ar >> m_NormalizationVector[1];
	ar >> m_NormalizationVector[2];
}

PyroParticles::CPyroParticleMesh *PyroParticles::CPyroParticleMeshes::FindMesh(REUInt32 ID)
{
	for (int i = 0; i < m_nMeshes; i ++)
		if (m_pMeshList[i].m_ID == ID)
			return m_pMeshList + i;
	
	return NULL;
}

void PyroParticles::CPyroParticleMeshes::CreateVertexAndIndexBuffers(PyroGraphics::IDevice *pGraphicsDevice)
{
	for (int i = 0; i < m_nMeshes; i ++)
		m_pMeshList[i].CreateVertexAndIndexBuffers(pGraphicsDevice);
}

void PyroParticles::CPyroParticleMeshes::Serialize(Engine::CArchive &ar, int nVersion)
{
	m_World.Serialize(ar, nVersion);
	
	ar >> m_nMeshes;
	
	if (m_nMeshes)
	{
		m_pMeshList = new CPyroParticleMesh[m_nMeshes];
		
		for (int i = 0; i < m_nMeshes; i ++)
		{
			REUInt32 MeshID;
			ar >> MeshID;
			
			REUInt32 ObjectID;
			ar >> ObjectID;
			
			PyroParticles::CPyroAse::CObject *pObject = m_World.FindObject(ObjectID);
			CPyroParticleMesh *pMesh = m_pMeshList + i;
			
			assert(pObject);
			
			pMesh->m_ID = MeshID;
			pMesh->m_pGeomObject = (PyroParticles::CPyroAse::CGeomObject *) pObject;
			
			pMesh->Serialize(ar, nVersion);
		}
	}
}

PyroParticles::CPyroParticleLayerMeshSet::CPyroParticleLayerMeshSet()
{
	m_nMeshes = 0;
	m_pMeshList = NULL;
	
	m_pMeshes = NULL;
	
	m_bClone = false;
	
	m_SumProbability = 0;
}

PyroParticles::CPyroParticleLayerMeshSet::~CPyroParticleLayerMeshSet()
{
	if (m_bClone)
		return;
	
	if (m_pMeshList)
		delete m_pMeshList;
}

void PyroParticles::CPyroParticleLayerMeshSet::Serialize(Engine::CArchive &ar, int nVersion)
{
	ar >> m_nMeshes;
	
	if (m_nMeshes)
	{
		m_pMeshList = new CLayerMesh[m_nMeshes];
		
		for (int i = 0; i < m_nMeshes; i ++)
		{
			REUInt32 MeshID;
			ar >> MeshID;
			
			int Probability;
			ar >> Probability;
			
			m_SumProbability += Probability;
			
			float Scale;
			ar >> Scale;
			
			bool bRandomAxis = ar.DeserializeBOOL();
			
			CPyroParticleMesh *pMesh = m_pMeshes->FindMesh(MeshID);
			assert(pMesh);
			
            CLayerMesh *pLayerMesh = m_pMeshList + i;
			
            pLayerMesh->m_pMesh = pMesh;
            pLayerMesh->m_Probability = Probability;
            pLayerMesh->m_Scale = Scale;
            pLayerMesh->m_bRandomAxis = bRandomAxis;
		}
	}
}

float PyroParticles::CPyroParticleParam::GetValueInternal(float Frame) const /* throw() */
{
	if (m_nKeys == 1 || Frame <= m_KeyList[0].Frame)
		return m_KeyList[0].Value;
	
	if (Frame >= m_KeyList[m_nKeys - 1].Frame)
		return m_KeyList[m_nKeys - 1].Value;
	
	CKey *pKey = m_KeyList;
	
	while (Frame > pKey[1].Frame)
		pKey ++;
	
	float t = (Frame - pKey[0].Frame) /
	(pKey[1].Frame - pKey[0].Frame);
	
	if (m_bBezierCurve)
		t = t * t * (3.0f - 2.0f * t);
	
	return (pKey[1].Value - pKey[0].Value) * t + pKey[0].Value;
}

void PyroParticles::CPyroParticleParam::Serialize(Engine::CArchive &ar, int nVersion)
{
	assert(m_nKeys == 0);
	
	ar >> m_nKeys;
	
	if (m_nKeys)
	{
		m_KeyList = new CKey[m_nKeys];
		
		for (int i = 0; i < m_nKeys; i ++)
		{
			CKey &key = m_KeyList[i];
			
			ar >> key.Frame;
			ar >> key.Value;
			
			if (nVersion >= 0x00016000)
			{
				ar >> key.Type;
				
				ar >> key.CntFrame[0];
				ar >> key.CntValue[0];
				
				ar >> key.CntFrame[1];
				ar >> key.CntValue[1];
			}
			else
			{
				key.Type = 0;
				
				key.CntFrame[0] = 0.0f;
				key.CntValue[0] = 0.0f;
				
				key.CntFrame[1] = 0.0f;
				key.CntValue[1] = 0.0f;
			}
		}
	}
	
	m_bBezierCurve = ar.DeserializeBOOL();
}

void PyroParticles::CPyroParticleColorParam::GetValue(float Frame, float &r, float &g, float &b, float &a)
{
	if (m_nKeys == 1 || Frame <= m_KeyList[0].Frame)
	{
		CKey * kPtr = &m_KeyList[0];
		r = kPtr->r;
		g = kPtr->g;
		b = kPtr->b;
		a = kPtr->a;
	}
	else
	{
		if (m_nRepeat > 1 && m_nKeys > 1)
		{
			Frame *= m_nRepeat;
			Frame = (float) fmod(Frame, m_KeyList[m_nKeys - 1].Frame -
								 m_KeyList[0].Frame) + m_KeyList[0].Frame;
		}
		else
			if (Frame >= m_KeyList[m_nKeys - 1].Frame)
			{
				CKey * kPtr = &m_KeyList[m_nKeys - 1];
				r = kPtr->r;
				g = kPtr->g;
				b = kPtr->b;
				a = kPtr->a;
				
				return;
			}
		
		{
			CKey* pKey = m_KeyList;
			while (pKey[1].Frame < Frame)
				pKey ++;
			
			float t = (Frame - pKey[0].Frame) / (pKey[1].Frame - pKey[0].Frame);
			
			r = (pKey[1].r - pKey[0].r) * t + pKey[0].r;
			g = (pKey[1].g - pKey[0].g) * t + pKey[0].g;
			b = (pKey[1].b - pKey[0].b) * t + pKey[0].b;
			a = (pKey[1].a - pKey[0].a) * t + pKey[0].a;
		}
	}
}

void PyroParticles::CPyroParticleColorParam::Serialize(Engine::CArchive &ar, int nVersion)
{
	ar >> m_nKeys;
	
	m_KeyList = new CKey[m_nKeys];
	
	for (int i = 0; i < m_nKeys; i ++)
	{
		CKey &key = m_KeyList[i];
		
		ar >> key.Frame;
		ar >> key.r;
		ar >> key.g;
		ar >> key.b;
		ar >> key.a;
	}
	
	ar >> m_nRepeat;
	m_bChooseRandomColor = ar.DeserializeBOOL();
}

PyroParticles::CPyroParticleLayer::~CPyroParticleLayer()
{
	CParticleListItem *pListItem = m_pListRoot;
	
	while (pListItem)
	{
		CParticleListItem *pNextItem = pListItem->pNext;
		
		if (pListItem->Particle.pSubEmitters)
		{
			for (int i = 0; i < pListItem->Particle.nSubEmitters; i ++)
				if (pListItem->Particle.pSubEmitters[i].pSubEmitter)
					delete pListItem->Particle.pSubEmitters[i].pSubEmitter;
			
			free(pListItem->Particle.pSubEmitters);
			pListItem->Particle.pSubEmitters = NULL;
		}
		
		CPyroCommon::KillParticle(pListItem);
		
		pListItem = pNextItem;
	}
	
	if (m_bClone)
		return;
	
	if (m_pSubEmitterList)
		delete[] m_pSubEmitterList;
}

int PyroParticles::CPyroParticleLayer::Render()
{
	return Render(-1, FALSE);
}

int PyroParticles::CPyroParticleLayer::Render(int MaxCount, PyroBool bUniformDistribution)
{
	if (!m_pListRoot)
		return 0;
	
	if (MaxCount == 0)
		return 0;
	
	int nParticles = 0;
	
	int nTotalParticles = 0;
	
	int LayerMaxParticles = MaxCount;
	int LayerParticles = m_LayerParticles;
	int LayerRenderedParticles = 0;
	
	if (MaxCount > 0)
	{
		nTotalParticles = GetNumParticles();
	}
	
	if (m_nSubEmitters)
	{
		CParticleListItem *pListItem = m_pListRoot;
		
		do
		{
			if (pListItem->Particle.pSubEmitters)
			{
				for (int i = 0; i < pListItem->Particle.nSubEmitters; i ++)
				{
					int nSubEmitterMaxParticles;
					
					if (MaxCount < 0)
					{
						nSubEmitterMaxParticles = -1;
					}
					else
					{
						int nSubEmitterParticles = pListItem->Particle.pSubEmitters[i].pSubEmitter->GetNumParticles();
						nSubEmitterMaxParticles = MaxCount * nSubEmitterParticles / nTotalParticles;
					}
					
					if (nSubEmitterMaxParticles != 0)
						nParticles += pListItem->Particle.pSubEmitters[i].pSubEmitter->Render(nSubEmitterMaxParticles, bUniformDistribution);
				}
			}
			
			pListItem = pListItem->pNext;
			
		} while (pListItem);
	}
	
	UpdateParticleRenderingData();
	
	m_pLibrary->GetGraphicsDevice()->SetRenderState(PyroGraphics::IDevice::RS_ALPHABLENDENABLE, TRUE);
	
	if (m_bIntense)
	{
		m_pLibrary->GetGraphicsDevice()->SetBlendFunc(
													  m_bPremult ? PyroGraphics::IDevice::BLEND_SRC_ALPHA : PyroGraphics::IDevice::BLEND_ONE,
													  PyroGraphics::IDevice::BLEND_ONE);
	}
	else
	{
		m_pLibrary->GetGraphicsDevice()->SetBlendFunc(
													  m_bPremult ? PyroGraphics::IDevice::BLEND_SRC_ALPHA : PyroGraphics::IDevice::BLEND_ONE,
													  PyroGraphics::IDevice::BLEND_ONE_MINUS_SRC_ALPHA);
	}
	
	PyroGraphics::ITexture *pTexture = m_pShape ? m_pShape->GetTexture() : NULL;
	m_pLibrary->GetGraphicsDevice()->SetTexture(0, pTexture);
	
	CParticleListItem *pListItem = m_pListRoot;
	
	int nBufferParticles = 0;
	int nBufferMaxParticles = m_pLibrary->GetVertexBufferMaxQuads();
	
	CPyroParticleVertex *pVertex;
	
	HRESULT hr;
	
#define CHECK_MAX_PARTICLES     \
if (LayerMaxParticles < 0 || \
(LayerMaxParticles > 0 && bUniformDistribution && \
(LayerParticles <= LayerMaxParticles || \
(CRandom::GetSingleton()->GetDWordRand() % LayerParticles) < LayerMaxParticles)) || \
(LayerMaxParticles > 0 && !bUniformDistribution && LayerRenderedParticles < LayerMaxParticles) \
)
	
	if (!m_bIntense && m_bInvertOrder)
	{
		do
		{
			CHECK_MAX_PARTICLES
			{
				LayerRenderedParticles ++;
				
				if (pListItem->Particle.Visibility > 0.01f && pListItem->Particle.bRender)
				{
					if (nBufferParticles == 0)
					{
						if (FAILED(hr = m_pLibrary->GetVertexBuffer()->Lock()))
						{
							return 0;
						}
						
						pVertex = (CPyroParticleVertex *) m_pLibrary->GetVertexBuffer()->GetBuffer();
					}
					
					RenderParticle(pListItem->Particle, pVertex);
					pVertex += 4;
					
					if (++nBufferParticles == nBufferMaxParticles)
					{
						if (FAILED(hr = m_pLibrary->GetVertexBuffer()->Unlock()))
						{
							return 0;
						}
						
						m_pLibrary->GetGraphicsDevice()->RenderQuads(m_pLibrary->GetVertexBuffer(), 0, nBufferParticles, 0);
						nBufferParticles = 0;
					}
					
					nParticles ++;
				}
			}
			
			pListItem = pListItem->pNext;
			
		} while (pListItem);
	}
	else
	{
		if (MaxCount > 0 && !bUniformDistribution)
		{
			int Count = LayerParticles;
			if (Count > LayerMaxParticles)
				Count = LayerMaxParticles;
			while (Count > 0 && pListItem->pNext)
				pListItem = pListItem->pNext, Count --;
		}
		else
		{
			while (pListItem->pNext)
				pListItem = pListItem->pNext;
		}
		
		do
		{
			CHECK_MAX_PARTICLES
			{
				LayerRenderedParticles ++;
				
				if (pListItem->Particle.Visibility > 0.01f && pListItem->Particle.bRender)
				{
					if (nBufferParticles == 0)
					{
						if (FAILED(hr = m_pLibrary->GetVertexBuffer()->Lock()))
						{
							return 0;
						}
						
						pVertex = (CPyroParticleVertex *) m_pLibrary->GetVertexBuffer()->GetBuffer();
					}
					
					RenderParticle(pListItem->Particle, pVertex);
					pVertex += 4;
					
					if (++nBufferParticles == nBufferMaxParticles)
					{
						if (FAILED(hr = m_pLibrary->GetVertexBuffer()->Unlock()))
						{
							return 0;
						}
						
						m_pLibrary->GetGraphicsDevice()->RenderQuads(m_pLibrary->GetVertexBuffer(), 0, nBufferParticles, 0);
						nBufferParticles = 0;
					}
					
					nParticles ++;
				}
			}
			
			pListItem = pListItem->pPrev;
			
		} while (pListItem);
	}
	
	if (nBufferParticles)
	{
		if (FAILED(hr = m_pLibrary->GetVertexBuffer()->Unlock()))
		{
			return 0;
		}
		
		m_pLibrary->GetGraphicsDevice()->RenderQuads(m_pLibrary->GetVertexBuffer(), 0, nBufferParticles, 0);
	}
	
	return nParticles;
}

void PyroParticles::CPyroParticleLayer::UpdateParticleRenderingData()
{
	float tx0, tx1;
	if (m_bFlipX)
	{
		tx0 = 1.0f;
		tx1 = 0.0f;
	}
	else
	{
		tx0 = 0.0f;
		tx1 = 1.0f;
	}
	
	float ty0, ty1;
	
	if (m_bFlipY ^ GetEmitter()->IsVolumeEmitter())
	{
		ty0 = 1.0f;
		ty1 = 0.0f;
	}
	else
	{
		ty0 = 0.0f;
		ty1 = 1.0f;
	}
	
	m_ParticleRendering.tx0 = tx0;
	m_ParticleRendering.ty0 = ty0;
	
	m_ParticleRendering.tx1 = tx1;
	m_ParticleRendering.ty1 = ty1;
	
	float px, py;
	
	if (m_pShape)
	{
		m_ParticleRendering.sx = m_pShape->GetNormalizedWidth();
		m_ParticleRendering.sy = m_pShape->GetNormalizedHeight();
	}
	else
	{
		m_ParticleRendering.sx = m_ParticleRendering.sy = 64.0f;
		//px = py = 0.0f;
	}
	
	px = m_Pivot[0];
	py = m_Pivot[1];
	
	m_ParticleRendering.px = px;
	m_ParticleRendering.py = py;
}

void PyroParticles::CPyroParticleLayer::RenderParticle(CParticle &Particle, CPyroParticleVertex *pVertex)
{
	float r, g, b, a;
	
	const float Strength = Particle.TintStrength;
	
	if (Strength == 0.0f)
	{
		r = Particle.r;
		g = Particle.g;
		b = Particle.b;
		a = Particle.a;
	}
	else
        if (Strength == 1.0f)
        {
			r = Particle.TintColor.r;
			g = Particle.TintColor.g;
			b = Particle.TintColor.b;
			a = Particle.TintColor.a;
        }
        else
        {
			const float InvStrength = 1.0f - Strength;
			
			r = Particle.r * InvStrength + Particle.TintColor.r * Strength;
			g = Particle.g * InvStrength + Particle.TintColor.g * Strength;
			b = Particle.b * InvStrength + Particle.TintColor.b * Strength;
			a = Particle.a * InvStrength + Particle.TintColor.a * Strength;
        }
	
	const float Size = Particle.Size;
	
	float sx = m_ParticleRendering.sx * Size;
	float sy = m_ParticleRendering.sy * Size;
	
	float px = m_ParticleRendering.px;
	float py = m_ParticleRendering.py;
	
	float x, y, z;
	
	if (m_AttachAmount == 0.0f)
	{
		x = Particle.Position[0];
		y = Particle.Position[1];
		z = Particle.Position[2];
	}
	else
        if (m_AttachAmount == 1.0f)
        {
			x = m_pEmitter->m_Position[0] + Particle.RelativePosition[0];
			y = m_pEmitter->m_Position[1] + Particle.RelativePosition[1];
			z = m_pEmitter->m_Position[2] + Particle.RelativePosition[2];
        }
        else
        {
			x = (m_pEmitter->m_Position[0] + Particle.RelativePosition[0] - Particle.Position[0]) * m_AttachAmount + Particle.Position[0];
			y = (m_pEmitter->m_Position[1] + Particle.RelativePosition[1] - Particle.Position[1]) * m_AttachAmount + Particle.Position[1];
			z = (m_pEmitter->m_Position[2] + Particle.RelativePosition[2] - Particle.Position[2]) * m_AttachAmount + Particle.Position[2];
        }
	
	const float s = (float) sin(-Particle.Rotation);
	const float c = (float) cos(Particle.Rotation);
	
	const float xx = c *  sx;
	const float xy = s * -sx;
	
	float yx = s * sy;
	float yy = c * sy;
	
	x -= xx * px;
	y -= xy * px;
	
	x -= yx * py;
	y -= yy * py;
	
	const int ir = (int) (r * 255.0f + 0.5f);
	const int ig = (int) (g * 255.0f + 0.5f);
	const int ib = (int) (b * 255.0f + 0.5f);
	const int ia = (int) (a * Particle.Visibility * 255.0f + 0.5f);
	
	REUInt32 diffuse;
	
#ifdef __BIG_ENDIAN__
	if (m_pLibrary->GetGraphicsDevice()->IsRGBA())
		diffuse = (ir << 24) | (ig << 16) | (ib << 8) | ia;
	else    diffuse = (ib << 24) | (ig << 16) | (ir << 8) | ia;
#else
	if (m_pLibrary->GetGraphicsDevice()->IsRGBA())
		diffuse = ir | (ig << 8) | (ib << 16) | (ia << 24);
	else    diffuse = ib | (ig << 8) | (ir << 16) | (ia << 24);
#endif /* __BIG_ENDIAN__ */
	
	const float EmitterScale = m_pEmitter->GetScale();
	Geometry::CVector3 EmitterOffset = m_pEmitter->GetOffset();
	
	if (GetEmitter()->IsVolumeEmitter() && (m_OrientationType == PYRO_ORIENTATION_TYPE_ALIGN_TO_CAMERA || m_bNeedOrientationTransform))
	{
		/* FIXME: Add LocalMatrix multiplication */
		
		float xx, xy, xz;
		float yx, yy, yz;
		
		if (Particle.Rotation != 0.0f)
		{
			float Angle2 = Particle.Rotation * 0.5f;
			
			float w = (float) cos(Angle2);
			float sa = (float) sin(Angle2);
			
			float x  = m_ParticleTransform[2][0] * sa;
			float x2 = x + x;
			float xx2 = x2 * x;
			
			float y  = m_ParticleTransform[2][1] * sa;
			float y2 = y + y;
			float yy2 = y2 * y;
			
			float z  = m_ParticleTransform[2][2] * sa;
			float z2 = z + z;
			float zz2 = z2 * z;
			
			float wx2 = w * x2;
			float wy2 = w * y2;
			float wz2 = w * z2;
			
			float xy2 = x * y2;
			float xz2 = x * z2;
			float yz2 = z * y2;
			
			float SpinMatrix[3][3];
			
			SpinMatrix[0][0] = 1.0f - yy2 - zz2;
			SpinMatrix[1][1] = 1.0f - xx2 - zz2;
			SpinMatrix[2][2] = 1.0f - xx2 - yy2;
			
			SpinMatrix[0][1] = xy2 - wz2;
			SpinMatrix[1][0] = xy2 + wz2;
			
			SpinMatrix[0][2] = xz2 + wy2;
			SpinMatrix[2][0] = xz2 - wy2;
			
			SpinMatrix[1][2] = yz2 - wx2;
			SpinMatrix[2][1] = yz2 + wx2;
			
			xx = m_ParticleTransform[0][0] * SpinMatrix[0][0] +
			m_ParticleTransform[0][1] * SpinMatrix[1][0] +
			m_ParticleTransform[0][2] * SpinMatrix[2][0];
			
			xy = m_ParticleTransform[0][0] * SpinMatrix[0][1] +
			m_ParticleTransform[0][1] * SpinMatrix[1][1] +
			m_ParticleTransform[0][2] * SpinMatrix[2][1];
			
			xz = m_ParticleTransform[0][0] * SpinMatrix[0][2] +
			m_ParticleTransform[0][1] * SpinMatrix[1][2] +
			m_ParticleTransform[0][2] * SpinMatrix[2][2];
			
			yx = m_ParticleTransform[1][0] * SpinMatrix[0][0] +
			m_ParticleTransform[1][1] * SpinMatrix[1][0] +
			m_ParticleTransform[1][2] * SpinMatrix[2][0];
			
			yy = m_ParticleTransform[1][0] * SpinMatrix[0][1] +
			m_ParticleTransform[1][1] * SpinMatrix[1][1] +
			m_ParticleTransform[1][2] * SpinMatrix[2][1];
			
			yz = m_ParticleTransform[1][0] * SpinMatrix[0][2] +
			m_ParticleTransform[1][1] * SpinMatrix[1][2] +
			m_ParticleTransform[1][2] * SpinMatrix[2][2];
		}
		else
		{
			xx = m_ParticleTransform[0][0];
			xy = m_ParticleTransform[0][1];
			xz = m_ParticleTransform[0][2];
			
			yx = m_ParticleTransform[1][0];
			yy = m_ParticleTransform[1][1];
			yz = m_ParticleTransform[1][2];
		}
		
		xx *= sx;
		xy *= sx;
		xz *= sx;
		
		yx *= sy;
		yy *= sy;
		yz *= sy;
		
		x -= px * xx;
		y -= px * xy;
		z -= px * xz;
		
		x -= py * yx;
		y -= py * yy;
		z -= py * yz;
		
		const float xx_min_yx = xx - yx;
		const float xy_min_yy = xy - yy;
		
		pVertex[0].x       = (x - xx_min_yx) * EmitterScale + EmitterOffset.x;
		pVertex[0].y       = (y - xy_min_yy) * EmitterScale + EmitterOffset.y;
		pVertex[0].z       = (z - xz - yz) * EmitterScale + EmitterOffset.z;
		pVertex[0].tu      = m_ParticleRendering.tx0;
		pVertex[0].tv      = m_ParticleRendering.ty0;
		pVertex[0].diffuse = diffuse;
		pVertex[0].w       = 1.0f;
		
		pVertex[1].x       = (x + xx_min_yx) * EmitterScale + EmitterOffset.x;
		pVertex[1].y       = (y + xy_min_yy) * EmitterScale + EmitterOffset.y;
		pVertex[1].z       = (z + xz - yz) * EmitterScale + EmitterOffset.z;
		pVertex[1].tu      = m_ParticleRendering.tx1;
		pVertex[1].tv      = m_ParticleRendering.ty0;
		pVertex[1].diffuse = diffuse;
		pVertex[1].w       = 1.0f;
		
		pVertex[2].x       = (x + xx + yx) * EmitterScale + EmitterOffset.x;
		pVertex[2].y       = (y + xy + yy) * EmitterScale + EmitterOffset.y;
		pVertex[2].z       = (z + xz + yz) * EmitterScale + EmitterOffset.z;
		pVertex[2].tu      = m_ParticleRendering.tx1;
		pVertex[2].tv      = m_ParticleRendering.ty1;
		pVertex[2].diffuse = diffuse;
		pVertex[2].w       = 1.0f;
		
		pVertex[3].x       = (x - xx + yx) * EmitterScale + EmitterOffset.x;
		pVertex[3].y       = (y - xy + yy) * EmitterScale + EmitterOffset.y;
		pVertex[3].z       = (z - xz + yz) * EmitterScale + EmitterOffset.z;
		pVertex[3].tu      = m_ParticleRendering.tx0;
		pVertex[3].tv      = m_ParticleRendering.ty1;
		pVertex[3].diffuse = diffuse;
		pVertex[3].w       = 1.0f;
		
		if (Particle.pMesh)
		{
			// FIXME
			Engine::Geometry::CMatrix34 PrevWorldMatrix; //  = m_pLibrary->GetGraphics()->GetWorldMatrix();
			
			Engine::Geometry::CMatrix34 WorldMatrix = PrevWorldMatrix;
			
			Engine::Geometry::CAXYZ AXYZ(Particle.MeshRotationAngle,
										 Particle.MeshRotationAxis[0],
										 Particle.MeshRotationAxis[1],
										 Particle.MeshRotationAxis[2]);
			
			Engine::Geometry::CQuat Quat(AXYZ);
			
			Engine::Geometry::CMatrix34 RotMatrix = Quat.ToInvMatrix();
			
			float MeshScale = Particle.MeshScale;
			
			RotMatrix.ScaleCols(Particle.pMesh->m_NormalizationVector[0] * MeshScale,
								Particle.pMesh->m_NormalizationVector[1] * MeshScale,
								Particle.pMesh->m_NormalizationVector[2] * MeshScale, 1.0f);
			
			WorldMatrix.Translate(x, y, z);
			
			WorldMatrix = WorldMatrix * RotMatrix;
			
			m_pLibrary->GetGraphicsDevice()->SetWorldMatrix(WorldMatrix.m);
			
			RenderMesh(Particle.pMesh);
			
			m_pLibrary->GetGraphicsDevice()->SetWorldMatrix(PrevWorldMatrix.m);
		}
	}
	else
	{
		pVertex[0].tu      = m_ParticleRendering.tx0;
		pVertex[0].tv      = m_ParticleRendering.ty0;
		pVertex[0].diffuse = diffuse;
		pVertex[0].w       = 1.0f;
		
		pVertex[1].tu      = m_ParticleRendering.tx1;
		pVertex[1].tv      = m_ParticleRendering.ty0;
		pVertex[1].diffuse = diffuse;
		pVertex[1].w       = 1.0f;
		
		pVertex[2].tu      = m_ParticleRendering.tx1;
		pVertex[2].tv      = m_ParticleRendering.ty1;
		pVertex[2].diffuse = diffuse;
		pVertex[2].w       = 1.0f;
		
		pVertex[3].tu      = m_ParticleRendering.tx0;
		pVertex[3].tv      = m_ParticleRendering.ty1;
		pVertex[3].diffuse = diffuse;
		pVertex[3].w       = 1.0f;
		
		const float ParticleZ = z * EmitterScale + EmitterOffset.z;
		
		
		const float v0x = (x - xx - yx) * EmitterScale + EmitterOffset.x;
		const float v0y = (y - xy - yy) * EmitterScale + EmitterOffset.y;
		const float v0z = ParticleZ;
		
		const float v1x = (x + xx - yx) * EmitterScale + EmitterOffset.x;
		const float v1y = (y + xy - yy) * EmitterScale + EmitterOffset.y;
		const float v1z = ParticleZ;
		
		const float v2x = (x + xx + yx) * EmitterScale + EmitterOffset.x;
		const float v2y = (y + xy + yy) * EmitterScale + EmitterOffset.y;
		const float v2z = ParticleZ;
		
		const float v3x = (x - xx + yx) * EmitterScale + EmitterOffset.x;
		const float v3y = (y - xy + yy) * EmitterScale + EmitterOffset.y;
		const float v3z = ParticleZ;
		
		if (GetEmitter()->IsIdentityLocalMatrix())
		{
			pVertex[0].x = v0x;
			pVertex[0].y = v0y;
			pVertex[0].z = v0z;
			
			pVertex[1].x = v1x;
			pVertex[1].y = v1y;
			pVertex[1].z = v1z;
			
			pVertex[2].x = v2x;
			pVertex[2].y = v2y;
			pVertex[2].z = v2z;
			
			pVertex[3].x = v3x;
			pVertex[3].y = v3y;
			pVertex[3].z = v3z;
		}
		else
		{
			const float (*m)[4] = (const float(*)[4])GetEmitter()->GetLocalMatrix();
			
			pVertex[0].x = m[0][0] * v0x + m[0][1] * v0y + m[0][2] * v0z + m[0][3];
			pVertex[0].y = m[1][0] * v0x + m[1][1] * v0y + m[1][2] * v0z + m[1][3];
			pVertex[0].z = m[2][0] * v0x + m[2][1] * v0y + m[2][2] * v0z + m[2][3];
			
			pVertex[1].x = m[0][0] * v1x + m[0][1] * v1y + m[0][2] * v1z + m[0][3];
			pVertex[1].y = m[1][0] * v1x + m[1][1] * v1y + m[1][2] * v1z + m[1][3];
			pVertex[1].z = m[2][0] * v1x + m[2][1] * v1y + m[2][2] * v1z + m[2][3];
			
			pVertex[2].x = m[0][0] * v2x + m[0][1] * v2y + m[0][2] * v2z + m[0][3];
			pVertex[2].y = m[1][0] * v2x + m[1][1] * v2y + m[1][2] * v2z + m[1][3];
			pVertex[2].z = m[2][0] * v2x + m[2][1] * v2y + m[2][2] * v2z + m[2][3];
			
			pVertex[3].x = m[0][0] * v3x + m[0][1] * v3y + m[0][2] * v2z + m[0][3];
			pVertex[3].y = m[1][0] * v3x + m[1][1] * v3y + m[1][2] * v2z + m[1][3];
			pVertex[3].z = m[2][0] * v3x + m[2][1] * v3y + m[2][2] * v2z + m[2][3];
		}
		
		if (Particle.pMesh)
		{
			RenderMesh(Particle.pMesh);
		}
	}
	
}

void PyroParticles::CPyroParticleLayer::UpdateOrientationTransform()
{
	if (m_Orientation.Yaw == 0.0f && m_Orientation.Pitch == 0.0f && m_Orientation.Roll == 0.0f)
		m_bNeedOrientationTransform = false;
	else
	{
		m_bNeedOrientationTransform = true;
		
		float sx = (float) sin(m_Orientation.Yaw),   cx = (float) cos(m_Orientation.Yaw);
		float sy = (float) sin(m_Orientation.Pitch), cy = (float) cos(m_Orientation.Pitch);
		float sz = (float) sin(m_Orientation.Roll),  cz = (float) cos(m_Orientation.Roll);
		
		m_OrientationTransform[0][0] = cy * cz;
		m_OrientationTransform[1][0] = cy * sz;
		m_OrientationTransform[2][0] = -sy;
		
		m_OrientationTransform[0][1] = sx * sy * cz - cx * sz;
		m_OrientationTransform[1][1] = sx * sy * sz + cx * cz;
		m_OrientationTransform[2][1] = sx * cy;
		
		m_OrientationTransform[0][2] = cx * sy * cz + sx * sz;
		m_OrientationTransform[1][2] = cx * sy * sz - sx * cz;
		m_OrientationTransform[2][2] = cx * cy;
	}
}

void PyroParticles::CPyroParticleLayer::UpdateParticleAxes()
{
	CPyroParticleEmitter *pParentEmitter = GetEmitter();
	
	if (pParentEmitter->GetLayer())
		pParentEmitter = pParentEmitter->GetLayer()->GetEmitter();
	
	if (!m_bNeedOrientationTransform)
	{
		pParentEmitter->GetParticleTransform(m_ParticleTransform);
	}
	else
	{
		float Transform[3][3];
		
		if (m_OrientationType == PYRO_ORIENTATION_TYPE_ALIGN_TO_CAMERA)
		{
			pParentEmitter->GetParticleTransform(Transform);
			
			for (int i = 0; i < 3; i ++)
				for (int j = 0; j < 3; j ++)
					m_ParticleTransform[i][j] =
					m_OrientationTransform[i][0] * Transform[0][j] +
					m_OrientationTransform[i][1] * Transform[1][j] +
					m_OrientationTransform[i][2] * Transform[2][j];
		}
		else
			if (m_OrientationType == PYRO_ORIENTATION_TYPE_LOOKAT)
			{
				pParentEmitter->GetLookatTransform(Transform);
				
				for (int i = 0; i < 3; i ++)
					for (int j = 0; j < 3; j ++)
						m_ParticleTransform[i][j] =
						m_OrientationTransform[i][0] * Transform[0][j] +
						m_OrientationTransform[i][1] * Transform[1][j] +
						m_OrientationTransform[i][2] * Transform[2][j];
			}
			else
			{
				for (int i = 0; i < 3; i ++)
					for (int j = 0; j < 3; j ++)
						m_ParticleTransform[i][j] = m_OrientationTransform[i][j];
			}
	}
}

void PyroParticles::CPyroParticleLayer::Move(float Time, float TimeDelta)
{
	float Frame = Time * m_pEmitter->GetFPS();
	float FrameDelta = TimeDelta * m_pEmitter->GetFPS();
	CParticleListItem* pListItem = m_pListRoot;
	int nParticle = 0;
	
	while (pListItem)
	{
		CParticle *pParticle = &pListItem->Particle;
		
		float Position[3];
		
		float MotionRandX;
		float MotionRandY;
		float MotionRandZ;
		
		if (pParticle->MotionRand != 0.0f)
		{
			const float MotionRandFreq = 0.05f;
			
			float NormMotionRand = pParticle->MotionRand * 0.01f;
			
			float NoiseX = Frame * MotionRandFreq + pParticle->Seed;
			float NoiseY = NoiseX + 450.0f;
			
			float NoiseDelta = FrameDelta * MotionRandFreq;
			
			MotionRandX = Engine::Geometry::Noise::Noise11Int(NoiseX, NoiseX + NoiseDelta) * NormMotionRand;
			MotionRandY = Engine::Geometry::Noise::Noise11Int(NoiseY, NoiseY + NoiseDelta) * NormMotionRand;
			
			if (GetEmitter()->IsVolumeEmitter())
			{
				float NoiseZ = NoiseX + 940.0f;
				MotionRandZ = Engine::Geometry::Noise::Noise11Int(NoiseZ, NoiseZ + NoiseDelta) * NormMotionRand;
			}
			else
			{
				MotionRandZ = 0.0f;
			}
		}
		else
		{
			MotionRandX = MotionRandY = MotionRandZ = 0.0f;
		}
		
		float VelocityX = pParticle->Direction[0] * pParticle->Velocity;
		float VelocityY = pParticle->Direction[1] * pParticle->Velocity;
		float VelocityZ = pParticle->Direction[2] * pParticle->Velocity;
		
		VelocityX += m_pEmitter->m_AirVector[0] * pParticle->AirAmount;
		VelocityY += m_pEmitter->m_AirVector[1] * pParticle->AirAmount;
		VelocityZ += m_pEmitter->m_AirVector[2] * pParticle->AirAmount;
		
		if (!m_pEmitter->m_pCollisionObjectList)
		{
			float VelocityScale = pParticle->Zoom;
			
			Position[0] = pParticle->Position[0] + VelocityX * VelocityScale * (TimeDelta + MotionRandX);
			Position[1] = pParticle->Position[1] + VelocityY * VelocityScale * (TimeDelta + MotionRandY);
			Position[2] = pParticle->Position[2] + VelocityZ * VelocityScale * (TimeDelta + MotionRandZ);
		}
		else
		{
			float Pos[3];
			
			Pos[0] = pParticle->Position[0];
			Pos[1] = pParticle->Position[1];
			Pos[2] = pParticle->Position[2];
			
			float VelocityScale = pParticle->Zoom;
			
			float Vel[3];
			
			Vel[0] = VelocityX * VelocityScale;
			Vel[1] = VelocityY * VelocityScale;
			Vel[2] = VelocityZ * VelocityScale;
			
			float Dir[3];
			
			Dir[0] = pParticle->Direction[0];
			Dir[1] = pParticle->Direction[1];
			Dir[2] = pParticle->Direction[2];
			
			float ColTime = Time;
			float ColDelta = TimeDelta;
			
			bool bFoundCollisions;
			
			IPyroCollisionObject *pPrevCollisionObject = NULL;
			
			do
			{
				const CPyroParticleEmitter *pEmitter;
				
				float NearColPos[3];
				float NearColVel[3];
				float NearColDir[3];
				
				float MinColT;
				
				bFoundCollisions = false;
				
				IPyroCollisionObject *pNearCollisionObject = NULL;
				
				for (int p = 0; p < 2; p ++)
				{
					if (p == 0)
					{
						pEmitter = m_pEmitter;
					}
					else
					{
						pEmitter = m_pEmitter->m_pPrototype;
						
						if (pEmitter->m_pCollisionObjectList == m_pEmitter->m_pCollisionObjectList)
							break;
					}
					
					for (int i = 0; i < pEmitter->m_nCollisionObjects; i ++)
					{
						IPyroCollisionObject *pCollisionObject = pEmitter->m_pCollisionObjectList[i];
						
						if (pCollisionObject == pPrevCollisionObject)
							continue;
						
						float ColT = ColDelta;
						
						float _Pos[3], _Vel[3], _Dir[3];
						
						_Pos[0] = Pos[0], _Pos[1] = Pos[1], _Pos[2] = Pos[2];
						_Vel[0] = Vel[0], _Vel[1] = Vel[1], _Vel[2] = Vel[2];
						_Dir[0] = Dir[0], _Dir[1] = Dir[1], _Dir[2] = Dir[2];
						
						bool bCollide = pCollisionObject->CheckCollision(ColTime, _Pos, _Vel, _Dir, ColDelta,
																		 pParticle->Size, pParticle->Bounce,
																		 nParticle, &ColT, FALSE);
						
						if (bCollide)
						{
							if (!bFoundCollisions || MinColT > ColT)
								MinColT = ColT;
							
							NearColPos[0] = _Pos[0];
							NearColPos[1] = _Pos[1];
							NearColPos[2] = _Pos[2];
							
							NearColVel[0] = _Vel[0];
							NearColVel[1] = _Vel[1];
							NearColVel[2] = _Vel[2];
							
							NearColDir[0] = _Dir[0];
							NearColDir[1] = _Dir[1];
							NearColDir[2] = _Dir[2];
							
							pNearCollisionObject = pCollisionObject;
							
							bFoundCollisions = true;
						}
					}
					
				}
				
				if (bFoundCollisions)
				{
					Pos[0] = NearColPos[0];
					Pos[1] = NearColPos[1];
					Pos[2] = NearColPos[2];
					
					Vel[0] = NearColVel[0];
					Vel[1] = NearColVel[1];
					Vel[2] = NearColVel[2];
					
					Dir[0] = NearColDir[0];
					Dir[1] = NearColDir[1];
					Dir[2] = NearColDir[2];
					
					ColTime  += MinColT;
					ColDelta -= MinColT;
					
					if (ColDelta <= 0.0f)
						break;
				}
				
				pPrevCollisionObject = pNearCollisionObject;
				
			} while (bFoundCollisions);
			
			if (ColDelta > 0.0f)
			{
				Pos[0] += Vel[0] * ColDelta;
				Pos[1] += Vel[1] * ColDelta;
				Pos[2] += Vel[2] * ColDelta;
			}
			
			Position[0] = Pos[0];
			Position[1] = Pos[1];
			Position[2] = Pos[2];
			
			pParticle->Direction[0] = Dir[0];
			pParticle->Direction[1] = Dir[1];
			pParticle->Direction[2] = Dir[2];
		}
		
		if (pParticle->AngularVelocity != 0.0f || pParticle->Attraction != 0.0f)
		{
			float RelativePosition[2];
			RelativePosition[0] = Position[0] - pParticle->RotationPoint[0];
			RelativePosition[1] = Position[1] - pParticle->RotationPoint[1];
			
			if (pParticle->AngularVelocity != 0.0f)
			{
				float RotAngle = pParticle->AngularVelocity * TimeDelta;
				
				float sa = (float) sin(RotAngle);
				float ca = (float) cos(RotAngle);
				
				float _x            = RelativePosition[0] * ca - RelativePosition[1] * sa;
				RelativePosition[1] = RelativePosition[0] * sa + RelativePosition[1] * ca;
				RelativePosition[0] = _x;
				
				if (m_OrientationType != PYRO_ORIENTATION_TYPE_ALIGN_TO_CAMERA)
					pParticle->Rotation += RotAngle;
			}
			
			if (pParticle->Attraction != 0.0f)
			{
				float Scale = 1.0f - pParticle->Attraction * TimeDelta;
				
				RelativePosition[0] *= Scale;
				RelativePosition[1] *= Scale;
			}
			
			Position[0] = RelativePosition[0] + pParticle->RotationPoint[0];
			Position[1] = RelativePosition[1] + pParticle->RotationPoint[1];
		}
		
		pParticle->Position[0] = Position[0];
		pParticle->Position[1] = Position[1];
		pParticle->Position[2] = Position[2];
		
		pParticle->Rotation += pParticle->Spin * TimeDelta;
		
		pParticle->MeshRotationAngle += pParticle->MeshSpin * TimeDelta;
		
		if (pParticle->pSubEmitters)
		{
			for (int i = 0; i < pParticle->nSubEmitters; i ++)
				if (pParticle->pSubEmitters[i].pSubEmitter)
					pParticle->pSubEmitters[i].pSubEmitter->Move(Time - pParticle->pSubEmitters[i].StartTime, TimeDelta);
		}
		
		pListItem = pListItem->pNext;
		
		nParticle ++;
	}
}

int PyroParticles::CPyroParticleLayer::GetNumSubEmitterParticles() const
{
	int nParticles = 0;
	
	if (m_nSubEmitters)
	{
		CParticleListItem* pListItem = m_pListRoot;
		
		do
		{
			if (pListItem->Particle.pSubEmitters)
			{
				for (int i = 0; i < pListItem->Particle.nSubEmitters; i ++)
					nParticles += pListItem->Particle.pSubEmitters[i].pSubEmitter->GetNumParticles();
			}
			
			pListItem = pListItem->pNext;
			
		} while (pListItem);
	}
	
	return nParticles;
}

int PyroParticles::CPyroParticleLayer::GetNumParticles() const
{
	return m_LayerParticles + GetNumSubEmitterParticles();
}

int PyroParticles::CPyroParticleEmitter::GetNumParticles() const
{
	int nParticles = 0;
	for (int i = 0; i < m_nLayers; i ++)
		nParticles += m_LayerList[i].GetNumParticles();
	
	return nParticles;
}

void PyroParticles::CPyroParticleLayer::Prepare(float Time, float TimeDelta, PyroBool bEmitting) /* throw() */
{
	if (GetEmitter()->IsVolumeEmitter())
	{
		UpdateOrientationTransform();
		UpdateParticleAxes();
	}
	
	if (!m_pEmitter->IsActive() || (m_bSingleParticle && m_bEmitted))
	{
		PrepareParticles(Time, TimeDelta);
		return;
	}
	
	const float Frame = Time * m_pEmitter->GetFPS();
	
	const float Life = m_Life.GetValue(Frame) * m_pEmitter->GetLife();
	float Number = m_Number.GetValue(Frame) * m_pEmitter->GetNumber();
	const float Visibility = m_Visibility.GetValue(Frame) * m_pEmitter->GetVisibility();
	const float Bounce                   = m_Bounce.GetValue(Frame) * m_pEmitter->GetBounce();
	const float MeshSpin                 = m_MeshSpin.GetValue(Frame) * m_pEmitter->GetMeshSpin();
	const float Velocity                 = m_Velocity.GetValue(Frame) * m_pEmitter->GetVelocity();
	const float Size                     = m_Size.GetValue(Frame) * m_pEmitter->GetSize();
	const float MotionRand               = m_MotionRand.GetValue(Frame) * m_pEmitter->GetMotionRand();
	const float Weight                   = m_Weight.GetValue(Frame) * m_pEmitter->GetWeight();
	const float AngularVelocity          = m_AngularVelocity.GetValue(Frame) * m_pEmitter->GetAngularVelocity();
	const float Attraction               = m_Attraction.GetValue(Frame) * m_pEmitter->GetAttraction();
	const float AirAmount                = m_AirAmount.GetValue(Frame) * m_pEmitter->GetAirAmount();
	float Phi                      = m_Phi.GetValue(Frame);
	float Theta                    = m_Theta.GetValue(Frame);
	float LifeVariation            = m_LifeVariation.GetValue(Frame);
	float SizeVariation            = m_SizeVariation.GetValue(Frame);
	float NumberVariation          = m_NumberVariation.GetValue(Frame);
	float VelocityVariation        = m_VelocityVariation.GetValue(Frame);
	float MotionRandVariation      = m_MotionRandVariation.GetValue(Frame);
	float WeightVariation          = m_WeightVariation.GetValue(Frame);
	float SpinVariation            = m_SpinVariation.GetValue(Frame);
	float MeshSpinVariation        = m_MeshSpinVariation.GetValue(Frame);
	float BounceVariation          = m_BounceVariation.GetValue(Frame);
	float AirAmountVariation       = m_AirAmountVariation.GetValue(Frame);
	float AngularVelocityVariation = m_AngularVelocity.GetValue(Frame);
	float AttractionVariation      = m_Attraction.GetValue(Frame);
	
	float Spin = m_Spin.GetValue(Frame);
	if (m_bBidirectionalSpin && Spin != 0.0f)
	{
		if (Engine::CRandom::GetSingleton()->GetDWordRand() & 1)
			Spin = -Spin;
	}
	Spin *= m_pEmitter->GetSpin();
	
	float TintStrength = m_pEmitter->GetTintStrength();
	
	int iNumber;
	
	if (m_bSingleParticle)
	{
		m_bEmitted = true;
		iNumber = m_NumParticles;
	}
	else
	{
		Number = ApplyVariationPositive(Number, NumberVariation);
		Number *= TimeDelta;
		Number += m_PrevNumber;
		
		iNumber = (int) Number;
		m_PrevNumber = Number - (float) iNumber;
	}
	
	if (bEmitting)
	{
		for (int i = 0; i < iNumber; i ++)
		{
			for (int p = 0; p < m_pEmitter->GetNumEmitParticles(); p ++)
			{
				CParticleListItem *pListItem = CreateParticle();
				CParticle *pParticle = &pListItem->Particle;
				
				float x, y, z;
				float dx, dy, dz;
				float angle;
				
				m_pEmitter->CreateParticle(x, y, z, dx, dy, dz, angle, m_ParticleAngle, m_ParticleSpecAngle, p, Phi, Theta);
				
				if (m_pEmitter->GetParticleCreationCallback() != NULL)
				{
					CPyroCreatedParticle CreatedParticle;
					
					CreatedParticle.Position.x = x;
					CreatedParticle.Position.y = y;
					CreatedParticle.Position.z = z;
					
					CreatedParticle.Direction.x = dx;
					CreatedParticle.Direction.y = dy;
					CreatedParticle.Direction.z = dz;
					
					CreatedParticle.Spin = angle;
					
					m_pEmitter->GetParticleCreationCallback()(
															  &CreatedParticle,
															  m_pEmitter->GetParticleCreationCallbackUserData()
															  );
					
					x = CreatedParticle.Position.x;
					y = CreatedParticle.Position.y;
					z = CreatedParticle.Position.z;
					
					dx = CreatedParticle.Direction.x;
					dy = CreatedParticle.Direction.y;
					dz = CreatedParticle.Direction.z;
					
					angle = CreatedParticle.Spin;
				}
				
				m_LayerParticles ++;
				
				pParticle->bRender = true;
				pParticle->bColorChoosed = false;
				
				pParticle->nSubEmitters = 0;
				pParticle->pSubEmitters = NULL;
				
				pParticle->Position[0] = x;
				pParticle->Position[1] = y;
				pParticle->Position[2] = z;
				pParticle->RelativePosition[0] = x - m_pEmitter->m_Position[0];
				pParticle->RelativePosition[1] = y - m_pEmitter->m_Position[1];
				pParticle->RelativePosition[2] = z - m_pEmitter->m_Position[2];
				pParticle->RotationPoint[0] = m_pEmitter->m_Position[0];
				pParticle->RotationPoint[1] = m_pEmitter->m_Position[1];
				pParticle->RotationPoint[2] = m_pEmitter->m_Position[2];
				pParticle->Direction[0] = dx;
				pParticle->Direction[1] = dy;
				pParticle->Direction[2] = dz;
				pParticle->Rotation = angle;
				pParticle->Zoom = m_pEmitter->GetZoom();
				pParticle->Seed = (float) (rand() & 0x7fff) * 0.1432f;
				
				m_pEmitter->GetTintColor(pParticle->TintColor.r,
										 pParticle->TintColor.g,
										 pParticle->TintColor.b,
										 pParticle->TintColor.a);
				
				pParticle->TintStrength = TintStrength;
				
				pParticle->Life = ApplyVariationPositive(Life, LifeVariation);
				
				pParticle->BirthFrame           = Frame;
				
				if (m_bBidirectionalSpin)
					pParticle->BirthSpin = ApplyVariation(Spin, SpinVariation);
				else    pParticle->BirthSpin = ApplyVariationPositive(Spin, SpinVariation);
				
				pParticle->BirthMeshSpin        = ApplyVariationPositive(MeshSpin, MeshSpinVariation);
				pParticle->BirthVisibility      = Visibility;
				pParticle->BirthSize            = ApplyVariationPositive(Size, SizeVariation);
				pParticle->BirthVelocity        = ApplyVariationPositive(Velocity, VelocityVariation);
				pParticle->BirthMotionRand      = ApplyVariationPositive(MotionRand, MotionRandVariation);
				pParticle->BirthWeight          = ApplyVariationPositive(Weight, WeightVariation);
				pParticle->BirthAirAmount       = ApplyVariationPositive(AirAmount, AirAmountVariation);
				pParticle->BirthAngularVelocity = ApplyVariationPositive(AngularVelocity, AngularVelocityVariation);
				pParticle->BirthAttraction      = ApplyVariationPositive(Attraction, AttractionVariation);
				pParticle->BirthBounce          = ApplyVariationPositive(Bounce, BounceVariation);
				
				pParticle->pMesh = m_MeshSet.ChooseMesh(pParticle->MeshRotationAxis,
														pParticle->MeshRotationAngle,
														pParticle->MeshScale);
				
				pParticle->nSubEmitters = 0;
				pParticle->pSubEmitters = NULL;
				
				for (int i = 0; i < m_nSubEmitters; i ++)
				{
					CPyroParticleEmitter *pSubEmitter = GetSubEmitter(i);
					if (pSubEmitter->GetSubType() == PYRO_SUB_TYPE_ALL_LIFE)
						pParticle->AddSubEmitter(pSubEmitter->CloneEmitter(), Time);
				}
			}
		}
	}
	
	PrepareParticles(Time, TimeDelta);
}

float PyroParticles::CPyroParticleLayer::ApplyVariationPositiveInternal(float Value, float Variation) /* throw() */
{
	float Amount = Engine::CRandom::GetSingleton()->GetFloatRand11();
	
	if (Variation > 1.0f)
	{
		float S = Variation - 1.0f;
		return Value + Value * (Amount * Variation + S);
	}
	
	return Value + Value * Amount * Variation;
}

float PyroParticles::CPyroParticleLayer::ApplyVariationInternal(float Value, float Variation) /* throw() */
{
	if (Variation == 0.0f)
		return Value;
	
	if (Value < 0.0f)
		return -ApplyVariationPositive(-Value, Variation);
	
	return ApplyVariationPositive(Value, Variation);
}

void PyroParticles::CPyroParticleLayer::PrepareParticles(float Time, float TimeDelta) /* throw() */
{
	float Frame = Time * m_pEmitter->GetFPS();
	
	CParticleListItem* pListItem = m_pListRoot;
	
	while (pListItem)
	{
		CParticle *pParticle = &pListItem->Particle;
		CParticleListItem *pNextItem = pListItem->pNext;
		
		float ParticleFrame = (Frame - pParticle->BirthFrame);
		
		float LocalTime;
		
		if (m_bInfiniteLife)
		{
			LocalTime = 0.0f;
		}
		else
		{
			LocalTime = ParticleFrame / pParticle->Life;
		}
		
		bool bProcess = true;
		
		if (LocalTime >= 1.0f)
		{
			if (pParticle->bRender)
			{
				int i;
				
				for (i = 0; i < pParticle->nSubEmitters; i ++)
				{
					CPyroParticleEmitter *pSubEmitter = pParticle->pSubEmitters[i].pSubEmitter;
					if (pSubEmitter->GetSubType() == PYRO_SUB_TYPE_ALL_LIFE)
						pSubEmitter->SetActive(FALSE);
				}
				
				for (i = 0; i < m_nSubEmitters; i ++)
				{
					CPyroParticleEmitter *pSubEmitter = GetSubEmitter(i);
					if (pSubEmitter->GetSubType() == PYRO_SUB_TYPE_ON_DEATH)
						pParticle->AddSubEmitter(pSubEmitter->CloneEmitter(), Time);
				}
				
				pParticle->bRender = false;
			}
			else
			{
				bool bFound = false;
				
				for (int i = 0; i < pParticle->nSubEmitters; i ++)
				{
					CPyroParticleEmitter *pSubEmitter = pParticle->pSubEmitters[i].pSubEmitter;
					
					if (pSubEmitter)
					{
						float  LifeTime = Time - pParticle->pSubEmitters[i].StartTime;
						if (LifeTime < 0.1f || pSubEmitter->HasParticles())
						{
							bFound = true;
							break;
						}
					}
				}
				
				if (!bFound)
				{
					bProcess = false;
					KillParticle(pListItem);
					m_LayerParticles --;
				}
			}
		}
		
		if (bProcess)
		{
			float MotionRandOverLife      = m_MotionRandOverLife.GetValue(LocalTime);
			float SizeOverLife            = m_SizeOverLife.GetValue(LocalTime);
			float SpinOverLife            = m_SpinOverLife.GetValue(LocalTime);
			float MeshSpinOverLife        = m_MeshSpinOverLife.GetValue(LocalTime);
			float VelocityOverLife        = m_VelocityOverLife.GetValue(LocalTime);
			float WeightOverLife          = m_WeightOverLife.GetValue(LocalTime);
			float AirAmountOverLife       = m_AirAmountOverLife.GetValue(LocalTime);
			float VisibilityOverLife      = m_VisibilityOverLife.GetValue(LocalTime);
			float AngularVelocityOverLife = m_AngularVelocityOverLife.GetValue(LocalTime);
			float AttractionOverLife      = m_AttractionOverLife.GetValue(LocalTime);
			
			if (!m_ColorParam.GetChooseRandomColor() || !pParticle->bColorChoosed)
			{
				pParticle->bColorChoosed = true;
				m_ColorParam.GetValue(
									  m_ColorParam.GetChooseRandomColor() ? Engine::CRandom::GetSingleton()->GetFloatRand01() : (ParticleFrame / pParticle->Life),
									  pParticle->r, pParticle->g, pParticle->b, pParticle->a);
			}
			
			pParticle->Velocity        = pParticle->BirthVelocity        * VelocityOverLife;
			pParticle->Spin            = pParticle->BirthSpin            * SpinOverLife;
			pParticle->MeshSpin        = pParticle->BirthMeshSpin        * MeshSpinOverLife;
			pParticle->Size            = pParticle->BirthSize            * SizeOverLife;
			pParticle->MotionRand      = pParticle->BirthMotionRand      * MotionRandOverLife;
			pParticle->AirAmount       = pParticle->BirthAirAmount       * AirAmountOverLife;
			pParticle->Visibility      = pParticle->BirthVisibility      * VisibilityOverLife;
			pParticle->AngularVelocity = pParticle->BirthAngularVelocity * AngularVelocityOverLife;
			pParticle->Attraction      = pParticle->BirthAttraction      * AttractionOverLife;
			
			pParticle->Size *= pParticle->Zoom;
			
			float Weight = pParticle->BirthWeight * WeightOverLife;
			
			if (!GetEmitter()->IsVolumeEmitter())
				pParticle->Direction[1] += (Frame - pParticle->BirthFrame) * Weight;
			else    pParticle->Direction[2] -= (Frame - pParticle->BirthFrame) * Weight;
			
			for (int i = 0; i < pParticle->nSubEmitters; i ++)
			{
				CSubEmitter *pSubEmitter = pParticle->pSubEmitters + i;
				if (pSubEmitter->pSubEmitter)
					pSubEmitter->pSubEmitter->Prepare(Time - pSubEmitter->StartTime, TimeDelta,
													  pParticle->Position[0],
													  pParticle->Position[1],
													  pParticle->Position[2], TRUE);
			}
		}
		
		pListItem = pNextItem;
	}
}

void PyroParticles::CPyroParticleLayer::SetCameraMatrix(float m[3][4])
{
	CParticleListItem *pListItem = m_pListRoot;
	
	while (pListItem)
	{
		for (int i = 0; i < pListItem->Particle.nSubEmitters; i ++)
			if (pListItem->Particle.pSubEmitters[i].pSubEmitter)
				pListItem->Particle.pSubEmitters[i].pSubEmitter->SetCameraMatrix(m);
		
		pListItem = pListItem->pNext;
	}
}

void PyroParticles::CPyroParticleEmitter::SetLocalMatrix(const float m[3][4])
{
	bool bIdentity = true;
	
	for (int i = 0; i < 3; i ++)
	{
		for (int j = 0; j < 4; j ++)
		{
			m_LocalMatrix[i][j] = m[i][j];
			if (m[i][j] != (i == j ? 1.0f : 0.0f))
				bIdentity = false;
		}
	}
	
	m_bIdentityLocalMatrix = bIdentity;
}

void PyroParticles::CPyroParticleLayer::Reset()
{
	m_bEmitted = false;
	
	CParticleListItem *pListItem = m_pListRoot;
	
	while (pListItem)
	{
		CParticleListItem *pNextItem = pListItem->pNext;
		delete pListItem;
		
		pListItem = pNextItem;
	}
	
	m_pListRoot = NULL;
	
	m_PrevNumber = 0;
}

PyroParticles::CPyroParticleLayer::CParticleListItem* PyroParticles::CPyroParticleLayer::CreateParticle() /* throw() */
{
	if (!m_pListRoot)
	{
		m_pListRoot = CPyroCommon::CreateParticle();
		m_pListRoot->pPrev =
		m_pListRoot->pNext = NULL;
		
		return m_pListRoot;
	}
	
	CParticleListItem *pListItem = CPyroCommon::CreateParticle();
	pListItem->pPrev = NULL;
	pListItem->pNext = m_pListRoot;
	m_pListRoot->pPrev = pListItem;
	m_pListRoot = pListItem;
	
	return m_pListRoot;
}

void PyroParticles::CPyroParticleLayer::RenderMesh(CPyroParticleMesh *pMesh)
{
	/*PyroParticles::CPyroAse::CGeomObject *pGeomObject =*/ pMesh->GetGeomObject();
	//PyroParticles::CPyroAse::CMesh *pAseMesh = &pGeomObject->m_Mesh;
	//PyroParticles::CPyroAse::CVertex *pVertexList = pAseMesh->m_pVertexList;
	//PyroParticles::CPyroAse::CFace *pFace = pAseMesh->m_pFaceList;
	//PyroGraphics::IVertexBuffer *pVB = pAseMesh->m_pVB;
	//PyroGraphics::IIndexBuffer *pIB = pAseMesh->m_pIB;
	/*
	 PyroGraphics::ITexture *pDiffuseTexture =
	 (pGeomObject->m_pMaterial &&
	 pGeomObject->m_pMaterial->m_pDiffuseTexture &&
	 pGeomObject->m_pMaterial->m_pDiffuseTexture->m_pBitmap) ?
	 pGeomObject->m_pMaterial->m_pDiffuseTexture->m_pBitmap->m_pTexture : NULL;
	 */
	// FIXME
	/*
	 m_pLibrary->GetGraphics()->SetTexture(0, pDiffuseTexture);
	 m_pLibrary->GetGraphics()->gCullFace(G_NONE);
	 m_pLibrary->GetGraphics()->gRender(G_PR_TRIANGLES, pVB, 0, pAseMesh->m_nFaces, pIB);
	 m_pLibrary->GetGraphics()->gSetTexture(0, (IPyroGraphics::CTexture *) NULL);
	 */
}

void PyroParticles::CPyroParticleLayer::KillParticle(CParticleListItem *pListItem)
{
	CParticleListItem *pPrevItem = pListItem->pPrev;
	CParticleListItem *pNextItem = pListItem->pNext;
	
	if (pPrevItem)
		pPrevItem->pNext = pNextItem;
	
	if (pNextItem)
		pNextItem->pPrev = pPrevItem;
	
	if (!pPrevItem)
		m_pListRoot = pNextItem;
	
	if (pListItem->Particle.pSubEmitters)
	{
		for (int i = 0; i < pListItem->Particle.nSubEmitters; i ++)
			if (pListItem->Particle.pSubEmitters[i].pSubEmitter)
				delete pListItem->Particle.pSubEmitters[i].pSubEmitter;
		
		free(pListItem->Particle.pSubEmitters);
		pListItem->Particle.pSubEmitters = NULL;
		
		pListItem->Particle.nSubEmitters = 0;
	}
	
	CPyroCommon::KillParticle(pListItem);
}

void PyroParticles::CPyroParticleLayer::Serialize(Engine::CArchive &ar, int nVersion)
{
	bool bUseShape = ar.DeserializeBOOL();
	
	if (bUseShape)
	{
		int nID;
		ar >> nID;
		m_pShape = m_pPyroFile->FindShape(nID);
	}
	
	Engine::CString Name;
	ar >> Name;
	
	m_bIntense = ar.DeserializeBOOL();
	ar >> m_ParticleAngle;
	
	ar >> m_ParticleSpecAngle;
	
	m_bFlipX = ar.DeserializeBOOL();
	m_bFlipY = ar.DeserializeBOOL();
	
	REUInt32 Flags;
	ar >> Flags;
	
	m_bSingleParticle               = (Flags & PYRO_LAYER_FLAG_SINGLE_PARTICLE) > 0;
	m_bInfiniteLife                 = (Flags & PYRO_LAYER_FLAG_INFINITE_LIFE) > 0;
	m_bBidirectionalSpin    = (Flags & PYRO_LAYER_FLAG_BIDIRECTIONAL_SPIN) > 0;
	
    m_ColorParam.Serialize(ar, nVersion);
	
	m_Life.Serialize(ar, nVersion);
	m_Size.Serialize(ar, nVersion);
	m_Spin.Serialize(ar, nVersion);
	m_Velocity.Serialize(ar, nVersion);
	m_Number.Serialize(ar, nVersion);
	m_Visibility.Serialize(ar, nVersion);
	m_MotionRand.Serialize(ar, nVersion);
	m_LifeVariation.Serialize(ar, nVersion);
	m_SizeVariation.Serialize(ar, nVersion);
	m_NumberVariation.Serialize(ar, nVersion);
	m_MotionRandVariation.Serialize(ar, nVersion);
	m_VelocityVariation.Serialize(ar, nVersion);
	m_SizeOverLife.Serialize(ar, nVersion);
	m_MotionRandOverLife.Serialize(ar, nVersion);
	m_VelocityOverLife.Serialize(ar, nVersion);
	m_SpinOverLife.Serialize(ar, nVersion);
	
	m_Weight.Serialize(ar, nVersion);
	m_WeightOverLife.Serialize(ar, nVersion);
	m_WeightVariation.Serialize(ar, nVersion);
	
	m_AirAmount.Serialize(ar, nVersion);
	m_AirAmountVariation.Serialize(ar, nVersion);
	m_AirAmountOverLife.Serialize(ar, nVersion);
	m_VisibilityOverLife.Serialize(ar, nVersion);
	
	ar >> m_AttachAmount;
	m_bInvertOrder = ar.DeserializeBOOL();
	
	m_Bounce.Serialize(ar, nVersion);
	m_BounceVariation.Serialize(ar, nVersion);
	m_BounceOverLife.Serialize(ar, nVersion);
	
	ar >> m_nSubEmitters;
	
	if (m_nSubEmitters)
	{
		m_pSubEmitterList = new CPyroParticleEmitter[m_nSubEmitters];
		
		for (int i = 0; i < m_nSubEmitters; i ++)
		{
			m_pSubEmitterList[i].SetLibrary(m_pLibrary);
			m_pSubEmitterList[i].SetLayer(this);
			m_pSubEmitterList[i].SetPyroFile(m_pPyroFile);
			m_pSubEmitterList[i].Serialize(ar, nVersion);
		}
	}
	
	ar >> m_OrientationType;
	
	ar >> m_Orientation.Yaw;
	ar >> m_Orientation.Pitch;
	ar >> m_Orientation.Roll;
	
	m_Phi.Serialize(ar, nVersion);
	m_Theta.Serialize(ar, nVersion);
	
	m_AngularVelocity.Serialize(ar, nVersion);
	m_Attraction.Serialize(ar, nVersion);
	m_AngularVelocityVariation.Serialize(ar, nVersion);
	m_AttractionVariation.Serialize(ar, nVersion);
	m_AngularVelocityOverLife.Serialize(ar, nVersion);
	m_AttractionOverLife.Serialize(ar, nVersion);
	
	m_SpinVariation.Serialize(ar, nVersion);
	
	m_MeshSet.Serialize(ar, nVersion);
	
	m_bPremult = ar.DeserializeBOOL();
	ar >> m_NumParticles;
	
	m_MeshSpin.Serialize(ar, nVersion);
	m_MeshSpinVariation.Serialize(ar, nVersion);
	m_MeshSpinOverLife.Serialize(ar, nVersion);
	
	ar >> m_Pivot[0];
	ar >> m_Pivot[1];
}

PyroParticles::CPyroParticleEmitter *PyroParticles::CPyroParticleLayer::GetSubEmitter(int nSubEmitter)
{
	assert(nSubEmitter >= 0 && nSubEmitter < m_nSubEmitters);
	return m_pSubEmitterList + nSubEmitter;
}

void PyroParticles::CPyroParticleEmitter::AddCollisionObject(IPyroCollisionObject *pCollisionObject)
{
    if (m_bClone)
    {
		m_pCollisionObjectList = NULL;
		
		m_nCollisionObjects = 0;
		m_nMaxCollisionObjects = 0;
    }
	
	if (m_nCollisionObjects == m_nMaxCollisionObjects)
	{
		if (m_nMaxCollisionObjects == 0)
			m_nMaxCollisionObjects = 4;
		
		m_nMaxCollisionObjects *= 2;
		
		m_pCollisionObjectList = (IPyroCollisionObject **)
		realloc(m_pCollisionObjectList, sizeof(IPyroCollisionObject *) * m_nMaxCollisionObjects);
	}
	
	m_pCollisionObjectList[m_nCollisionObjects ++] = pCollisionObject;
}

int PyroParticles::CPyroParticleEmitter::Render(int MaxParticles, PyroBool bUniformDistribution)
{
	int nParticles = 0;
	
	for (int i = 0; i < m_nLayers; i++)
		nParticles += m_LayerList[i].Render(MaxParticles, bUniformDistribution);
	
	return nParticles;
}

int PyroParticles::CPyroParticleEmitter::Render()
{
	return Render(-1, PyroFalse);
}

void PyroParticles::CPyroParticleEmitter::Move(float Time, float TimeDelta)
{
	for (int i = 0; i < m_nLayers; i ++)
		m_LayerList[i].Move(Time, TimeDelta);
}

PyroBool PyroParticles::CPyroParticleEmitter::HasParticles() const
{
	for (int i = 0; i < m_nLayers; i ++)
		if (m_LayerList[i].HasParticles())
			return PyroTrue;
	
	return PyroFalse;
}

void PyroParticles::CPyroParticleEmitter::Prepare(float Time, float TimeDelta, float x, float y, float z, PyroBool bEmitting) /* throw() */
{
	PrepareInternal(Time, TimeDelta, x, y, z, bEmitting);
}

void PyroParticles::CPyroParticleEmitter::PrepareInternal(float Time, float TimeDelta, float x, float y, float z, PyroBool bEmitting) /* throw() */
{
	m_Position[0] = x;
	m_Position[1] = y;
	m_Position[2] = z;
	
	CreateLookatMatrix(m_LookatDirection);
	
	float Frame = Time * GetFPS();
	
	m_vLife            = m_Life.GetValue(Frame);
	m_vNumber          = m_Number.GetValue(Frame);
	m_vVisibility      = m_Visibility.GetValue(Frame);
	m_vSpin            = m_Spin.GetValue(Frame);
	m_vMeshSpin        = m_MeshSpin.GetValue(Frame);
	m_vVelocity        = m_Velocity.GetValue(Frame);
	m_vSize            = m_Size.GetValue(Frame);
	m_vMotionRand      = m_MotionRand.GetValue(Frame);
	m_vWeight          = m_Weight.GetValue(Frame);
	m_vEmissionAngle   = m_EmissionAngle.GetValue(Frame);
	m_vEmissionRange   = m_EmissionRange.GetValue(Frame);
	m_vAngle           = m_Angle.GetValue(Frame);
	m_vZoom            = m_Zoom.GetValue(Frame);
	m_vRadius          = m_Radius.GetValue(Frame);
	m_vTintStrength    = m_TintStrength.GetValue(Frame);
	m_vAirAmount       = m_AirAmount.GetValue(Frame);
	m_vAngularVelocity = m_AngularVelocity.GetValue(Frame);
	m_vAttraction      = m_Attraction.GetValue(Frame);
	m_vBounce          = m_Bounce.GetValue(Frame);
	
	for (int i = 0; i < m_nLayers; i ++)
		m_LayerList[i].Prepare(Time, TimeDelta, bEmitting);
}

void PyroParticles::CPyroParticleEmitter::Prepare(float Time, float TimeDelta) /* throw() */
{
	Prepare(Time, TimeDelta, m_Position[0], m_Position[1], m_Position[2], m_bEmitting);
}

void PyroParticles::CPyroParticleEmitter::GetParticleTransform(float m[3][3])
{
	for (int i = 0; i < 3; i ++)
		for (int j = 0; j < 3; j ++)
			m[i][j] = m_ParticleTransform[i][j];
}

void PyroParticles::CPyroParticleEmitter::GetLookatTransform(float m[3][3])
{
	for (int i = 0; i < 3; i ++)
		for (int j = 0; j < 3; j ++)
			m[i][j] = m_LookatMatrix[i][j];
}

void PyroParticles::CPyroParticleEmitter::SetCameraMatrix(float m[3][4])
{
	int i;
	
	for (i = 0; i < 3; i ++)
		for (int j = 0; j < 3; j ++)
			m_ParticleTransform[i][j] = m[i][j];
	
	for (i = 0; i < m_nLayers; i ++)
	{
		CPyroParticleLayer &Layer = m_LayerList[i];
		Layer.SetCameraMatrix(m);
	}
}

void PyroParticles::CPyroParticleEmitter::SetLookatDirection(float x, float y, float z)
{
	m_LookatDirection[0] = x;
	m_LookatDirection[1] = y;
	m_LookatDirection[2] = z;
}

void PyroParticles::CPyroParticleEmitter::CreateLookatMatrix(float Delta[3])
{
	float DeltaLength = (float) sqrt(Delta[0] * Delta[0] +
									 Delta[1] * Delta[1] +
									 Delta[2] * Delta[2]);
	
	if ((float) fabs(DeltaLength) < 0.0001f)
	{
		Delta[0] = 1.0f;
		Delta[1] = 0.0f;
		Delta[2] = 0.0f;
		
		DeltaLength = 1.0f;
	}
	
	float ang_x = (float) -atan2(Delta[0], Delta[1]);
    float ang_y = (float) asin(Delta[2] / DeltaLength);
	
	float sx, cx;
	float sy, cy;
	float sz, cz;
	
	float roll = 0.0f;
	
    sx = (float) sin(ang_x); cx = (float) cos(ang_x);
    sy = (float) sin(ang_y); cy = (float) cos(ang_y);
    sz = (float) sin(roll ); cz = (float) cos(roll );
	
	float sycz = sy * cz;
	
    m_LookatMatrix[0][0] = sx * sy * sz + cx * cz;
    m_LookatMatrix[1][0] = sx * sycz - cx * sz;
    m_LookatMatrix[2][0] = sx * cy;
	
    m_LookatMatrix[0][1] = sx * cz - cx * sy * sz;
    m_LookatMatrix[1][1] = -cx * sycz - sx * sz;
    m_LookatMatrix[2][1] = -cx * cy;
	
    m_LookatMatrix[0][2] = cy * sz;
    m_LookatMatrix[1][2] = cy * cz;
    m_LookatMatrix[2][2] = -sy;
}

void PyroParticles::CPyroParticleEmitter::CreateParticle(
														 float &x, float &y, float &z,
														 float &dx, float &dy, float &dz,
														 float &angle, int ParticleAngle,
														 float ParticleSpecAngle, int nParticle,
														 float Phi, float Theta) /* throw() */
{
	float dangle;
	bool bNegDir;
	
	if (m_bEmissionIn && m_bEmissionOut)
	{
		if (m_bEmitAtPoints)
		{
			bNegDir = nParticle & 1;
			nParticle /= 2;
		}
		else
		{
			bNegDir = Engine::CRandom::GetSingleton()->GetDWordRand() & 1;
		}
	}
	else
	{
		bNegDir = m_bEmissionIn;
	}
	
	angle = Engine::CRandom::GetSingleton()->GetFloatRand11() * m_vEmissionRange * 0.5f + m_vEmissionAngle;
	
	if (m_Shape == 0)
	{
		x = m_Position[0];
		y = m_Position[1];
		z = m_Position[2];
	}
	else
        if (m_Shape == 1)
        {
			float p;
			
			if (m_bEmitAtPoints)
			{
				p = (m_nEmitPoints > 1) ?
				((float) nParticle / (m_nEmitPoints - 1)) : 0.0f;
			}
			else
			{
				p = Engine::CRandom::GetSingleton()->GetFloatRand01();
			}
			
			p = (p - 0.5f) * 2.0f;
			
			x = m_Position[0] + (float) cos(m_vAngle) * m_vRadius * p;
			y = m_Position[1] + (float) sin(m_vAngle) * m_vRadius * p;
			z = m_Position[2];

			angle += m_vAngle + MATH_PI_DIV_2; //3.1415f * 0.5f;
        }
        else
        {
			float p;
			
			if (m_bEmitAtPoints)
			{
				p = (float) nParticle / m_nEmitPoints;
			}
			else
			{
				p = Engine::CRandom::GetSingleton()->GetFloatRand01();
			}
			
			p *= MATH_2_MUL_PI; //3.1415f * 2.0f;
			
			float Angle = p + m_vAngle;
			x = m_Position[0] + (float) cos(Angle) * m_vRadius;
			y = m_Position[1] + (float) sin(Angle) * m_vRadius;
			z = m_Position[2];
			
			angle += Angle;
        }
	
	if (ParticleAngle == 2)
	{
		dangle = angle;
		angle = ParticleSpecAngle * MATH_PI_DIV_180; //0.0174533f;
	}
	else
	{
		if (ParticleAngle == 0)
		{
			dangle = angle;
			angle += ParticleSpecAngle * MATH_PI_DIV_180; //0.0174533f;
		}
		else
		{
			dangle = angle;
			angle = Engine::CRandom::GetSingleton()->GetFloatRandAngle();
		}
	}
	
	if (IsVolumeEmitter())
	{
		float MinAngle = Phi;
		float MaxAngle = Theta;
		
		if (MinAngle > MaxAngle)
		{
			float t = MinAngle;
			MinAngle = MaxAngle;
			MaxAngle = t;
		}
		
		float dangle2 = Engine::CRandom::GetSingleton()->GetFloatRand01() * (MaxAngle - MinAngle) + MinAngle;
		
		dangle2 = MATH_PI /*3.1415f*/ - dangle2;
		
		const float sinDangle2 = (float)sin(dangle2);
		dx = (float) cos(dangle) * sinDangle2;
		dy = (float) sin(dangle) * sinDangle2;
		dz = (float) -cos(dangle2);
	}
	else
	{
		dx = (float) cos(dangle);
		dy = (float) sin(dangle);
		dz = 0.0f;
	}
	
	if (bNegDir)
	{
		dx = -dx;
		dy = -dy;
		dz = -dz;
	}
}

void PyroParticles::CPyroParticleEmitter::Serialize(Engine::CArchive &ar, int nVersion)
{
	m_Life.Serialize(ar , nVersion);
	m_Size.Serialize(ar, nVersion);
	m_Number.Serialize(ar, nVersion);
	m_Spin.Serialize(ar, nVersion);
	m_Velocity.Serialize(ar, nVersion);
	m_Visibility.Serialize(ar, nVersion);
	m_MotionRand.Serialize(ar, nVersion);
	m_EmissionAngle.Serialize(ar, nVersion);
	m_EmissionRange.Serialize(ar, nVersion);
	m_Weight.Serialize(ar, nVersion);
	m_Zoom.Serialize(ar, nVersion);
	m_Angle.Serialize(ar, nVersion);
	m_Radius.Serialize(ar, nVersion);
	m_TintStrength.Serialize(ar, nVersion);
	m_AirAmount.Serialize(ar, nVersion);
	
	ar >> m_AirVector[0];
	ar >> m_AirVector[1];
	
	m_AirVector[2] = 0.0f;
	
	m_Bounce.Serialize(ar, nVersion);
	
	ar >> m_nLayers;
	
	if (m_nLayers)
	{
		m_LayerList = new CPyroParticleLayer[m_nLayers];
		
		for (int i = 0; i < m_nLayers; i ++)
		{
			m_LayerList[i].SetLibrary(m_pLibrary);
			m_LayerList[i].SetPyroFile(m_pPyroFile);
			m_LayerList[i].SetMeshes(m_pPyroFile->GetMeshes());
			m_LayerList[i].SetEmitter(this);
			m_LayerList[i].Serialize(ar, nVersion);
		}
	}
	
	Engine::CString Name;
	ar >> Name;
	
	m_pName = Name.GetBuffer() ? strdup(Name.GetBuffer()) : NULL;
	
	ar >> m_TintColor.r;
	ar >> m_TintColor.g;
	ar >> m_TintColor.b;
	ar >> m_TintColor.a;
	
	m_bEmitAtPoints = ar.DeserializeBOOL();
	ar >> m_nEmitPoints;
	
	m_bEmissionIn = ar.DeserializeBOOL();
	m_bEmissionOut = ar.DeserializeBOOL();
	
	ar >> m_Shape;
	
	m_bVolumeEmitter = ar.DeserializeBOOL();
	
	m_AngularVelocity.Serialize(ar, nVersion);
	m_Attraction.Serialize(ar, nVersion);
	
	m_MeshSpin.Serialize(ar, nVersion);
	
	if (nVersion >= 0x00016000)
	{
		ar >> m_SubType;
	}
	
	if (nVersion >= 0x000024000)
	{
		ar >> m_ContainerRadius;
	}
}

PyroParticles::CPyroParticleShape::CPyroParticleShape()
{
	m_pFile = NULL;
	m_nFileBytes = 0;
	m_pBitmap = NULL;
	
	m_pTexture = NULL;
}

PyroParticles::CPyroParticleShape::~CPyroParticleShape()
{
	SAFE_DELETE_ARRAY(m_pFile);
}

void PyroParticles::CPyroParticleShape::InvalidateTexture()
{
	m_pTexture->Invalidate();
}

bool PyroParticles::CPyroParticleShape::CreateTexture()
{
	if (m_pTexture)
		return true;
	
	CMemoryFile MemFile(m_pFile, m_nFileBytes);
	CBitmapFile BitmapFile(MemFile);
	
	CBitmapIO BitmapIO;
	
	if (!BitmapIO.Load(BitmapFile))
		return false;
	
	HRESULT hr = m_pLibrary->GetGraphicsDevice()->CreateBitmap(&m_pBitmap, (BYTE *) BitmapIO.GetImage(), BitmapIO.GetWidth(), BitmapIO.GetHeight(), 0);
	if (FAILED(hr))
	{
		return false;
	}
	
	hr = m_pLibrary->GetGraphicsDevice()->CreateTexture(&m_pTexture, m_pBitmap, 0);
	if (FAILED(hr))
	{
		return false;
	}
	
	BitmapIO.SetImage(NULL);
	
	if (m_pBitmap && m_pBitmap->GetBuffer())
		delete m_pBitmap->GetBuffer();
	
	SAFE_DELETE(m_pBitmap);
	
	return true;
}

void PyroParticles::CPyroParticleShape::DestroyTexture()
{
	SAFE_DELETE(m_pTexture);
}

void PyroParticles::CPyroParticleShape::Serialize(Engine::CArchive &ar, int nVersion)
{
	int nFrames;
	ar >> nFrames;
	
	Engine::CString Name;
	ar >> Name;
	
	ar >> m_nID;
	
	for (int nFrame = 0; nFrame < nFrames; nFrame ++)
	{
		int nFileBytes;
		ar >> nFileBytes;
		
		if (nFileBytes)
		{
			if (nFrame > 0)
			{
				ar.Skip(nFileBytes);
			}
			else
			{
				m_pFile = new BYTE[nFileBytes];
				ar.Read(m_pFile, nFileBytes);
			}
		}
		
		float NormalizedWidth;
		float NormalizedHeight;
		ar >> NormalizedWidth;
		ar >> NormalizedHeight;
		
		if (nFrame == 0)
		{
			m_NormalizedWidth = NormalizedWidth;
			m_NormalizedHeight = NormalizedHeight;
			m_nFileBytes = nFileBytes;
		}
	}
	
	int nTimes;
	ar >> nTimes;
	
	float dFps;
	ar >> dFps;
	bool bReverse;
	ar >> bReverse;
	bool bPingPong;
	ar >> bPingPong;
}

PyroParticles::CPyroParticleShape *PyroParticles::CPyroFile::FindShape(int nID)
{
	for (int i = 0; i < m_nShapes; i ++)
		if (m_ShapeList[i].GetID() == nID)
			return m_ShapeList + i;
	
	return NULL;
}

void PyroParticles::CPyroParticleLibrary::Init(PyroParticles::PyroGraphics::IDevice *pGraphicsDevice)
{
	// CTimer::GetSingleton()->Init();
	
#ifdef PYROLIB_COMPILATION
	CFileManager::GetSingleton()->SetLocalMode();
#endif /* PYROLIB_COMPILATION */
	
	m_pGraphicsDevice = pGraphicsDevice;
	
	m_VertexBufferMaxQuads = 512;
	
	HRESULT hr = m_pGraphicsDevice->CreateVertexBuffer(
													   &m_pVB, 4 * m_VertexBufferMaxQuads, sizeof(CPyroParticleVertex),
													   PyroGraphics::IVertexBuffer::VF_POSITION |
													   PyroGraphics::IVertexBuffer::VF_RHW |
													   PyroGraphics::IVertexBuffer::VF_DIFFUSE |
													   PyroGraphics::IVertexBuffer::VF_TEXTURE0, 0);
	
	if (FAILED(hr))
	{
		return;
	}
}

void PyroParticles::CPyroParticleLibrary::Done()
{
	// CTimer::GetSingleton()->Done();
	
	SAFE_DELETE(m_pVB);
}

void PyroParticles::CPyroFile::InvalidateTextures()
{
	for (int i = 0; i < m_nShapes; i ++)
		m_ShapeList[i].InvalidateTexture();
}

void PyroParticles::CPyroFile::CreateTextures()
{
	for (int i = 0; i < m_nShapes; i ++)
		m_ShapeList[i].CreateTexture();
	
	m_Meshes.CreateTextures(m_pLibrary);
}

void PyroParticles::CPyroFile::DestroyTextures()
{
	for (int i = 0; i < m_nShapes; i ++)
		m_ShapeList[i].DestroyTexture();
}

PyroParticles::CPyroParticleLibrary::CPyroParticleLibrary()
{
	m_pGraphicsDevice = NULL;
	m_pVB = NULL;
}

PyroParticles::CPyroParticleLibrary::~CPyroParticleLibrary()
{
	Done();
}

PyroParticles::IPyroParticleEmitter *PyroParticles::CPyroFile::FindEmitter(const char *pName)
{
	for (int i = 0; i < m_nEmitters; i ++)
		if (strcmp(m_EmitterList[i].GetName(), pName) == 0)
			return m_EmitterList + i;
	
	return NULL;
}

PyroParticles::IPyroParticleEmitter *PyroParticles::CPyroFile::CreateEmitter(const char *pName)
{
	CPyroParticleEmitter *pOrgEmitter = (CPyroParticleEmitter *) FindEmitter(pName);
	if (!pOrgEmitter)
	{
		return 0;
	}
	
	CPyroParticleEmitter *pEmitter = new CPyroParticleEmitter(*pOrgEmitter);
	
	return pEmitter;
}

PyroParticles::CPyroParticleEmitter *PyroParticles::CPyroFile::CreateEmitter(const char *pName, float CreationTime)
{
	CPyroParticleEmitter *pOrgEmitter = (CPyroParticleEmitter *) FindEmitter(pName);
	if (!pOrgEmitter)
	{
		return 0;
	}
	
	CPyroParticleEmitter *pEmitter = new CPyroParticleEmitter(*pOrgEmitter);
	
	return pEmitter;
}

int PyroParticles::CPyroParticleEmitter::GetNumEmitParticles()
{
	if (m_Shape != 0 && m_bEmitAtPoints)
	{
		if (m_bEmissionIn && m_bEmissionOut)
			return m_nEmitPoints * 2;
		
		return m_nEmitPoints;
	}
	
	return 1;
}

PyroParticles::CPyroFile::CPyroFile(CPyroParticleLibrary *pLibrary)
{
	m_pLibrary = pLibrary;
	
	m_nEmitters = 0;
	m_EmitterList = NULL;
	
	m_nShapes = 0;
	m_ShapeList = NULL;
}

PyroParticles::CPyroFile::~CPyroFile()
{
	if (m_ShapeList)
		delete[] m_ShapeList;
	
	if (m_EmitterList)
		delete[] m_EmitterList;
}

void PyroParticles::CPyroFile::Serialize(Engine::CArchive &ar)
{
	int ID;
	ar >> ID;
	
	if (ID != PYRO_FILE_ID)
	{
		return;
	}
	
	int nVersion;
	ar >> nVersion;
	
	if (nVersion < PYRO_FILE_VERSION)
	{
		return;
	}
	
	if (nVersion > PYRO_FILE_VERSION)
	{
		return;
	}
	
	m_Meshes.Serialize(ar, nVersion);
	
    ar >> m_nShapes;
	
    if (m_nShapes)
    {
		m_ShapeList = new CPyroParticleShape[m_nShapes];
		
		for (int i = 0; i < m_nShapes; i ++)
		{
			m_ShapeList[i].SetLibrary(m_pLibrary);
			m_ShapeList[i].Serialize(ar, nVersion);
		}
	}
	
	ar >> m_nEmitters;
	
	m_EmitterList = new CPyroParticleEmitter[m_nEmitters];
	
	for (int i = 0; i < m_nEmitters; i ++)
	{
		m_EmitterList[i].SetLibrary(m_pLibrary);
		m_EmitterList[i].SetPyroFile(this);
		m_EmitterList[i].Serialize(ar, nVersion);
	}
	
	m_Meshes.CreateVertexAndIndexBuffers(m_pLibrary->GetGraphicsDevice());
	
	if (nVersion >= 0x00016000)
	{
		int nDeflectors;
		ar >> nDeflectors;
	}
	
	if (nVersion >= 0x00023000)
	{
		REUInt32 Signature0[4];
		REUInt32 Signature1[4];
		REUInt32 Signature2[4];
		
		for (int i = 0; i < 4; i ++)
			ar >> Signature0[i];
		
		for (int i = 0; i < 4; i ++)
			ar >> Signature1[i];
		
		for (int i = 0; i < 4; i ++)
			ar >> Signature2[i];
	}
}

int PyroParticles::CPyroFile::GetFileEmitters() const
{
	return m_nEmitters;
}

const PyroParticles::IPyroParticleEmitter *PyroParticles::CPyroFile::GetFileEmitter(int nEmitter)
{
	if (nEmitter < 0 || nEmitter >= m_nEmitters)
		return NULL;
	
	return &m_EmitterList[nEmitter];
}

PyroParticles::CPyroParticleMesh *PyroParticles::CPyroParticleLayerMeshSet::ChooseMesh(float Axis[3], float &Angle, float &Scale)
{
	if (m_nMeshes == 0)
	{
		Axis[0] = 0.0f;
		Axis[1] = 0.0f;
		Axis[2] = 1.0f;
		
		Angle = 0.0f;
		Scale = 1.0f;
		
		return NULL;
	}
	
	CLayerMesh *pLayerMesh;
	
	if (m_nMeshes == 1)
	{
		pLayerMesh = m_pMeshList;
	}
	else
        if (m_SumProbability == 0)
        {
			REUInt32 nMesh = Engine::CRandom::GetSingleton()->GetDWordRand() % ((REUInt32) m_nMeshes);
			pLayerMesh = m_pMeshList + nMesh;
        }
        else
        {
			REUInt32 RandValue = Engine::CRandom::GetSingleton()->GetDWordRand();
			RandValue %= (REUInt32) m_SumProbability;
			
			pLayerMesh = m_pMeshList + (m_nMeshes - 1);
			for (int i = 0; i < m_nMeshes - 1; i ++)
			{
				if (RandValue < (REUInt32) m_pMeshList[i].m_Probability)
				{
					pLayerMesh = m_pMeshList + i;
					break;
				}
				
				RandValue -= m_pMeshList[i].m_Probability;
			}
        }
	
	CPyroParticleMesh *pMesh = pLayerMesh->m_pMesh;
	
	Scale = pLayerMesh->m_Scale;
	
	if (pLayerMesh->m_bRandomAxis)
	{
		float Length;
		
		do
		{
			Axis[0] = Engine::CRandom::GetSingleton()->GetFloatRand11();
			Axis[1] = Engine::CRandom::GetSingleton()->GetFloatRand11();
			Axis[2] = Engine::CRandom::GetSingleton()->GetFloatRand11();
			
			Length = Axis[0] * Axis[0] +
			Axis[1] * Axis[1] +
			Axis[2] * Axis[2];
			
		} while (Length < 0.1f);
		
		const float InvLength = 1.0f / (float) sqrt(Length);
		
		Axis[0] *= InvLength;
		Axis[1] *= InvLength;
		Axis[2] *= InvLength;
		
		Angle = Engine::CRandom::GetSingleton()->GetFloatRandAngle();
	}
	else
	{
		Axis[0] = 0.0f;
		Axis[1] = 0.0f;
		Axis[2] = 1.0f;
		
		Angle = 0.0f;
	}
	
	return pMesh;
}

PyroParticles::CPyroParticleLayer::CParticleListItem *PyroParticles::CPyroCommon::m_pFreeParticleList = NULL;

PyroParticles::IPyroFile *PyroParticles::CPyroParticleLibrary::LoadPyroFile(IPyroFileIO *pFileIO)
{
	CPyroFileIO PyroFileIO(pFileIO);
	
	CPyroFile * pFile = new CPyroFile(this);
	if (pFile) 
	{	
		Engine::CArchive ar(&PyroFileIO);
		pFile->Serialize(ar);
		
		if (PyroFileIO.Tell() != PyroFileIO.GetSize())
		{
			delete pFile;
			pFile = 0;
		}
	}
	
	return pFile;
}

PyroParticles::IPyroFile *PyroParticles::CPyroParticleLibrary::LoadPyroFile(const char *pFileName)
{
    Engine::CFile File;
	
	if (!File.Open(pFileName))
	{
		return 0;
	}
	
	CPyroFile * pFile = new CPyroFile(this);
	
	if (pFile) 
	{
		Engine::CArchive ar(&File);
		pFile->Serialize(ar);
		
		if (File.Tell() != File.GetSize())
		{
			delete pFile;
			pFile = 0;
		}
	}
	return pFile;
}

PyroParticles::PyroGraphics::IVertexBuffer *PyroParticles::CPyroParticleLibrary::GetVertexBuffer() const
{
	return m_pVB;
}

int PyroParticles::CPyroParticleLibrary::GetVertexBufferMaxQuads() const
{
	return m_VertexBufferMaxQuads;
}

PyroParticles::PyroGraphics::IDevice *PyroParticles::CPyroParticleLibrary::GetGraphicsDevice() const
{
	return m_pGraphicsDevice;
}

PyroParticles::IPyroParticleLibrary *CreateParticleLibrary(PyroDWord Version)
{
	/*
	 #if defined(_WIN32) && !defined(ENGINE_COMPILATION)
	 char Path[MAX_PATH];
	 GetModuleFileName(GetModuleHandle("Pyro.dll"), Path, sizeof(Path));
	 
	 DWORD VerHnd;
	 DWORD VerInfoSize = GetFileVersionInfoSize(Path, &VerHnd);
	 DWORD DllVersion = 0;
	 
	 if (VerInfoSize)
	 {
	 HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, VerInfoSize);
	 LPSTR StrVffInfo = (LPSTR) GlobalLock(hMem);
	 GetFileVersionInfo(Path, VerHnd, VerInfoSize, StrVffInfo);
	 
	 LPVOID Version;
	 UINT VersionSize;
	 if (VerQueryValue(StrVffInfo, "\\", &Version, &VersionSize))
	 {
	 if (VersionSize >= sizeof(VS_FIXEDFILEINFO))
	 {
	 VS_FIXEDFILEINFO *pVersionInfo = (VS_FIXEDFILEINFO *) Version;
	 DllVersion =
	 PYRO_MAKE_VERSION(
	 pVersionInfo->dwFileVersionMS >> 16,
	 pVersionInfo->dwFileVersionMS & 0xFFFF,
	 pVersionInfo->dwFileVersionLS >> 16);
	 }
	 }
	 
	 GlobalUnlock(hMem);
	 GlobalFree(hMem);
	 }
	 
	 if (DllVersion != 0 && DllVersion != Version)
	 throw PyroParticles::CPyroException(
	 "Incompatible Pyro.dll and SDK version (DLL = %d.%d.%d, SDK = %d.%d.%d)",
	 Version >> 16, (Version >> 8) & 0xff, (Version & 0xff),
	 DllVersion >> 16, (DllVersion >> 8) & 0xff, (DllVersion & 0xff));
	 #endif
	 */
	return new PyroParticles::CPyroParticleLibrary();
}

void DestroyParticleLibrary(PyroParticles::IPyroParticleLibrary *pParticleLibrary)
{
	if (pParticleLibrary)
		delete pParticleLibrary;
}

#endif /* ENGINE_FLASH */

#endif /* ENGINE_NO_PYRO */
