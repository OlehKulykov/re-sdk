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


#include "../../include/remedia/private/REOGGTheoraPlayerPrivate.h"

#ifndef __RE_OS_WINDOWS__
#include <sys/time.h>
#endif


/*
./gas-preprocessor.pl /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/clang -arch armv7 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.0.sdk -E armidct.S -o armidct.out
*/

#define __TIME_TEST__
//#undef __TIME_TEST__

#define __THEORA_DECODE_TIME_TEST__

//THEORA_DISABLE_FLOAT

#ifdef __TIME_TEST__
#include "../../include/recore/RETime.h"
#endif

/*
#define __RE_USING_TREMOR_LIBRARY__
#define __RE_USING_VORBIS_LIBRARY__
*/

#ifdef __RE_OS_WINDOWS__

#include <time.h>
#include <windows.h> 

struct timezone2 
{
	REInt32  tz_minuteswest; /* minutes W of Greenwich */
	bool  tz_dsttime;     /* type of dst correction */
};

struct timeval2 
{
	REInt32    tv_sec;         /* seconds */
	REInt32    tv_usec;        /* microseconds */
};

int gettimeofday(struct timeval2 *tv/*in*/, struct timezone2 *tz/*in*/)
{
	FILETIME ft;
	REInt32 tmpres = 0;
	TIME_ZONE_INFORMATION tz_winapi;
	int rez = 0;
	
	ZeroMemory(&ft, sizeof(ft));
	ZeroMemory(&tz_winapi, sizeof(tz_winapi));
	
    GetSystemTimeAsFileTime(&ft);
	
    tmpres = ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
	
    /*converting file time to unix epoch*/
    tmpres /= 10;  /*convert into microseconds*/
    tmpres -= (REUInt64)11644473600000000; 
    tv->tv_sec = (REInt32)(0.000001 * tmpres);
    tv->tv_usec =(tmpres % 1000000);
	
	
    //_tzset(),don't work properly, so we use GetTimeZoneInformation
    rez = GetTimeZoneInformation(&tz_winapi);
    tz->tz_dsttime = (rez == 2) ? true : false;
    tz->tz_minuteswest = tz_winapi.Bias + ((rez == 2) ? tz_winapi.DaylightBias : 0);
	
	return 0;
}

#endif

double REOGGTheoraPlayerPrivate::getTime()
{
	static ogg_int64_t last = 0;
	static ogg_int64_t up = 0;
#ifdef __RE_OS_WINDOWS__
	struct timeval2 tv;
#else
	struct timeval tv;
#endif	
	gettimeofday(&tv, 0);
	const ogg_int64_t now = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	if (_audioFDTimerCalibrate == -1) 
	{
		_audioFDTimerCalibrate = last = now;
	}
	
	const ogg_int64_t now_min_last = now - last;
	if (now_min_last > 1000)
	{
		_audioFDTimerCalibrate += now_min_last;
	}
	last = now;
	
	if ((now - up) > 200)
	{
		up = now;
	}
	
	return (0.001 * (now - _audioFDTimerCalibrate));
}

int REOGGTheoraPlayerPrivate::bufferData(REData * inData, ogg_sync_state * oy)
{
	char * buffer = ogg_sync_buffer(oy, 65536);
	const long sizeReaded = (long)inData->fileRead(buffer, 65536); /*fread(buffer, elementSize, 1, in) * elementSize*/;
	ogg_sync_wrote(oy, sizeReaded);
	return (int)sizeReaded;
}

void REOGGTheoraPlayerPrivate::queuePage(ogg_page * page)
{
	if (theora_p) ogg_stream_pagein(&_theoraOggStreamState, page);
	if (vorbis_p) ogg_stream_pagein(&_vorbisOggStreamState, page);
}

