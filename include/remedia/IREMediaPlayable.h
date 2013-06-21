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


#ifndef __IREMEDIAPLAYABLE_H__
#define __IREMEDIAPLAYABLE_H__


#include "../recore/RECommonHeader.h"


class __RE_PUBLIC_CLASS_API__ IREMediaPlayable 
{
public:
	virtual REBOOL play() = 0;
	virtual REBOOL isPlaying() const = 0;
	virtual REBOOL pause() = 0;
	virtual REBOOL stop() = 0;
	virtual REBOOL setLooped(const REBOOL isLooped) = 0;
	virtual REBOOL isLooped() const = 0; /* by default is not looped */
	virtual REBOOL setVolume(const REFloat32 newVolume) = 0;
	virtual const REFloat32 getVolume() const = 0;
	
	virtual ~IREMediaPlayable() { }
};

#endif /* __IREMEDIAPLAYABLE_H__ */

