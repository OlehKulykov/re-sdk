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


#ifndef __REPOINT3_H__
#define __REPOINT3_H__

#include "RECommonHeader.h"


class __RE_PUBLIC_CLASS_API__ REPoint3
{
public:
	union
	{
		struct 
		{
			REFloat32 x;
			REFloat32 y;
			REFloat32 z;
		};
		
		REFloat32 coords[3];
	};
	
	REPoint3 & operator=(const REPoint3 & anotherPoint)
	{
		x = anotherPoint.x;
		y = anotherPoint.y;
		z = anotherPoint.z;
		return (*this);
	}
	
	REPoint3(const REFloat32 newX, const REFloat32 newY, const REFloat32 newZ) : x(newX), y(newY), z(newZ) { }
	REPoint3(const REPoint3 & anotherPoint) : x(anotherPoint.x), y(anotherPoint.y), z(anotherPoint.z) { }
	REPoint3() : x(0.0f), y(0.0f), z(0.0f) { }
	~REPoint3() { }

};

#endif /* __REPOINT3_H__ */