void REOGGTheoraPlayerPrivate::writeVideo()
{
	if (_texture && _colorConversionFunction) 
	{
		th_ycbcr_buffer yuv;
		th_decode_ycbcr_out(_theoraDecoderContext, yuv);
		
		_colorConversionFunction((REUByte *)_frameRGBBuffer.getBuffer(), 
								 yuv[0].data, 
								 yuv[1].data, 
								 yuv[2].data, 
								 _theoraInfo.frame_width, 
								 _theoraInfo.frame_height,
								 yuv[0].stride,
								 yuv[1].stride,
								 _theoraInfo.frame_width * 3,
								 _colorConversionTable,
								 0);

		_texture->Update((REUByte *)_frameRGBBuffer.getBuffer(), REImagePixelFormatR8G8B8, _theoraInfo.frame_width, _theoraInfo.frame_height);
		
	}
}

void REOGGTheoraPlayerPrivate::decodeVideo()
{
	if (theora_p) 
	{
		while (!_isVideoBufferReady)
		{
			if (ogg_stream_packetout(&_theoraOggStreamState, &_oggPacket) > 0)
			{
				if (_ppInc)
				{
					_ppLevel += _ppInc;
					th_decode_ctl(_theoraDecoderContext, TH_DECCTL_SET_PPLEVEL, &_ppLevel, sizeof(_ppLevel));
					_ppInc = 0;
				}
				
				if (_oggPacket.granulepos >= 0)
				{
					th_decode_ctl(_theoraDecoderContext, TH_DECCTL_SET_GRANPOS, &_oggPacket.granulepos, sizeof(_oggPacket.granulepos));
				}
				
				if (th_decode_packetin(_theoraDecoderContext, &_oggPacket, &_videoBufferGranulePos) == 0)
				{
					_videoBufferTime = th_granule_time(_theoraDecoderContext, _videoBufferGranulePos);
					if(_videoBufferTime >= this->getTime() )
					{
						_isVideoBufferReady = true;
					}
					else
					{
						_ppInc = 0;
						if (_ppLevel > 0)
						{
							_ppInc = -1;
						}
					}
				}
			}
			else
			{
				break;
			}
		}
	}
}

void REOGGTheoraPlayerPrivate::convertSamples(const int samples, float ** pcm, void * buf, const int channels)
{
	for (int i = 0; i < channels; i++)
	{
		ogg_int16_t * ptr = &((ogg_int16_t *)buf)[i];
		float * mono = pcm[i];
		for (int j = 0; j < samples; j++)
		{
			const int val = (int)(*mono++ * 32767.0f);
			if (val > 32767) *ptr = 32767;
			else if (val < -32768) *ptr = -32768;
			else *ptr = (ogg_int16_t)val;
			
			ptr += channels;
		}
	}
}

void REOGGTheoraPlayerPrivate::decodeAudio()
{
	while (!this->isAudioBufferReady())
	{
		float ** pcm = NULL;
		/* if there's pending, decoded audio, grab it */
		const int samples = vorbis_synthesis_pcmout(&_vorbisDSPState, &pcm);
		if (samples > 0)
		{
			const REUInt32 dataSize = samples * _vorbisInfo.channels * sizeof(ogg_int16_t);
			if ((_soundPCMDataMemorySize - _soundPCMDataSize) < dataSize)
			{
				const REUInt32 newDataSize = _soundPCMDataMemorySize + dataSize;
				REUByte * newData = new REUByte[newDataSize];
				memcpy(newData, _soundPCMData, _soundPCMDataMemorySize);
				delete _soundPCMData;
				_soundPCMData = newData;
				_soundPCMDataMemorySize = newDataSize;
			}
			
			REOGGTheoraPlayerPrivate::convertSamples(samples, pcm, &_soundPCMData[_soundPCMDataSize], _vorbisInfo.channels);
			_soundPCMDataSize += dataSize;
			vorbis_synthesis_read(&_vorbisDSPState, samples);
		}
		else // no pending audio; is there a pending packet to decode?
		{
			if (ogg_stream_packetout(&_vorbisOggStreamState, &_oggPacket) > 0)
			{
				if (vorbis_synthesis(&_vorbisBlock, &_oggPacket) == 0) // test for success!
				{
					vorbis_synthesis_blockin(&_vorbisDSPState, &_vorbisBlock);
				}
			}
			else  // we need more data; break out to suck in another page 
			{
				break;
			}
		}
	}
}

