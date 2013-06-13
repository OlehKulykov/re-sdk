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


#include "../../include/remedia/REVideoView.h"
#include "../../include/remedia/private/REOGGTheoraPlayerPrivate.h"


const REUInt32 REVideoView::GetClassIdentifier() const
{
	return REView::ClassIdentifier();
}

const REUInt32 REVideoView::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REMD5Generator::GenerateFromString("REVideoView");
	return clasIdentif;
}

REBOOL REVideoView::Play()
{
	if (_fields) 
	{
		REOGGTheoraPlayerPrivate * player = (REOGGTheoraPlayerPrivate *)_fields;
		this->SetTexture(player->GetTexture());
		return player->Play();
	}
	return false;
}

REBOOL REVideoView::IsPlaying() const
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->IsPlaying();
	}
	return false;
}

REBOOL REVideoView::Pause()
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->Pause();
	}
	return false;
}

REBOOL REVideoView::Stop()
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->Stop();
	}
	return false;
}

REBOOL REVideoView::SetLooped(const REBOOL isLooped)
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->SetLooped(isLooped);
	}
	return false;
}

REBOOL REVideoView::IsLooped() const /* by default is not looped */
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->IsLooped();
	}
	return false;
}

REBOOL REVideoView::SetVolume(const REFloat32 newVolume)
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->SetVolume(newVolume);
	}
	return false;
}

const REFloat32 REVideoView::GetVolume() const
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->GetVolume();
	}
	return 0.0f;
}

REBOOL REVideoView::InitWithData(const REData & soundFileData)
{
	if (_fields) 
	{
		REOGGTheoraPlayerPrivate * player = (REOGGTheoraPlayerPrivate *)_fields;
		delete player;
		_fields = NULL;
	}
	
	REOGGTheoraPlayerPrivate * player = new REOGGTheoraPlayerPrivate();
	if (player) 
	{
		if (player->InitWithData(soundFileData)) 
		{
			_fields = player;
			return true;
		}
		delete player;		
	}
	
	return false;
}

REVideoView::REVideoView() : REView(),
	_fields(NULL)
{
	
}

REVideoView::~REVideoView()
{
	if (_fields) 
	{
		REOGGTheoraPlayerPrivate * player = (REOGGTheoraPlayerPrivate *)_fields;
		delete player;
	}
}




