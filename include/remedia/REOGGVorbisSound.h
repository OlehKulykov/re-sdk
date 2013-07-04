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


#ifndef __REOGGVORBISSOUND_H__
#define __REOGGVORBISSOUND_H__


#include "../recore/RECommonHeader.h"
#include "../recore/REObject.h"
#include "../recore/REBuffer.h"
#include "../recore/REMainLoopUpdatable.h"
#include "../recore/REData.h"
#include "IREMediaPlayable.h"

#ifdef __RE_USING_OPENAL__
#include "REAudioOpenAL.h"
#endif


#define OGG_BUFFERS_COUNT 4

typedef struct _reOggStreamStruct
{
	REData * oggFileData;
	void * oggStream;
	const REUByte * oggData;
	REUInt32 dataSize;
	REUInt32 dataRead;
} REOggStreamStruct;

class REOGGVorbisSound : public REObject, 
#ifdef __RE_USING_OPENAL__
public REAudioOpenAL,
#endif
public IREMediaPlayable, public REMainLoopUpdatable
{
protected:	
	REOggStreamStruct _oggMemoryFile;
#ifdef __RE_USING_OPENAL__	
	ALuint _buffers[OGG_BUFFERS_COUNT];
	ALuint _buffer;
	ALuint _source;
#endif	
	REUInt32 _totalBuffersProcessed;
	REUInt32 _frequency;
	REUInt32 _channels;
	REUInt32 _format;
	REUInt32 _bufferSize;
	REUInt32 _bytesWritten;
	RETimeInterval _lastDecodeTime;
	REBuffer * _decodeBuffer;
 	REFloat32 _volume;
	REBOOL _isLooped;
	
	void clear();
	
	void decodeAndPlay();
	void getVorbisInformation();
#ifdef __RE_USING_OPENAL__	
	void generateAndSetupALResources();
#endif	
	void fillBuffersWithDecodedAudioData();
	REUInt32 decodeOggVorbis(void * psOggVorbisFileVoid, char * pDecodeBuffer, const REUInt32 ulBufferSize, const REUInt32 ulChannels);
	
public:
	/* REMainLoopUpdatable */
	virtual void update(const RETimeInterval currentTime);
	virtual const REUIdentifier getMainLoopUpdatableIdentifier() const;
	
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
	
	REOGGVorbisSound();
	virtual ~REOGGVorbisSound();
};



#endif /* __REOGGVORBISSOUND_H__ */

