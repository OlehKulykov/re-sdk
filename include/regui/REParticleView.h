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


#ifndef __REPARTICLEVIEW_H__
#define __REPARTICLEVIEW_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REMainLoopUpdatable.h"
#include "../recore/REPoint2.h"
#include "REView.h"

/// Class of Pyro Particle view.
class __RE_PUBLIC_CLASS_API__ REParticleView : public REView, public REMainLoopUpdatable
{
private:
#if defined(__RE_USING_ADITIONAL_PYRO_PARTICLES_LIBRARY__) || defined(__RE_USING_SYSTEM_PYRO_PARTICLES_LIBRARY__)
	void * _emitter;
	void * _particleFile;
	REFloat32 _emitterX;
	REFloat32 _emitterY;
	REFloat32 _emitterZ;
	REFloat32 _emitterStartTime;
	REFloat32 _emitterActivityTime;
	REFloat32 _particleGlobalTime;
	REFloat32 _particleGlobalStartTime;

    REBOOL InitFromParticleViewFile(void * file);
#endif

protected:
	REParticleView();
	virtual ~REParticleView();
	
	void Clear();
private:
    static REBOOL AcceptParticleViewStringParameter(REParticleView * particle, const char * key, const char * value);
public:
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	/* IRERenderable */
	virtual void Render();
	virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	virtual void SetFrame(const RERect & newViewFrame);
	
	/* REMainLoopUpdatable */
	virtual void Update(const RETimeInterval currentTime);
	virtual const REUIdentifier GetMainLoopUpdatableIdentifier() const { return this->GetObjectIdentifier(); }
	
    /* REGUIObject */
    /// Acepting string parameter and value string presentation from XML document.
    /// Using when view controller loading from XML. This method can be invoked from non-main thread if view controller loading in background.
    virtual REBOOL AcceptStringParameter(const char * key, const char * value);

	/// Initializes particle from file path.
	REBOOL InitFromFilePath(const REString & filePath);
	
	/// Initializes particle from file data.
    REBOOL InitFromFileData(const REData & fileData);
	
	/// Initializes particle from file buffer.
    REBOOL InitFromFileBuffer(const REBuffer & fileBuffer);

	/// Resets particle view.
	void Reset();
	
	/// Sets center coordinates of particle view.
	void SetCenter(const REFloat32 centerX, const REFloat32 centerY);
	
	/// Sets center point of particle view.
	void SetCenter(const REPoint2 & center);
	
	/// Returns center point of particle view.
	REPoint2 GetCenter() const;
	
	/// Creates and returns new particle view object.
	static REParticleView * Create();

	/// Returns XML key string for file path.
    static const char * GetXMLPathKeyString();
	
	/// Returns XML key string for center coordinates.
    static const char * GetXMLCenterKeyString();
	
	/// Returns XML format string for center coordinates.
    static const char * GetXMLCenterFormatString();
};


#endif /* __REPARTICLEVIEW_H__ */



