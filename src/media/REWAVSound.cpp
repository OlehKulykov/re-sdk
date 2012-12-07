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


#include "../../include/remedia/REWAVSound.h"

#ifdef __RE_USING_OPENAL__		
REBOOL REWAVSound::LoadPcmWavData(const REUByte * data, ALuint * buffer, ALuint * source)
{
	*buffer = 0;
	*source = 0;
	ALenum format = 0;
	ALsizei freq = 0;
	REUInt32 subChunk1Size = 0;
	REUInt32 subChunk2Size = 0;
	REUInt32 fileOffset = 16;
	REUInt16 audioFormat = 0; 
	REUInt16 channels = 0;
	REUInt16 bps = 0;
	char str4[5] = { 0 };
	memcpy(&subChunk1Size, &data[fileOffset], sizeof(REUInt32));
	fileOffset += sizeof(REUInt32);
	if (subChunk1Size >= 16)
	{
		memcpy(&audioFormat, &data[fileOffset], sizeof(REUInt16));
		fileOffset += sizeof(REUInt16);
		if (audioFormat == 1)
		{
			memcpy(&channels, &data[fileOffset],sizeof(REUInt16));
			fileOffset += sizeof(REUInt16);
			
			memcpy(&freq, &data[fileOffset], sizeof(REUInt32));
			fileOffset += sizeof(REUInt32) + 6;
			
			memcpy(&bps, &data[fileOffset], sizeof(REUInt16));
			fileOffset += sizeof(REUInt16);
			if (channels == 1)
			{
				format = (bps == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
			}
			else
			{
				format = (bps == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
			}
			memcpy(&str4, &data[fileOffset], 4);
			fileOffset += 4;
			if (strcmp(str4, "data") == 0)
			{
				memcpy(&subChunk2Size, &data[fileOffset], sizeof(REUInt32));
				fileOffset += 4;
				alGenBuffers(1, buffer);
				alBufferData(*buffer, format, &data[fileOffset], subChunk2Size, freq);
				alGenSources(1, source);
				ALfloat sourcePos[] = {0.0f, 0.0f, 0.0f};
				ALfloat sourceVel[] = {0.0f, 0.0f, 0.0f};
				alSourcef(*source, AL_PITCH, 1.0f);
				alSourcef(*source, AL_GAIN, 1.0f);
				alSourcefv(*source, AL_POSITION, sourcePos);
				alSourcefv(*source, AL_VELOCITY, sourceVel);
				alSourcei(*source, AL_LOOPING, false); 
				alSourcei(*source, AL_BUFFER, *buffer);
				return true;
			}
		}
	}
	return false;
}
#endif

REBOOL REWAVSound::InitWithData(const REData & soundFileData)
{
	this->Clear();	
	
	if ( soundFileData.IsEmpty() ) 
	{
		return false; 
	}
	
#ifdef __RE_USING_OPENAL__	
	return REWAVSound::LoadPcmWavData(soundFileData.GetBytes(), &_buffer, &_source);
#endif
	
	return false;
}
 
REBOOL REWAVSound::Play()
{
#ifdef __RE_USING_OPENAL__
	if ( _source )
	{
		alSourcePlay(_source);
		return (alGetError() == AL_NO_ERROR);
	}
#endif
	return false;
}

REBOOL REWAVSound::IsPlaying() const
{
#ifdef __RE_USING_OPENAL__
	if ( _source )
	{
		ALenum state;
		alGetSourcei(_source, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}
#endif
	return false;
}

REBOOL REWAVSound::Pause()
{
#ifdef __RE_USING_OPENAL__
	if ( _source )
	{
		alSourcePause(_source); 
		return (alGetError() == AL_NO_ERROR);
	}
#endif
	return false;
}

REBOOL REWAVSound::Stop()
{
#ifdef __RE_USING_OPENAL__
	if ( _source )
	{ 
		alSourceStop(_source);
		return (alGetError() == AL_NO_ERROR);
	}
#endif
	return false;
}

REBOOL REWAVSound::SetLooped(const REBOOL isLooped)
{
#ifdef __RE_USING_OPENAL__
	if ( _source )
	{
		alSourcei(_source, AL_LOOPING, isLooped);
		if (alGetError() == AL_NO_ERROR)
		{
			_isLooped = isLooped;
			return true;
		}
	}
#endif
	
	return false;
}

REBOOL REWAVSound::IsLooped() const
{
	return _isLooped;
}

REBOOL REWAVSound::SetVolume(const REFloat32 newVolume)
{
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
		return (alGetError() == AL_NO_ERROR);
	}
#endif
	
	return false;
}

const REFloat32 REWAVSound::GetVolume() const
{
	return _volume;
}

REWAVSound::REWAVSound() : REObject(),
#ifdef __RE_USING_OPENAL__
REAudioOpenAL(),
_buffer(0),
_source(0),
#endif
_volume(1.0f),
_isLooped(false)
{
	
}

void REWAVSound::Clear()
{
#ifdef __RE_USING_OPENAL__	
	if ( _source )
	{
		this->Stop();
		alDeleteBuffers(1, &_buffer);
		alDeleteSources(1, &_source); 
	}
#endif	
}

REWAVSound::~REWAVSound()
{
	this->Clear();
}

REBOOL REWAVSound::IsValidData(const REData & data)
{
	if ( data.GetSize() > 12 )
	{
		const char * dataString = (const char *)data.GetBytes();
		REBOOL isValidData = ( (strncmp(dataString, "RIFF", 4) == 0) && (strncmp(&dataString[8], "WAVE", 4) == 0) );
		return isValidData;
	}
	return false;
}

