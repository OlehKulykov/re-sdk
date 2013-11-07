/*
 *   Copyright 2012 - 2013 Kulykov Oleh
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */


#include "../../include/regui/REParticleView.h"
#include "../../include/recore/REData.h"
#include "../../include/recore/RETime.h"
#include "../../include/recore/RELog.h"

#if defined(__RE_USING_ADITIONAL_PYRO_PARTICLES_LIBRARY__)
#include "../addlibs/pyroparticles.h"
#elif defined(__RE_USING_SYSTEM_PYRO_PARTICLES_LIBRARY__)
#include <Pyro.h>

#if defined(__RE_USING_DIRECTX9__)
#include <PyroD3D9.h>
#elif defined(__RE_USING_OPENGL__)
#include <PyroOpenGL.h>
#elif defined(__RE_USING_OPENGL_ES__)
#include <PyroOpenGLES.h>
#endif

#else
#define __RE_NO_PYRO_PARTICLES_PRIVATE__
#endif

#ifdef __RE_USING_DIRECTX9__
#include "../../include/regui/RERenderDeviceDirectX9.h"
#endif

#define RE_PARTICLEVIEW_XML_PATH_KEY_STRING "path"
#define RE_PARTICLEVIEW_XML_CENTER_KEY_STRING "centerf"
#define RE_PARTICLEVIEW_XML_CENTER_FORMAT_STRING "%f;%f"

class REParticleViewPrivate
{
public:
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
	static PyroParticles::IPyroParticleLibrary * pyroPartLib;
	static PyroParticles::PyroGraphics::IDevice * pyroPartGraphicsDevice;
	static REUInt32 activeParticlesCount;
	
	static void initParticles();
	static void releaseParticles();
#endif
};

#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
PyroParticles::IPyroParticleLibrary * REParticleViewPrivate::pyroPartLib = NULL;
PyroParticles::PyroGraphics::IDevice * REParticleViewPrivate::pyroPartGraphicsDevice = NULL;
REUInt32 REParticleViewPrivate::activeParticlesCount = 0;

void REParticleViewPrivate::initParticles()
{
	if (REParticleViewPrivate::pyroPartLib == NULL)
	{
#ifdef __RE_USING_EXCEPTIONS__
		PyroParticles::IPyroParticleLibrary * plb = NULL;
        try
        {
            /// In headers used "PYRO_MAKE_VERSION(1, 1, 15)" and iPhone
            /// version of compiled library used old version "PYRO_MAKE_VERSION(1, 1, 14)".
            /// Please note that.
            plb = CreateParticleLibrary(PYRO_SDK_VERSION/*PYRO_MAKE_VERSION(1, 1, 14)*/);
        }
        catch (PyroParticles::CPyroException & e)
        {
            RELog::Log("Failed CreateParticleLibrary: %s", e.GetExceptionMessage());
            RELog::Log("Using CreateParticleLibrary(PYRO_SDK_VERSION) but <PYRO_SDK_VERSION> can be defined as one version and compiled library using another(old) version of code.\nDescribed bug was detected in iOS build.");
            return;
        }
#else
		PyroParticles::IPyroParticleLibrary * plb = CreateParticleLibrary(PYRO_SDK_VERSION);
#endif
		if (plb)
		{
            //int maxQuads = 512;
			/*
#ifdef __RE_OS_IPHONE__
			maxQuads = 512;
#endif
			
#ifdef __RE_OS_ANDROID__
			maxQuads = 512;
#endif
			*/
			PyroParticles::PyroGraphics::IDevice * graphicDevice = NULL;
#if (defined(__RE_USING_OPENGL__) || defined(__RE_USING_OPENGL_ES__))
            //graphicDevice = new PyroParticles::CGraphics_OGL(maxQuads);
            graphicDevice = new PyroParticles::CGraphics_OGL();
#endif
			
#ifdef __RE_USING_DIRECTX9__
			RERenderDeviceDirectX9 * d3d9Device = RERenderDevice::GetDefaultDeviceDirectX9();
			graphicDevice = new PyroParticles::CGraphicsDevice_D3D9(d3d9Device->GetD3D9Device(), maxQuads, (BOOL)true);
#endif
			if (graphicDevice)
			{
				plb->Init(graphicDevice);
				
				REParticleViewPrivate::pyroPartGraphicsDevice = graphicDevice;
				REParticleViewPrivate::pyroPartLib = plb;
			}
			else
			{
				delete plb;
			}
		}
	}
}

