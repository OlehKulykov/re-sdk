/*
 *   Copyright 2012 Kulykov Oleh
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


#ifndef __IRERENDERABLE_H__
#define __IRERENDERABLE_H__

#include "../recore/RECommonHeader.h"

/// Class interface for classes that can be rendered.
class __RE_PUBLIC_CLASS_API__ IRERenderable
{
public:
	/// Called for render. Top left posint is [0.0f, 0.0f]
	virtual void Render() = 0;
	
	/// Called for render using offset point. Top left posint is ['offsetX', 'offsetY']
	virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY) = 0;
	
	virtual ~IRERenderable() { }
};


#endif /* __IRERENDERABLE_H__ */


