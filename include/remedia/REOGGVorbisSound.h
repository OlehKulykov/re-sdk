/*
 *   Copyright 2012 Kulikov Oleg
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
	
	void Clear();
	
	void DecodeAndPlay();
	void GetVorbisInformation();
#ifdef __RE_USING_OPENAL__	
	void GenerateAndSetupALResources();
#endif	
	void FillBuffersWithDecodedAudioData();
	REUInt32 DecodeOggVorbis(void * psOggVorbisFileVoid, char * pDecodeBuffer, const REUInt32 ulBufferSize, const REUInt32 ulChannels);
	
public:
	/* REMainLoopUpdatable */
	virtual void Update(const RETimeInterval currentTime);
	virtual const REUIdentifier GetMainLoopUpdatableIdentifier() const;
	
	REBOOL InitWithData(const REData & soundFileData);
	virtual REBOOL Play();
	virtual REBOOL IsPlaying() const;
	virtual REBOOL Pause();
	virtual REBOOL Stop();
	virtual REBOOL SetLooped(const REBOOL isLooped);
	virtual REBOOL IsLooped() const;
	virtual REBOOL SetVolume(const REFloat32 newVolume);
	virtual const REFloat32 GetVolume() const;
	
	static REBOOL IsValidData(const REData & data);
	
	REOGGVorbisSound();
	virtual ~REOGGVorbisSound();
};



#endif /* __REOGGVORBISSOUND_H__ */