void REParticleViewPrivate::releaseParticles()
{
	if (REParticleViewPrivate::pyroPartLib)
	{
		PyroParticles::IPyroParticleLibrary * plb = (PyroParticles::IPyroParticleLibrary *)REParticleViewPrivate::pyroPartLib;
		REParticleViewPrivate::pyroPartLib = NULL;
		
		PyroParticles::PyroGraphics::IDevice * graphicDevice = (PyroParticles::PyroGraphics::IDevice *)REParticleViewPrivate::pyroPartGraphicsDevice;
		REParticleViewPrivate::pyroPartGraphicsDevice = NULL;
		
		plb->Done();
		DestroyParticleLibrary(plb);
		
		delete graphicDevice;
	}
}

class REParticleViewFilePrivate : public REData, public PyroParticles::IPyroFileIO 
{	
public:
	/* PyroParticles::IPyroFileIO */
	virtual long Read(void *pDest, long nBytes) 
	{
		return (long)this->FileRead(pDest, (REUInt32)nBytes); 
	}
	virtual long Write(void *pSrc, long nBytes) 
	{
		return 0; 
	}
	virtual long Seek(long Position, int Where = SEEK_SET) 
	{ 
		return (long)this->FileSeek((REUInt32)Position, Where);
	}	
	virtual long Tell() 
	{
		return (long)this->FileTell();
	}

    REParticleViewFilePrivate() : REData() { }
    REParticleViewFilePrivate(const REData & anotherData) : REData(anotherData) { }
    REParticleViewFilePrivate(const REBuffer & buffer) : REData(buffer) { }
};

#endif

REBOOL REParticleView::acceptParticleViewStringParameter(REParticleView * particle, const char * key, const char * value)
{
    if (strcmp(key, RE_PARTICLEVIEW_XML_PATH_KEY_STRING) == 0)
    {
        return particle->initFromFilePath(REString(value));
    }
    else if (strcmp(key, RE_PARTICLEVIEW_XML_CENTER_KEY_STRING) == 0)
    {
        REPoint2 c;
        if (sscanf(value, RE_PARTICLEVIEW_XML_CENTER_FORMAT_STRING, &c.x, &c.y) == 2)
        {
            particle->setCenter(c);
            return true;
        }
    }
    return false;
}

/* IREXMLSerializable */
/// Acepting string parameter and value string presentation from XML document.
/// Using when view controller loading from XML. This method can be invoked from non-main thread if view controller loading in background.
REBOOL REParticleView::acceptStringParameter(const char * key, const char * value)
{
#if 0	
    if (REView::acceptStringParameter(key, value))
    {
        return true;
    }
    if (key && value)
    {
        return REParticleView::acceptParticleViewStringParameter(this, key, value);
    }
#endif	
    return false;
}

void REParticleView::clear()
{
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
    this->RemoveFromMainLoop();
	
	if (_particleFile)
	{
		PyroParticles::IPyroFile * partFile = (PyroParticles::IPyroFile *)_particleFile;
		partFile->DestroyTextures();
		delete partFile;
		_particleFile = NULL;
	}
	
	if (_emitter)
	{
		PyroParticles::IPyroParticleEmitter * emitter = (PyroParticles::IPyroParticleEmitter *)_emitter;
		delete emitter;
		_emitter = NULL;
	}
#endif
}

