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


#include "../../include/remedia/REOGGVorbisSound.h"

#if defined(__RE_USING_ADITIONAL_VORBIS_LIBRARY__)
#include "../addlibs/vorbis.h"
#elif defined(__RE_USING_SYSTEM_VORBIS_LIBRARY__)
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#else
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
#define __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
#endif
#endif


#if defined(__RE_USING_SYSTEM_OGG_LIBRARY__)
#include <ogg/ogg.h>
#elif defined(__RE_USING_ADITIONAL_OGG_LIBRARY__)
#include "../addlibs/ogg.h"
#else
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
#define __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
#endif
#endif


#define SWAP_SHORT(a,b) {const REInt16 t=a;a=b;b=t;}


#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
class REOGGVorbisSoundPrivate
{
public:
	static size_t VorbisRead(void *ptr, size_t byteSize, size_t sizeToRead, void *datasource);
	static int VorbisSeek(void *datasource, ogg_int64_t offset, int whence);
	static int VorbisClose(void *datasource);
	static long VorbisTell(void *datasource);
};


size_t REOGGVorbisSoundPrivate::VorbisRead(void *ptr, size_t byteSize, size_t sizeToRead, void *datasource)
{
	REOggStreamStruct * ogStream = (REOggStreamStruct*)datasource;
	const size_t spaceToEOF = (ogStream->dataSize - ogStream->dataRead);
	const size_t needReadBytes = (sizeToRead * byteSize);
	const size_t actualSizeToRead = (needReadBytes < spaceToEOF) ? needReadBytes : spaceToEOF;
	
	if (actualSizeToRead)
	{
		memcpy(ptr, &ogStream->oggData[ogStream->dataRead], actualSizeToRead);
		ogStream->dataRead += (REUInt32)actualSizeToRead;
	}
	
	return actualSizeToRead;
}

int REOGGVorbisSoundPrivate::VorbisSeek(void *datasource, ogg_int64_t offset, int whence)
{
	REOggStreamStruct * ogStream = (REOggStreamStruct*)datasource;
	switch (whence)
	{
		case SEEK_SET:
		{
			if (ogStream->dataSize >= offset)
			{
				ogStream->dataRead = (REUInt32)offset;
			}
			else
			{
				ogStream->dataRead = ogStream->dataSize;
			}
		}
			break;
			
		case SEEK_CUR:
		{
			const REUInt32 spaceToEOF = ogStream->dataSize - ogStream->dataRead;
			if (offset < spaceToEOF)
			{
				ogStream->dataRead += (REUInt32)offset;
			}
			else
			{
				ogStream->dataRead += spaceToEOF;
			}
		}
			break;
			
		case SEEK_END:
		{
			ogStream->dataRead = ogStream->dataSize + 1;
		}
			break;
			
		default:
			break;
	}
	return 0;
}

int REOGGVorbisSoundPrivate::VorbisClose(void *datasource)
{
	return 1;
}

long REOGGVorbisSoundPrivate::VorbisTell(void *datasource)
{
	REOggStreamStruct * ogStream = (REOggStreamStruct*)datasource;
	return ((long)ogStream->dataRead);
}
#endif /* __RE_NO_OGG_VORBIS_SOUND_PRIVATE__ */


void REOGGVorbisSound::update(const RETimeInterval currentTime)
{
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
	// update 20 milisec
	if ( (currentTime - _lastDecodeTime) >= 0.017 )
	{
		this->decodeAndPlay();
		_lastDecodeTime = currentTime;
	}
#endif
}

REBOOL REOGGVorbisSound::play()
{
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
#ifdef __RE_USING_OPENAL__	
	if ( _source )
	{
		this->addToMainLoop();
		alSourcePlay(_source);
		this->decodeAndPlay();
		return (alGetError() == AL_NO_ERROR);
	}
#endif	
#endif
	return false;
}

