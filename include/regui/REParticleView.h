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

    REBOOL initFromParticleViewFile(void * file);
#endif

protected:
	REParticleView();
	virtual ~REParticleView();
	
	void clear();
private:
    static REBOOL acceptParticleViewStringParameter(REParticleView * particle, const char * key, const char * value);
public:
	/* REObject */
	virtual void onReleased();
	
	/* IRERenderable */
	virtual void render();
	virtual void renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	virtual void setFrame(const RERect & newViewFrame);
	
	/* REMainLoopUpdatable */
	virtual void update(const RETimeInterval currentTime);
	
    /* REGUIObject */
    /// Acepting string parameter and value string presentation from XML document.
    /// Using when view controller loading from XML. This method can be invoked from non-main thread if view controller loading in background.
    virtual REBOOL acceptStringParameter(const char * key, const char * value);

	/// Initializes particle from file path.
	REBOOL initFromFilePath(const REString & filePath);
	
	/// Initializes particle from file data.
    REBOOL initFromFileData(const REData & fileData);
	
	/// Initializes particle from file buffer.
    REBOOL initFromFileBuffer(const REBuffer & fileBuffer);

	/// Resets particle view.
	void reset();
	
	/// Sets center coordinates of particle view.
	void setCenter(const REFloat32 centerX, const REFloat32 centerY);
	
	/// Sets center point of particle view.
	void setCenter(const REPoint2 & center);
	
	/// Returns center point of particle view.
	REPoint2 getCenter() const;
	
	/// Creates and returns new particle view object.
	static REParticleView * create();

	/// Returns XML key string for file path.
    static const char * getXMLPathKeyString();
	
	/// Returns XML key string for center coordinates.
    static const char * getXMLCenterKeyString();
	
	/// Returns XML format string for center coordinates.
    static const char * getXMLCenterFormatString();
};


#endif /* __REPARTICLEVIEW_H__ */



