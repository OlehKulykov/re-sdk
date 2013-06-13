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


#ifndef __REAUDIOPLAYERPRIVATE_IPHONE_H__
#define __REAUDIOPLAYERPRIVATE_IPHONE_H__

#include "../../recore/RECommonHeader.h"

#ifdef __RE_OS_IPHONE__

#include "../../recore/REObject.h"
#include "../../recore/REData.h"
#include "../IREMediaPlayable.h"


class __RE_PUBLIC_CLASS_API__ REAudioPlayerPrivate_iphone : public REObject, public IREMediaPlayable
{
private:
	void * _p;
	REData _soundFileData;
	
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
	
	REAudioPlayerPrivate_iphone();
	virtual ~REAudioPlayerPrivate_iphone();
};

#endif /* __RE_OS_IPHONE__ */


#endif /* __REAUDIOPLAYERPRIVATE_IPHONE_H__ */