/* IRERenderable */
void REParticleView::render()
{
	if (_isVisible)
	{
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
		if (_emitter && (_color.alpha > 0.0f)) 
		{
			PyroParticles::IPyroParticleEmitter * emitter = (PyroParticles::IPyroParticleEmitter *)_emitter;
			if (this->IsAnimating())
			{
				REPoint2 center(_frame.GetCenter());
				const float emitterScele = emitter->GetScale(); 
				_emitterX = (center.x / emitterScele);
				_emitterY = (center.y / emitterScele);
			}

			RERenderDevice * device = RERenderDevice::defaultDevice();
			const RERenderDeviceBlendType source = device->GetBlendSourceFactor();
			const RERenderDeviceBlendType dest = device->GetBlendDestinationFactor();
#if (defined(__RE_USING_OPENGL_ES__) || defined (__RE_USING_OPENGL__))
			emitter->render();
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            //glAlphaFunc(GL_GREATER, 0.1f);
#elif defined(__RE_USING_DIRECTX9__) 
			RERenderDeviceDirectX9 * d3d9Device = (RERenderDeviceDirectX9 *)RERenderDevice::defaultDevice();
			d3d9Device->PushRenderStates();
			d3d9Device->PushTextureStageStates();
			
			//d3d9Device->SetRenderState(D3DRS_ZENABLE, FALSE);
			//d3d9Device->SetRenderState(D3DRS_LIGHTING, FALSE);
			//d3d9Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			
			d3d9Device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
			d3d9Device->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
			d3d9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			d3d9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			
			emitter->render();
			
			d3d9Device->PopRenderStates();
			d3d9Device->PopTextureStageStates();
#endif
            device->SetBlendFunction(source, dest);
            //RERenderDevice::defaultDevice()->SetDefaultBlendFunction();
		}
#endif
		
		this->renderSubviews(_frame.x, _frame.y);
	}
}

void REParticleView::renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	if (_isVisible)
	{
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
		if (_emitter && (_color.alpha > 0.0f)) 
		{
			PyroParticles::IPyroParticleEmitter * emitter = (PyroParticles::IPyroParticleEmitter *)_emitter;
			if (this->IsAnimating())
			{
				REPoint2 center(_frame.GetCenter());
				const float emitterScele = emitter->GetScale(); 
				_emitterX = (center.x / emitterScele) + offsetX;
				_emitterY = (center.y / emitterScele) + offsetY;
			}

			RERenderDevice * device = RERenderDevice::defaultDevice();
			const RERenderDeviceBlendType source = device->GetBlendSourceFactor();
			const RERenderDeviceBlendType dest = device->GetBlendDestinationFactor();
#if ( defined(__RE_USING_OPENGL__) || defined(__RE_USING_OPENGL_ES__) )
			emitter->render();
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            //glAlphaFunc(GL_GREATER, 0.1f);
#elif defined(__RE_USING_DIRECTX9__) 
			RERenderDeviceDirectX9 * d3d9Device = (RERenderDeviceDirectX9 *)RERenderDevice::defaultDevice();
			d3d9Device->PushRenderStates();
			d3d9Device->PushTextureStageStates();
			
			d3d9Device->SetRenderState(D3DRS_ZENABLE, FALSE);
			d3d9Device->SetRenderState(D3DRS_LIGHTING, FALSE);
			d3d9Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			
			d3d9Device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
			d3d9Device->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
			d3d9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			d3d9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE); 
			
			emitter->render();
			
			d3d9Device->PopRenderStates();
			d3d9Device->PopTextureStageStates();
#endif
            device->SetBlendFunction(source, dest);
            //RERenderDevice::defaultDevice()->SetDefaultBlendFunction();
		}
#endif
		
		this->renderSubviews(_frame.x, _frame.y);
	}
}