void REOGGTheoraPlayerPrivate::playDecodedAudio()
{
	if (_audioPlayBackStarted)
	{
		ALint processed = 0;
		alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &processed);
		if (processed && (_soundPCMDataSize >= (processed * _alBufferSize)))
		{
			// we have decoded PCM data
			REUInt32 soundPCMDataReaded = 0;
			while (processed--)
			{
				ALuint buffer = 0;
				alSourceUnqueueBuffers(_alSource, 1, &buffer);
				alBufferData(buffer, _alPCMFormat, &_soundPCMData[soundPCMDataReaded], _alBufferSize, _vorbisInfo.rate);
				alSourceQueueBuffers(_alSource, 1, &buffer);
				soundPCMDataReaded += _alBufferSize;
			}
			
			_soundPCMDataSize = _soundPCMDataSize - soundPCMDataReaded;
			memcpy(_soundPCMData, &_soundPCMData[soundPCMDataReaded], _soundPCMDataSize);
			
			ALenum state = AL_NONE;
			alGetSourcei(_alSource, AL_SOURCE_STATE, &state);
			if (state != AL_PLAYING)
			{
				alSourcePlay(_alSource);
			}
		}
	}
	else if (this->isAudioBufferReady())
	{
		REUInt32 soundPCMDataReaded = 0;
		for(REUInt32 i = 0; i < NUM_BUFFERS; i++)
		{
			alBufferData(_alBuffers[i], _alPCMFormat, &_soundPCMData[soundPCMDataReaded], _alBufferSize, _vorbisInfo.rate);
			alSourceQueueBuffers(_alSource, 1, &_alBuffers[i]);
			soundPCMDataReaded += _alBufferSize;
		}
		alSourcePlay(_alSource);
		if (soundPCMDataReaded)
		{
			_soundPCMDataSize = _soundPCMDataSize - soundPCMDataReaded;
			memcpy(_soundPCMData, &_soundPCMData[soundPCMDataReaded], _soundPCMDataSize);
		}
		_audioPlayBackStarted = true;
	}
}

void REOGGTheoraPlayerPrivate::update(const RETimeInterval currentTime)
{		
	if (vorbis_p) 
	{	
		this->decodeAudio();	
		
		this->playDecodedAudio();	
	}

#ifdef __THEORA_DECODE_TIME_TEST__
	static RETimeInterval totalTime = 0.0;
	const RETimeInterval time1 = RETime::time();
#endif	
	this->decodeVideo();
#ifdef __THEORA_DECODE_TIME_TEST__
	totalTime += RETime::time() - time1;
#endif
	
	if ( (!_isVideoBufferReady) || (!this->isAudioBufferReady()) )
	{
		/* no data yet for somebody.  Grab another page */
		REOGGTheoraPlayerPrivate::bufferData(&_inData, &_oggSyncState);
		while (ogg_sync_pageout(&_oggSyncState, &_oggPage) > 0)
		{
			this->queuePage(&_oggPage);
		}
	}
	
	/* are we at or past time for this video frame? */
	if (_isVideoBufferReady && (_videoBufferTime <= this->getTime()) )
	{
		this->writeVideo();
		_isVideoBufferReady = false;
	}
	
	if ( (!_isVideoBufferReady) && (!this->isAudioBufferReady()) && _inData.isEndOfFile() /*feof(infile)*/) 
	{
		ALenum state;
		alGetSourcei(_alSource, AL_SOURCE_STATE, &state);
		if(state != AL_PLAYING)
		{
#ifdef __THEORA_DECODE_TIME_TEST__
			printf("\nTHEORA_DECODE_TIME_TEST DecodeVideo total time = %f", totalTime);
#endif			
			printf("\nstate != AL_PLAYING");
		}
		// STOP
		_isPlaying = false;
		//REMainLoopObjectsStorage::GetDefaultStorage()->RemoveObject(this);
		//printf("\nOGGTheoraPlayer: DONE PLAYING");
		/*break*/ 
		return;
	}
}

