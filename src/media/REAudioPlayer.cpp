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


#include "../../include/remedia/REAudioPlayer.h"
#include "../../include/remedia/REWAVSound.h"
#include "../../include/remedia/REOGGVorbisSound.h"
#include "../../include/remedia/private/REAudioPlayerPrivate_iphone.h"
#include "../../include/recore/REUserCallBacks.h"



REBOOL REAudioPlayer::Play()
{
	if (_media) 
	{
		return _media->Play();
	}
	return false;
}

REBOOL REAudioPlayer::IsPlaying() const
{
	if (_media) 
	{
		return _media->IsPlaying();
	}
	return false;
}

REBOOL REAudioPlayer::Pause()
{
	if (_media) 
	{
		return _media->Pause();
	}
	return false;
}

REBOOL REAudioPlayer::Stop()
{
	if (_media) 
	{
		return _media->Stop();
	}
	return false;
}

REBOOL REAudioPlayer::SetLooped(const REBOOL isLooped)
{
	if (_media) 
	{
		return _media->SetLooped(isLooped);
	}
	return false;
}

REBOOL REAudioPlayer::IsLooped() const /* by default is not looped */
{
	if (_media) 
	{
		return _media->IsLooped();
	}
	return false;
}

REBOOL REAudioPlayer::SetVolume(const REFloat32 newVolume)
{
	if (_media) 
	{
		return _media->SetVolume(newVolume);
	}
	return false;
}

const REFloat32 REAudioPlayer::GetVolume() const
{
	if (_media) 
	{
		return _media->GetVolume();
	}
	return false;	
}

REBOOL REAudioPlayer::InitWithData(const REData & soundFileData)
{
	RE_SAFE_DELETE(_media);

#ifdef __RE_OS_IPHONE__	
	REAudioPlayerPrivate_iphone * pl = new REAudioPlayerPrivate_iphone();
	if (pl) 
	{
		if (pl->InitWithData(soundFileData)) 
		{
			_media = pl;
			return true;
		}
		delete pl;
	}
#endif
	
	if (REOGGVorbisSound::IsValidData(soundFileData)) 
	{
		REOGGVorbisSound * s = new REOGGVorbisSound();
		if (s) 
		{
			if (s->InitWithData(soundFileData)) 
			{
				_media = s;
				return true;
			}
			delete s;
		}
	}
	else if (REWAVSound::IsValidData(soundFileData)) 
	{
		REWAVSound * s = new REWAVSound();
		if (s) 
		{
			if (s->InitWithData(soundFileData)) 
			{
				_media = s;
				return true;
			}
			delete s;
		}
	}
	
	return false;
}

REBOOL REAudioPlayer::InitFromFilePath(const REString & filePath)
{
	if (filePath.IsEmpty()) 
	{
		return false;
	}
	
	REData d;
	if (d.InitFromPath(filePath)) 
	{
		return this->InitWithData(d);
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