void REParticleView::setFrame(const RERect & newViewFrame)
{
	REView::setFrame(newViewFrame);
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
	if (_emitter)
	{
		PyroParticles::IPyroParticleEmitter * emitter = (PyroParticles::IPyroParticleEmitter *)_emitter;
		
		const float radius = emitter->GetContainerRadius();
		float emiterScaleFactor = 1.0f;
		if (radius != 0.0f) 
		{
			emiterScaleFactor = (_frame.width / 2.0f) / radius;
			if ( emiterScaleFactor == 0.0f )
			{
				emiterScaleFactor = 1.0f;
			}
		}
		emitter->SetScale( emiterScaleFactor );
	}
#endif
	this->setCenter(_frame.center());
}

#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
REBOOL REParticleView::InitFromParticleViewFile(void * file)
{
    if (file)
    {
        REParticleViewFilePrivate * fileData = (REParticleViewFilePrivate *)file;
        PyroParticles::IPyroParticleLibrary * l = (PyroParticles::IPyroParticleLibrary *)REParticleViewPrivate::pyroPartLib;
#ifdef __RE_USING_EXCEPTIONS__
        PyroParticles::IPyroFile * partFile = NULL;
        try
        {
            partFile = l->LoadPyroFile(fileData);
        }
        catch (PyroParticles::CPyroException & e)
        {
            RELog::log("\nFailed PyroParticles::IPyroFile::LoadPyroFile: %s", e.GetExceptionMessage());
            RELog::log("\n\nUsing <PYRO_SDK_VERSION> can be defined as one version and compiled library using another(old) version of code.\nParticle editor generates current version(in header) of file witch is uncompatible with compiled library.\nDescribed bug was detected in iOS build.");
            return false;
        }
#else
        PyroParticles::IPyroFile * partFile = l->LoadPyroFile(fileData);
#endif
        if (partFile)
        {
            partFile->CreateTextures();

            _particleGlobalTime = RETime::Time();
            _particleGlobalStartTime = _particleGlobalTime;

            PyroParticles::IPyroParticleEmitter * rootEmitter = (PyroParticles::IPyroParticleEmitter *)partFile->GetFileEmitter(0);
            if (rootEmitter)
            {
                const char * pEmitterName = rootEmitter->GetName();
                PyroParticles::IPyroParticleEmitter * newEmitter = partFile->CreateEmitter( pEmitterName );
                if ( newEmitter )
                {
                    _emitterStartTime = _particleGlobalTime;
                    _emitterActivityTime = _particleGlobalTime;
                    newEmitter->SetOffset(0.0f, 0.0f, 0.0f);
                    newEmitter->SetScale(1.0f);

                    _emitter = newEmitter;
                    _particleFile = partFile;

                    this->SetFrame(_frame);

                    this->AddToMainLoop();

                    return true;
                }
            }
            partFile->DestroyTextures();
            delete partFile;
        }
    }
    return false;
}
#endif

REBOOL REParticleView::initFromFilePath(const REString & filePath)
{
	this->clear();
	
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
	if (REParticleViewPrivate::pyroPartLib)
	{
        REParticleViewFilePrivate particleFileData;
        if (particleFileData.InitFromPath(filePath))
		{
            return this->InitFromParticleViewFile(&particleFileData);
        }
    }
#endif
	
	return false;
}

REBOOL REParticleView::initFromFileData(const REData & fileData)
{
    this->clear();
	
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
    if (REParticleViewPrivate::pyroPartLib)
    {
        REParticleViewFilePrivate particleFileData(fileData);
        if (!particleFileData.IsEmpty())
        {
            return this->InitFromParticleViewFile(&particleFileData);
        }
    }
#endif
    return false;
}

REBOOL REParticleView::initFromFileBuffer(const REBuffer & fileBuffer)
{
    this->clear();
	
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
    if (REParticleViewPrivate::pyroPartLib)
    {
        REParticleViewFilePrivate particleFileData(fileBuffer);
        if (!particleFileData.IsEmpty())
        {
            return this->InitFromParticleViewFile(&particleFileData);
        }
    }
#endif
	
    return false;
}

