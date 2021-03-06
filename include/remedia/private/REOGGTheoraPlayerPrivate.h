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


#ifndef __REOGGTHEORAPLAYERPRIVATE_H__
#define __REOGGTHEORAPLAYERPRIVATE_H__

#include "../../recore/RECommonHeader.h"
#include "../../recore/REData.h"
#include "../../recore/REMainLoopUpdatable.h"
#include "../../recore/REYUVtoRGB.h"
#include "../../recore/REBuffer.h"

#include "../../regui/RETexture.h"

#include "../../remedia/IREMediaPlayable.h"

#ifdef __RE_USING_SYSTEM_OGG_LIBRARY__
#include <ogg/ogg.h>
#endif

//#ifdef __RE_USING_ADITIONAL_OGG_LIBRARY__
#include "../../../src/addlibs/ogg.h"
//#endif

#ifdef __RE_USING_SYSTEM_THEORA_LIBRARY__
#include <theora/theoradec.h>
#endif

//#ifdef __RE_USING_ADITIONAL_THEORA_LIBRARY__
#include "../../../src/addlibs/theora.h"
//#endif

//#ifdef __RE_USING_ADITIONAL_VORBIS_LIBRARY__
#include "../../../src/addlibs/vorbis.h"
//#endif

#ifdef __RE_USING_SYSTEM_VORBIS_LIBRARY__
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#endif

#include <math.h>

#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
#include <arm_neon.h>
#ifdef __RE_OS_IPHONE__
#include <Accelerate/Accelerate.h>
#endif
#endif

//#ifdef __RE_USING_OPENAL__
#include "../REAudioOpenAL.h"
//#endif

#define NUM_BUFFERS 4

class __RE_PUBLIC_CLASS_API__ REOGGTheoraPlayerPrivate : public REObject,
#ifdef __RE_USING_OPENAL__
public REAudioOpenAL,
#endif
public IREMediaPlayable, 
public REMainLoopUpdatable
{
private:
	const REUInt32 * _colorConversionTable;
	RETexture * _texture;
	REUByte * _soundPCMData;
	th_dec_ctx * _theoraDecoderContext;
	th_setup_info * _theoraSetupInfo;
	REYUVtoRGBConversionFunc _colorConversionFunction;
	REData _inData;
	REBuffer _frameRGBBuffer;
	ogg_sync_state _oggSyncState;
	vorbis_info _vorbisInfo;
	vorbis_comment _vorbisComment;
	th_info _theoraInfo;
	th_comment _theoraComment;
	ogg_stream_state _vorbisOggStreamState;
	ogg_stream_state _theoraOggStreamState;
	ogg_packet _oggPacket;
	vorbis_dsp_state _vorbisDSPState;
	vorbis_block _vorbisBlock;
	ogg_page _oggPage;
	double _videoBufferTime;
	ogg_int64_t _videoBufferGranulePos;
	ogg_int64_t _audioFDTimerCalibrate;
	ALuint _alBuffers[NUM_BUFFERS];
	ALuint _alSource;
	REUInt32 _soundPCMDataSize;
	REUInt32 _soundPCMDataMemorySize;
	ALenum _alPCMFormat;
	ALsizei _alBufferSize;
	int _ppLevel;
	int _ppInc;
	int theora_p;
	int vorbis_p;
	bool _isVideoBufferReady;
	REBOOL _audioPlayBackStarted;
	REBOOL _isPlaying;
	
	void queuePage(ogg_page * page);
	REBOOL parseHeaders();
	REBOOL initDecoders();
	void decodeVideo();
	void decodeAudio();
	void playDecodedAudio();
	double getTime();
	void writeVideo();
	
	REBOOL isSoundExists() const { return (vorbis_p != 0); }
	REBOOL isAudioBufferReady() const { return (_soundPCMDataSize >= (NUM_BUFFERS * _alBufferSize)); }
	REUInt32 getFrameWidth() const { return _theoraInfo.frame_width; }
	REUInt32 getFrameHeight() const { return _theoraInfo.frame_height; }
	
	static int bufferData(REData * inData, ogg_sync_state * oy);
	
	static void convertSamples(const int samples, float ** pcm, void * buf, const int channels);
public:
	virtual void update(const RETimeInterval currentTime);
	
	virtual REBOOL play();
	virtual REBOOL isPlaying() const;
	virtual REBOOL pause();
	virtual REBOOL stop();
	virtual REBOOL setLooped(const REBOOL isLooped);
	virtual REBOOL isLooped() const; /* by default is not looped */
	virtual REBOOL setVolume(const REFloat32 newVolume);
	virtual const REFloat32 getVolume() const;
	
	RETexture * getTexture() const { return _texture; }
	
	REBOOL initWithData(const REData & soundFileData);
	
	REOGGTheoraPlayerPrivate();
	virtual ~REOGGTheoraPlayerPrivate();
	
	
private:
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
#endif	
};


#endif /* __REOGGTHEORAPLAYERPRIVATE_H__ */

