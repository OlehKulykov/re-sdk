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


#include "../../include/remedia/REAudioPlayer.h"
#include "../../include/remedia/REWAVSound.h"
#include "../../include/remedia/REOGGVorbisSound.h"
#include "../../include/remedia/private/REAudioPlayerPrivate_iphone.h"
#include "../../include/recore/REUserCallBacks.h"



REBOOL REAudioPlayer::play()
{
	if (_media) 
	{
		return _media->play();
	}
	return false;
}

REBOOL REAudioPlayer::isPlaying() const
{
	if (_media) 
	{
		return _media->isPlaying();
	}
	return false;
}

REBOOL REAudioPlayer::pause()
{
	if (_media) 
	{
		return _media->pause();
	}
	return false;
}

REBOOL REAudioPlayer::stop()
{
	if (_media) 
	{
		return _media->stop();
	}
	return false;
}

REBOOL REAudioPlayer::setLooped(const REBOOL isLooped)
{
	if (_media) 
	{
		return _media->setLooped(isLooped);
	}
	return false;
}

REBOOL REAudioPlayer::isLooped() const /* by default is not looped */
{
	if (_media) 
	{
		return _media->isLooped();
	}
	return false;
}

REBOOL REAudioPlayer::setVolume(const REFloat32 newVolume)
{
	if (_media) 
	{
		return _media->setVolume(newVolume);
	}
	return false;
}

const REFloat32 REAudioPlayer::getVolume() const
{
	if (_media) 
	{
		return _media->getVolume();
	}
	return false;	
}

REBOOL REAudioPlayer::initWithData(const REData & soundFileData)
{
	RE_SAFE_DELETE(_media);

#ifdef __RE_OS_IPHONE__	
	REAudioPlayerPrivate_iphone * pl = new REAudioPlayerPrivate_iphone();
	if (pl) 
	{
		if (pl->initWithData(soundFileData)) 
		{
			_media = pl;
			return true;
		}
		delete pl;
	}
#endif
	
	if (REOGGVorbisSound::isValidData(soundFileData)) 
	{
		REOGGVorbisSound * s = new REOGGVorbisSound();
		if (s) 
		{
			if (s->initWithData(soundFileData)) 
			{
				_media = s;
				return true;
			}
			delete s;
		}
	}
	else if (REWAVSound::isValidData(soundFileData)) 
	{
		REWAVSound * s = new REWAVSound();
		if (s) 
		{
			if (s->initWithData(soundFileData)) 
			{
				_media = s;
				return true;
			}
			delete s;
		}
	}
	
	return false;
}

REBOOL REAudioPlayer::initFromFilePath(const REString & filePath)
{
	if (filePath.isEmpty()) 
	{
		return false;
	}
	
	REData d;
	if (d.initFromPath(filePath)) 
	{
		return this->initWithData(d);
	}
	return false;
}

REAudioPlayer::REAudioPlayer() : REObject(),
	_media(NULL)
{
	
}

REAudioPlayer::~REAudioPlayer()
{
	if (_media) 
	{
		delete _media;
	}
}