REBOOL REOGGTheoraPlayerPrivate::parseHeaders()
{
	int stateflag = 0;
	while (!stateflag)
	{
		if (REOGGTheoraPlayerPrivate::bufferData(&_inData, &_oggSyncState) == 0)
		{
			break;
		}
		while (ogg_sync_pageout(&_oggSyncState, &_oggPage) > 0)
		{
			ogg_stream_state test;
			/* is this a mandated initial header? If not, stop parsing */
			if (!ogg_page_bos(&_oggPage))
			{
				/* don't leak the page; get it into the appropriate stream */
				this->queuePage(&_oggPage);
				stateflag = 1;
				break;
			}
			
			ogg_stream_init(&test, ogg_page_serialno(&_oggPage));
			ogg_stream_pagein(&test, &_oggPage);
			ogg_stream_packetout(&test, &_oggPacket);
			
			if (!theora_p)
			{
				if (th_decode_headerin(&_theoraInfo, &_theoraComment, &_theoraSetupInfo, &_oggPacket) >= 0)
				{
					/* it is theora */
					memcpy(&_theoraOggStreamState, &test, sizeof(test));
					theora_p = 1;
				}
			}
			else if (!vorbis_p)
			{
				if (vorbis_synthesis_headerin(&_vorbisInfo, &_vorbisComment, &_oggPacket) >= 0)
				{
					/* it is vorbis */
					memcpy(&_vorbisOggStreamState, &test, sizeof(test));
					vorbis_p = 1;
					
					switch (_vorbisInfo.channels) 
					{
						case 1:
							_alPCMFormat = AL_FORMAT_MONO16;
							// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
							_alBufferSize = _vorbisInfo.rate >> 1;
							// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
							_alBufferSize -= (_alBufferSize % 2);
							break;
						case 2:
							_alPCMFormat = AL_FORMAT_STEREO16;
							// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
							_alBufferSize = _vorbisInfo.rate;
							// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
							_alBufferSize -= (_alBufferSize % 4);
							break;
						case 4:
							_alPCMFormat = alGetEnumValue("AL_FORMAT_QUAD16");
							// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
							_alBufferSize = _vorbisInfo.rate * 2;
							// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
							_alBufferSize -= (_alBufferSize % 8);
							break;
						case 6:
							_alPCMFormat = alGetEnumValue("AL_FORMAT_51CHN16");
							// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
							_alBufferSize = _vorbisInfo.rate * 3;
							// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
							_alBufferSize -= (_alBufferSize % 12);
							break;
						default:
							break;
					}
					
					if (_alBufferSize > 0)
					{
						_soundPCMDataMemorySize = (NUM_BUFFERS + 1) * _alBufferSize;
						_soundPCMData = new REUByte[_soundPCMDataMemorySize];
					}
				}
			}
			else
			{
				/* whatever it is, we don't care about it */
				ogg_stream_clear(&test);
			} 
		}
	}
	
	/* we're expecting more header packets. */
	while ((theora_p && (theora_p < 3)) || (vorbis_p && (vorbis_p < 3)))
	{
		int ret = 0;
		/* look for further theora headers */
		while (theora_p && (theora_p < 3) && (ret = ogg_stream_packetout(&_theoraOggStreamState, &_oggPacket)))
		{
			if (ret < 0)
			{
				return false;
			}
			if (!th_decode_headerin(&_theoraInfo, &_theoraComment, &_theoraSetupInfo, &_oggPacket))
			{
				return false;
			}
			theora_p++;
		}
		
		/* look for more vorbis header packets */
		while (vorbis_p && (vorbis_p < 3) && (ret = ogg_stream_packetout(&_vorbisOggStreamState, &_oggPacket)))
		{
			if (ret < 0)
			{
				//fprintf(stderr,"Error parsing Vorbis stream headers; corrupt stream?\n");
				return false;
			}
			if (vorbis_synthesis_headerin(&_vorbisInfo, &_vorbisComment, &_oggPacket))
			{
				//fprintf(stderr,"Error parsing Vorbis stream headers; corrupt stream?\n");
				return false;
			}
			vorbis_p++;
			if (vorbis_p == 3) break;
		}
		/* The header pages/packets will arrive before anything else we
		 care about, or the stream is not obeying spec */
		
		if (ogg_sync_pageout(&_oggSyncState, &_oggPage) > 0)
		{
			this->queuePage(&_oggPage); /* demux into the appropriate stream */
		}
		else
		{
			if (REOGGTheoraPlayerPrivate::bufferData(&_inData, &_oggSyncState) == 0)
			{
				//fprintf(stderr,"End of file while searching for codec headers.\n");
				return false;
			}
		}
	}
	
	return true;
}