REBOOL REOGGVorbisSound::isPlaying() const
{
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
#ifdef __RE_USING_OPENAL__
	if ( _source )
	{
		ALenum state;
		alGetSourcei(_source, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}
#endif	
#endif
	return false;
}

REBOOL REOGGVorbisSound::pause()
{
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
#ifdef __RE_USING_OPENAL__
	if ( _source )
	{
		this->removeFromMainLoop();
		alSourcePause(_source);
		return (alGetError() == AL_NO_ERROR);
	}
#endif
#endif
	return false;
}

REBOOL REOGGVorbisSound::stop()
{
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
#ifdef __RE_USING_OPENAL__
	if ( _source )
	{
		this->removeFromMainLoop();
		alSourceStop(_source);
		return (alGetError() == AL_NO_ERROR);
	}
#endif
#endif
	return false;
}

REBOOL REOGGVorbisSound::setLooped(const REBOOL isLooped)
{
	_isLooped = isLooped;
	return true;
}

REBOOL REOGGVorbisSound::isLooped() const
{
	return _isLooped;
}

REBOOL REOGGVorbisSound::setVolume(const REFloat32 newVolume)
{
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
	if ( newVolume < 0.0f )
	{
		_volume = 0.0f;
	}
	else if ( newVolume > 1.0f )
	{
		_volume = 1.0f;
	}
	else
	{
		_volume = newVolume;
	}
#ifdef __RE_USING_OPENAL__
	if ( _source )
	{
		alSourcef(_source, AL_GAIN, _volume);
	}
#endif
#endif
	return false;
}

const REFloat32 REOGGVorbisSound::getVolume() const
{
	return _volume;
}

void REOGGVorbisSound::getVorbisInformation()
{
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
	OggVorbis_File * ogStream = (OggVorbis_File *)_oggMemoryFile.oggStream;
	ov_comment(ogStream, -1);
	vorbis_info * vorbisInfo = ov_info(ogStream, -1);
	if (vorbisInfo)
	{
		_frequency = (REUInt32)vorbisInfo->rate;
		_channels = (REUInt32)vorbisInfo->channels;
		if (vorbisInfo->channels == 1)
		{
#ifdef __RE_USING_OPENAL__
			_format = AL_FORMAT_MONO16;
#endif			
			_bufferSize = (REUInt32)(vorbisInfo->rate >> 1);
			_bufferSize -= (_bufferSize % 2);
		}
		else if (vorbisInfo->channels == 2)
		{
#ifdef __RE_USING_OPENAL__			
			_format = AL_FORMAT_STEREO16;
#endif			
			_bufferSize = (REUInt32)vorbisInfo->rate;
			_bufferSize -= (_bufferSize % 4);
		}
		else if (vorbisInfo->channels == 4)
		{
#ifdef __RE_USING_OPENAL__			
			_format = alGetEnumValue("AL_FORMAT_QUAD16");
#endif			
			_bufferSize = (REUInt32)(vorbisInfo->rate * 2);
			_bufferSize -= (_bufferSize % 8);
		}
		else if (vorbisInfo->channels == 6)
		{
#ifdef __RE_USING_OPENAL__			
			_format = alGetEnumValue("AL_FORMAT_51CHN16");
#endif			
			_bufferSize = (REUInt32)(vorbisInfo->rate * 3);
			_bufferSize -= (_bufferSize % 12);
		}
	}
#endif
}

#ifdef __RE_USING_OPENAL__
void REOGGVorbisSound::generateAndSetupALResources()
{
	alGenBuffers( OGG_BUFFERS_COUNT, _buffers );
	alGenSources( 1, &_source );
	ALfloat sourcePos[] = {0.0f, 0.0f, 0.0f};
	ALfloat sourceVel[] = {0.0f, 0.0f, 0.0f};
	alSourcef(_source, AL_PITCH, 1.0f);
	alSourcef(_source, AL_GAIN, 1.0f);
	alSourcefv(_source, AL_POSITION, sourcePos);
	alSourcefv(_source, AL_VELOCITY, sourceVel);
	alSourcei(_source, AL_LOOPING, _isLooped); 
}
#endif

void REOGGVorbisSound::decodeAndPlay()
{
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
	OggVorbis_File * ogStream = (OggVorbis_File *)_oggMemoryFile.oggStream;
	ALint buffersProcessed = 0;
#ifdef __RE_USING_OPENAL__	
	alGetSourcei(_source, AL_BUFFERS_PROCESSED, &buffersProcessed);
#endif	
	_totalBuffersProcessed += buffersProcessed;
	while (buffersProcessed)
	{
		_buffer = 0;
#ifdef __RE_USING_OPENAL__		
		alSourceUnqueueBuffers(_source, 1, &_buffer);
#endif		
		_bytesWritten = this->decodeOggVorbis(ogStream, (char*)_decodeBuffer->buffer(), _decodeBuffer->size(), _channels);
		if (_bytesWritten > 0)
		{
#ifdef __RE_USING_OPENAL__			
			alBufferData(_buffer, (ALenum)_format, _decodeBuffer->buffer(), (ALsizei)_bytesWritten, (ALsizei)_frequency);
			alSourceQueueBuffers(_source, 1, &_buffer);
#endif	
		}
		buffersProcessed--;
	}
	
#ifdef __RE_USING_OPENAL__	
	ALint iState;
	alGetSourcei(_source, AL_SOURCE_STATE, &iState);
	if ( iState != AL_PLAYING )
	{
		ALint queuedBuffers = 0;
		alGetSourcei(_source, AL_BUFFERS_QUEUED, &queuedBuffers);
		if (queuedBuffers)
		{
			alSourcePlay(_source);
		}
		else
		{
			//STOPED
			alSourceStop(_source);
			alSourcei(_source, AL_BUFFER, 0);
			_oggMemoryFile.dataRead = 0;
			_bytesWritten = 0;
			_lastDecodeTime = 0.0;
			_totalBuffersProcessed = 0;
			this->fillBuffersWithDecodedAudioData();
			_totalBuffersProcessed = 0;
			
			if (_isLooped)
			{
				alSourcePlay(_source);
			}
			else
			{
				this->removeFromMainLoop();
			}
		}
	}
#endif	
#endif	
	//glutTimerFunc(20, DecodeAndPlay, sound);
}

REUInt32 REOGGVorbisSound::decodeOggVorbis(void * psOggVorbisFileVoid, char * pDecodeBuffer, const REUInt32 ulBufferSize, const REUInt32 ulChannels)
{
	REUInt32 bytesDone = 0;
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
	OggVorbis_File * psOggVorbisFile = (OggVorbis_File *)psOggVorbisFileVoid;
	bool emptyBuffer = true;
	while (emptyBuffer)
	{
		int currentSection = 0;
		const long decodeSize = ov_read(psOggVorbisFile, pDecodeBuffer + bytesDone, (int)(ulBufferSize - bytesDone), 0, 2, 1, &currentSection);
		if (decodeSize > 0)
		{
			bytesDone += decodeSize;
			if (bytesDone >= ulBufferSize)
			{
				emptyBuffer = false;
			}
		}
		else
		{
			emptyBuffer = false;
		}
	}
	
	if (ulChannels == 6)
	{
		REInt16 * pSamples = (REInt16*)pDecodeBuffer;
		for (unsigned long samples = 0; samples < (ulBufferSize >> 1); samples += 6)
		{
			// WAVEFORMATEXTENSIBLE Order : FL, FR, FC, LFE, RL, RR
			// OggVorbis Order            : FL, FC, FR,  RL, RR, LFE
			
			SWAP_SHORT(pSamples[samples + 1], pSamples[samples + 2]);
			SWAP_SHORT(pSamples[samples + 3], pSamples[samples + 5]);
			SWAP_SHORT(pSamples[samples + 4], pSamples[samples + 5]);
		}
	}
	
#endif
	return bytesDone;
}

void REOGGVorbisSound::fillBuffersWithDecodedAudioData()
{
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__
	OggVorbis_File * ogStream = (OggVorbis_File *)_oggMemoryFile.oggStream;
	for (int i = 0; i < OGG_BUFFERS_COUNT; i++)
	{
		_bytesWritten = this->decodeOggVorbis(ogStream, (char*)_decodeBuffer->buffer(), _decodeBuffer->size(), _channels);
		if (_bytesWritten)
		{
#ifdef __RE_USING_OPENAL__			
			alBufferData(_buffers[i], (ALenum)_format, _decodeBuffer->buffer(), (ALsizei)_bytesWritten, (ALsizei)_frequency);
			alSourceQueueBuffers(_source, 1, &_buffers[i]);
#endif			
		}
	}
#endif
}

REBOOL REOGGVorbisSound::initWithData(const REData & soundFileData)
{
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__	
	this->clear();
	 
	_oggMemoryFile.oggFileData = new REData(soundFileData);
	if (_oggMemoryFile.oggFileData == NULL) { return false; }
	
	_oggMemoryFile.oggStream = new OggVorbis_File();
	if (_oggMemoryFile.oggStream == NULL) 
	{
		delete _oggMemoryFile.oggFileData;
		_oggMemoryFile.oggFileData = NULL;
		return false;
	}
	
	ov_callbacks vorbisCallbacks;
	vorbisCallbacks.read_func = REOGGVorbisSoundPrivate::VorbisRead;
	vorbisCallbacks.close_func = REOGGVorbisSoundPrivate::VorbisClose;
	vorbisCallbacks.seek_func = REOGGVorbisSoundPrivate::VorbisSeek;
	vorbisCallbacks.tell_func = REOGGVorbisSoundPrivate::VorbisTell;
	
	_oggMemoryFile.oggData = _oggMemoryFile.oggFileData->bytes();
	_oggMemoryFile.dataSize = _oggMemoryFile.oggFileData->size();
		
	OggVorbis_File * ogStream = (OggVorbis_File *)_oggMemoryFile.oggStream;
	if (ov_open_callbacks(&_oggMemoryFile, ogStream, NULL, 0, vorbisCallbacks) == 0)
	{
		this->getVorbisInformation();
		
		if (_format == 0)
		{
			ov_clear(ogStream);
			delete ogStream;
			_oggMemoryFile.oggStream = NULL;
			delete _oggMemoryFile.oggFileData;
			_oggMemoryFile.oggFileData = NULL;
			return false;
		}
		
		_decodeBuffer = new REBuffer((REUInt32)_bufferSize);
		if (_decodeBuffer == NULL)
		{
			ov_clear(ogStream);
			delete ogStream;
			_oggMemoryFile.oggStream = NULL;
			delete _oggMemoryFile.oggFileData;
			_oggMemoryFile.oggFileData = NULL;
			return false;
		}
#ifdef __RE_USING_OPENAL__		
		this->generateAndSetupALResources();
#endif		
		this->fillBuffersWithDecodedAudioData();
		_totalBuffersProcessed = 0;
		return true;
	}
#endif
	return false;
}

void REOGGVorbisSound::clear()
{
#ifndef __RE_NO_OGG_VORBIS_SOUND_PRIVATE__	
#ifdef __RE_USING_OPENAL__	
	if (_source > 0) 
	{
		alSourcei(_source, AL_BUFFER, 0);
		ALuint sources[1] = { _source };
		alDeleteSources(1, sources);
	
		alDeleteBuffers(OGG_BUFFERS_COUNT, _buffers);
		for (REUInt32 i = 0; i < OGG_BUFFERS_COUNT; i++) { _buffers[i] = 0; }
	}
#endif
	
	OggVorbis_File * ogStream = (OggVorbis_File *)_oggMemoryFile.oggStream;
	if (ogStream)
	{
		ov_clear(ogStream);
		delete ogStream;
		_oggMemoryFile.oggStream = NULL;
	}
	
	if (_oggMemoryFile.oggFileData) 
	{
		delete _oggMemoryFile.oggFileData;
		_oggMemoryFile.oggFileData = NULL;
	}
	
	_oggMemoryFile.oggData = NULL;
	_oggMemoryFile.dataSize = 0;
	_oggMemoryFile.dataRead = 0;
	
	if (_decodeBuffer) 
	{
		delete _decodeBuffer;
		_decodeBuffer = NULL;
	}
#ifdef __RE_USING_OPENAL__	
	_buffer = 0;
	_source = 0;
#endif	
	_totalBuffersProcessed = 0;
	_frequency = 0;
	_channels = 0;
	_format = 0;
	_bufferSize = 0;
	_bytesWritten = 0;
	_lastDecodeTime = 0.0;
	_volume = 1.0f;
	_isLooped = false;
#endif
}

REOGGVorbisSound::REOGGVorbisSound() : REObject(),
#ifdef __RE_USING_OPENAL__
	REAudioOpenAL(),
	_buffer(0),
	_source(0),
#endif
	_totalBuffersProcessed(0),
	_frequency(0),
	_channels(0),
	_format(0),
	_bufferSize(0),
	_bytesWritten(0),
	_lastDecodeTime(0.0),
	_decodeBuffer(NULL),
	_volume(1.0f),
	_isLooped(false)
{
	_oggMemoryFile.oggStream = NULL;
	_oggMemoryFile.oggFileData = NULL;
	_oggMemoryFile.dataRead = 0;
#ifdef __RE_USING_OPENAL__	
	for (REUInt32 i = 0; i < OGG_BUFFERS_COUNT; i++) 
	{
		_buffers[i] = 0;
	}
#endif	
}

REOGGVorbisSound::~REOGGVorbisSound()
{
	this->stop();
	
	this->clear();
}

REBOOL REOGGVorbisSound::isValidData(const REData & data)
{
	if ( data.size() > 4 )
	{
		const char * dataString = (const char *)data.bytes();
		return ( strncmp(dataString, "OggS", 4) == 0 );
	}
	return false;
}