void REParticleView::reset()
{
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
	if (_emitter)
	{
		PyroParticles::IPyroParticleEmitter * emitter = (PyroParticles::IPyroParticleEmitter *)_emitter;
		emitter->Reset();
	}
#endif
}

void REParticleView::setCenter(const REFloat32 centerX, const REFloat32 centerY)
{
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
	if (_emitter)
	{
		PyroParticles::IPyroParticleEmitter * emitter = (PyroParticles::IPyroParticleEmitter *)_emitter;
		
		float emiterScaleFactor = emitter->GetScale();
		if (emiterScaleFactor != 0.0f) 
		{
			_emitterX = centerX / emiterScaleFactor;
			_emitterY = centerY / emiterScaleFactor;
		}
	}
#endif
	_frame.setCenter(centerX, centerY);
}

void REParticleView::setCenter(const REPoint2 & center)
{
	this->setCenter(center.x, center.y);
}

REPoint2 REParticleView::getCenter() const
{
	return _frame.center();
}

/* REMainLoopUpdatable */
void REParticleView::update(const RETimeInterval currentTime)
{
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
	PyroParticles::IPyroParticleEmitter * emitter = (PyroParticles::IPyroParticleEmitter *)_emitter;
	
	const float prevTime = _particleGlobalTime;
	_particleGlobalTime = currentTime - _particleGlobalStartTime;
	float timeDelta = _particleGlobalTime - prevTime;
	float curTime = prevTime;
	while (timeDelta > 0.0f)
	{
		const float safeTimeDelta = (timeDelta >= 0.02f) ? 0.02f : timeDelta;
		
		const float curTimeMStartTime = curTime - _emitterStartTime;
		
		// MOVE
		emitter->Move(curTimeMStartTime, safeTimeDelta);
		
		//PREPARE
		emitter->Prepare(curTimeMStartTime, safeTimeDelta, _emitterX, _emitterY, _emitterZ, PyroTrue);
		
		if (emitter->HasParticles())
		{
			_emitterActivityTime = curTime;
		}
		
		curTime += safeTimeDelta;
		timeDelta -= safeTimeDelta;
	}
#endif
}

void REParticleView::onReleased()
{
	this->clear();
	
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
	if (REParticleViewPrivate::activeParticlesCount > 0)
	{
		REParticleViewPrivate::activeParticlesCount--;
	}
	
	if (REParticleViewPrivate::activeParticlesCount == 0)
	{
		REParticleViewPrivate::ReleaseParticles();
	}
#endif
	
	REView::onReleased();
}

REParticleView::REParticleView() : REView()
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
	,_emitter(NULL),
	_particleFile(NULL),
	_emitterX(0.0f),
	_emitterY(0.0f),
	_emitterZ(0.0f),
	_emitterStartTime(0.0f),
	_emitterActivityTime(0.0f),
	_particleGlobalTime(0.0f),
	_particleGlobalStartTime(0.0f)
#endif
{
#ifndef __RE_NO_PYRO_PARTICLES_PRIVATE__
	REParticleViewPrivate::activeParticlesCount++;
	REParticleViewPrivate::InitParticles();
#endif
}

REParticleView::~REParticleView()
{
	
}

REParticleView * REParticleView::create()
{
	REParticleView * newParticleView = new REParticleView();
	return newParticleView;
}

const char * REParticleView::getXMLPathKeyString() { return RE_PARTICLEVIEW_XML_PATH_KEY_STRING; }
const char * REParticleView::getXMLCenterKeyString() { return RE_PARTICLEVIEW_XML_CENTER_KEY_STRING; }
const char * REParticleView::getXMLCenterFormatString() { return RE_PARTICLEVIEW_XML_CENTER_FORMAT_STRING; }


