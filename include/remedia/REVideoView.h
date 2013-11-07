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


#ifndef __REVIDEOVIEW_H__
#define __REVIDEOVIEW_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REObject.h"
#include "../recore/REData.h"

#include "../regui/REView.h"

#include "IREMediaPlayable.h"


class __RE_PUBLIC_CLASS_API__ REVideoView : public REView, public IREMediaPlayable
{
private:
	void * _fields;
public:
	/* IREMediaPlayable */
	virtual REBOOL play();
	virtual REBOOL isPlaying() const;
	virtual REBOOL pause();
	virtual REBOOL stop();
	virtual REBOOL setLooped(const REBOOL isLooped);
	virtual REBOOL isLooped() const; /* by default is not looped */
	virtual REBOOL setVolume(const REFloat32 newVolume);
	virtual const REFloat32 getVolume() const;
		
	/* REObject */
	virtual const REUInt32 getClassIdentifier() const;
	static const REUInt32 classIdentifier();
	
	REBOOL initWithData(const REData & soundFileData);
	
	REVideoView();
	virtual ~REVideoView();
};

#endif /* __REVIDEOVIEW_H__ */

