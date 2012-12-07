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


#ifndef __IREMEDIAPLAYABLE_H__
#define __IREMEDIAPLAYABLE_H__


#include "../recore/RECommonHeader.h"


class __RE_PUBLIC_CLASS_API__ IREMediaPlayable 
{
public:
	virtual REBOOL Play() = 0;
	virtual REBOOL IsPlaying() const = 0;
	virtual REBOOL Pause() = 0;
	virtual REBOOL Stop() = 0;
	virtual REBOOL SetLooped(const REBOOL isLooped) = 0;
	virtual REBOOL IsLooped() const = 0; /* by default is not looped */
	virtual REBOOL SetVolume(const REFloat32 newVolume) = 0;
	virtual const REFloat32 GetVolume() const = 0;
	
	virtual ~IREMediaPlayable() { }
};

#endif /* __IREMEDIAPLAYABLE_H__ */

