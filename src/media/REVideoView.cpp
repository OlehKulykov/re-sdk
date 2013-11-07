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


REBOOL REVideoView::play()
{
	if (_fields) 
	{
		REOGGTheoraPlayerPrivate * player = (REOGGTheoraPlayerPrivate *)_fields;
		this->setTexture(player->getTexture());
		return player->play();
	}
	return false;
}

REBOOL REVideoView::isPlaying() const
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->isPlaying();
	}
	return false;
}

REBOOL REVideoView::pause()
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->pause();
	}
	return false;
}

REBOOL REVideoView::stop()
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->stop();
	}
	return false;
}

REBOOL REVideoView::setLooped(const REBOOL isLooped)
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->setLooped(isLooped);
	}
	return false;
}

REBOOL REVideoView::isLooped() const /* by default is not looped */
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->isLooped();
	}
	return false;
}

REBOOL REVideoView::setVolume(const REFloat32 newVolume)
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->setVolume(newVolume);
	}
	return false;
}

const REFloat32 REVideoView::getVolume() const
{
	if (_fields) 
	{
		return ((REOGGTheoraPlayerPrivate *)_fields)->getVolume();
	}
	return 0.0f;
}

REBOOL REVideoView::initWithData(const REData & soundFileData)
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
		if (player->initWithData(soundFileData)) 
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