REBOOL REOGGTheoraPlayerPrivate::initDecoders()
{
	if (theora_p)
	{
		_theoraDecoderContext = th_decode_alloc(&_theoraInfo, _theoraSetupInfo);
		switch(_theoraInfo.pixel_fmt)
		{
			case TH_PF_420:
				_colorConversionFunction = REYUVtoRGB::YUV420toRGB888;
				_colorConversionTable = REYUVtoRGB::getYUVtoRGB565Table();
				break;
			case TH_PF_422:
				_colorConversionFunction = REYUVtoRGB::YUV422toRGB888;
				_colorConversionTable = REYUVtoRGB::getYUVtoRGB565Table();
				break;
			case TH_PF_444: 
				_colorConversionFunction = REYUVtoRGB::YUV444toRGB888;
				_colorConversionTable = REYUVtoRGB::getYUVtoRGB565Table();
				break;
			case TH_PF_RSVD:
			default:
				//printf(" video\n  (UNKNOWN Chroma sampling!)\n");
				break;
		}
		int ppLevelMax = 0;
		th_decode_ctl(_theoraDecoderContext, TH_DECCTL_GET_PPLEVEL_MAX, &ppLevelMax, sizeof(ppLevelMax));
		_ppLevel = ppLevelMax;
		th_decode_ctl(_theoraDecoderContext,TH_DECCTL_SET_PPLEVEL, &_ppLevel, sizeof(_ppLevel));
		_ppInc = 0;
	}
	else
	{
		/* tear down the partial theora setup */
		th_info_clear(&_theoraInfo);
		th_comment_clear(&_theoraComment);
		return false;
	}
	
	th_setup_free(_theoraSetupInfo);
	
	if (vorbis_p)
	{
		vorbis_synthesis_init(&_vorbisDSPState, &_vorbisInfo);
		vorbis_block_init(&_vorbisDSPState, &_vorbisBlock);
	}
	else
	{
		/* tear down the partial vorbis setup */
		vorbis_info_clear(&_vorbisInfo);
		vorbis_comment_clear(&_vorbisComment);
		return false;
	}
	
	return true;
}

