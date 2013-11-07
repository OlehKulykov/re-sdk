/*
 *   Copyright 2012 - 2013 Kulikov Oleg
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


#ifndef __REWAVSOUND_H__
#define __REWAVSOUND_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REObject.h"
#include "../recore/REData.h"

#include "IREMediaPlayable.h"

#ifdef __RE_USING_OPENAL__
#include "REAudioOpenAL.h"
#endif



class __RE_PUBLIC_CLASS_API__ REWAVSound : public REObject,
#ifdef __RE_USING_OPENAL__
public REAudioOpenAL,
#endif

public IREMediaPlayable

{
private:
#ifdef __RE_USING_OPENAL__	
	ALuint _buffer;
	ALuint _source;
#endif	
	REFloat32 _volume;
	REBOOL _isLooped;

#ifdef __RE_USING_OPENAL__		
	static REBOOL loadPcmWavData(const REUByte * data, ALuint * buffer, ALuint * source);
#endif
	void clear();
public:
	REBOOL initWithData(const REData & soundFileData);
	virtual REBOOL play();
	virtual REBOOL isPlaying() const;
	virtual REBOOL pause();
	virtual REBOOL stop();
	virtual REBOOL setLooped(const REBOOL isLooped);
	virtual REBOOL isLooped() const;
	virtual REBOOL setVolume(const REFloat32 newVolume);
	virtual const REFloat32 getVolume() const;
	
	static REBOOL isValidData(const REData & data);
	
	REWAVSound();
	virtual ~REWAVSound();
};

#endif /* __REWAVSOUND_H__ */

