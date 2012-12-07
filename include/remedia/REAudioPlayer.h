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


#ifndef __REAUDIOPLAYER_H__
#define __REAUDIOPLAYER_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REObject.h"
#include "../recore/REData.h"

#include "IREMediaPlayable.h"

class __RE_PUBLIC_CLASS_API__ REAudioPlayer : public REObject, public IREMediaPlayable
{
private:
	IREMediaPlayable * _media;
public:
	virtual REBOOL Play();
	virtual REBOOL IsPlaying() const;
	virtual REBOOL Pause();
	virtual REBOOL Stop();
	virtual REBOOL SetLooped(const REBOOL isLooped);
	virtual REBOOL IsLooped() const; /* by default is not looped */
	virtual REBOOL SetVolume(const REFloat32 newVolume);
	virtual const REFloat32 GetVolume() const;
	
	REBOOL InitWithData(const REData & soundFileData);
	REBOOL InitFromFilePath(const REString & filePath);
	
	REAudioPlayer();
	virtual ~REAudioPlayer();
};



#endif /* __REAUDIOPLAYER_H__ */