REBOOL REOGGTheoraPlayerPrivate::initWithData(const REData & soundFileData)
{
	_inData = soundFileData;
	
	/* start up Ogg stream synchronization layer */
	ogg_sync_init(&_oggSyncState); 
	
	/* init supporting Vorbis structures needed in header parsing */
	vorbis_info_init(&_vorbisInfo);
	vorbis_comment_init(&_vorbisComment); 
	
	/* init supporting Theora structures needed in header parsing */
	th_comment_init(&_theoraComment);
	th_info_init(&_theoraInfo);
	
	if (!this->parseHeaders())
	{
		return false;
	}
	
	if (!this->initDecoders())
	{
		return false;
	}
	
	_alSource = 0;
	for (REUInt32 i = 0; i < NUM_BUFFERS; i++)
	{
		_alBuffers[i] = 0;
	}
	
	if ( this->isSoundExists() )
	{
		alGenBuffers(NUM_BUFFERS, _alBuffers);
		alGenSources(1, &_alSource);
	}
	
	_frameRGBBuffer.resize(_theoraInfo.frame_width * _theoraInfo.frame_height * 3, false);
	
	_texture = RETextureObject::CreateWithBlankTexture(REImagePixelFormatR8G8B8, _theoraInfo.frame_width, _theoraInfo.frame_height);
	if (_texture == NULL) 
	{
		return false;
	}
	
	//_isPlaying = true;
	//REMainLoopObjectsStorage::GetDefaultStorage()->AddObject(this);
	
	return true;
}

REBOOL REOGGTheoraPlayerPrivate::play()
{
	//_isPlaying = true;
	this->addToMainLoop();
	return false;
}

REBOOL REOGGTheoraPlayerPrivate::isPlaying() const
{
	return false;
}

REBOOL REOGGTheoraPlayerPrivate::pause()
{
	return false;
}

REBOOL REOGGTheoraPlayerPrivate::stop()
{
	return false;
}

REBOOL REOGGTheoraPlayerPrivate::setLooped(const REBOOL isLooped)
{
	return false;
}

REBOOL REOGGTheoraPlayerPrivate::isLooped() const /* by default is not looped */
{
	return false;
}

REBOOL REOGGTheoraPlayerPrivate::setVolume(const REFloat32 newVolume)
{
	return false;
}

const REFloat32 REOGGTheoraPlayerPrivate::getVolume() const
{
	return 0.0f;
}

REOGGTheoraPlayerPrivate::REOGGTheoraPlayerPrivate() : REObject()
#ifdef __RE_USING_OPENAL__
,REAudioOpenAL()
#endif
	,_colorConversionTable(NULL),
	_texture(NULL),
	_soundPCMData(NULL),
	_theoraDecoderContext(NULL),
	_theoraSetupInfo(NULL),
	_colorConversionFunction(NULL),
	_videoBufferTime(0.0),
	_videoBufferGranulePos(-1),
	_audioFDTimerCalibrate(-1),
	_alSource(0),
	_soundPCMDataSize(0),
	_soundPCMDataMemorySize(0),
	_alPCMFormat(0),
	_alBufferSize(0),
	_ppLevel(0),
	_ppInc(0),
	theora_p(0),
	vorbis_p(0),
	_isVideoBufferReady(false),
	_audioPlayBackStarted(false),
	_isPlaying(false)
{

}

REOGGTheoraPlayerPrivate::~REOGGTheoraPlayerPrivate()
{
	this->removeFromMainLoop();
	
	if (_texture) 
	{
		_texture->release();
	}
	
	if (vorbis_p)
	{
		ogg_stream_clear(&_vorbisOggStreamState);
		vorbis_block_clear(&_vorbisBlock);
		vorbis_dsp_clear(&_vorbisDSPState);
		vorbis_comment_clear(&_vorbisComment);
		vorbis_info_clear(&_vorbisInfo);
	}
	
	if (theora_p)
	{
		ogg_stream_clear(&_theoraOggStreamState);
		th_decode_free(_theoraDecoderContext);
		th_comment_clear(&_theoraComment);
		th_info_clear(&_theoraInfo);
	}
	
	ogg_sync_clear(&_oggSyncState);
	
	if (_soundPCMData) 
	{
		delete _soundPCMData;
	}
}
