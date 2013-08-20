/*
 *   Copyright 2012 - 2013 Kulykov Oleh
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


#ifndef __RECOLOR_H__
#define __RECOLOR_H__

#include "RECommonHeader.h"
#include "REMem.h"

#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
#include <arm_neon.h>
#endif

#ifdef __RE_USING_DIRECTX9__
#include <D3d9types.h>
#endif /* __RE_USING_DIRECTX9__ */

/// Class using for presenting color using float RGBA values in range [0.0f, 1.0f].
class __RE_PUBLIC_CLASS_API__ REColor
{
public:
	union
	{
		struct
		{
			/// Red color component. Range [0.0f, 1.0f].
			REFloat32 red;
			
			/// Green color component. Range [0.0f, 1.0f].
			REFloat32 green;
			
			/// Blue color component. Range [0.0f, 1.0f].
			REFloat32 blue;
			
			/// Alpha color component. Range [0.0f, 1.0f].
			REFloat32 alpha;
		};
		
		/// Array of RGBA color components. Each component in range [0.0f, 1.0f].
		REFloat32 rgba[4];
		
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
		float32x4_t armNeonColor;
#endif
	};

#ifdef __RE_USING_DIRECTX9__
	/// Returns DirectX color value.
	const D3DCOLOR D3DColor() const 
	{
		const int r = (int)(red * 255);
		const int g = (int)(green * 255);
		const int b = (int)(blue * 255);
		const int a = (int)(alpha * 255);
		return D3DCOLOR_ARGB(a, r, g, b);
	}
#endif /* __RE_USING_DIRECTX9__ */	
	
	/// Setting this color from 'anotherColor' color value.
	REColor & operator=(const REColor & anotherColor)
	{
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
		armNeonColor = anotherColor.armNeonColor;
#else
		memcpy(rgba, anotherColor.rgba, sizeof(REFloat32) * 4); 
#endif		
		return (*this);
	}
	
	/// Setting byte RGB color components. Each component in range [0, 255]. Alpha will be 1.0f.
	void setRGB(const REUByte newRed, const REUByte newGreen, const REUByte newBlue);
	
	/// Setting byte RGBA color components. Each component in range [0, 255].
	void setRGBA(const REUByte newRed, const REUByte newGreen, const REUByte newBlue, const REUByte newAlpha);
	
	/// Setting color from hex string. String not case sensitive. Example: "#FFaaBb". Alpha will be 1.0f.
	void setHEX(const char * hexString);
	
	/// Constructs color from float RGBA components. Each component in range [0.0f, 1.0f].
	REColor(const REFloat32 newRed, const REFloat32 newGreen, const REFloat32 newBlue, const REFloat32 newAlpha);
	
	/// Constructs color from hex string. String not case sensitive. Example: "#FFaaBb". Alpha will be 1.0f.
	REColor(const char * hexString);
	
	/// Constructs color from 'anotherColor' color value.
	REColor(const REColor & anotherColor) 
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
	: armNeonColor(anotherColor.armNeonColor) { }
#else		
	{ 
		memcpy(rgba, anotherColor.rgba, sizeof(REFloat32) * 4); 
	}
#endif	
	
	/// Constructs color with white color value. Each component is 1.0f
	REColor();
	
	/// Construct color object from arm neon vector
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
	REColor(const float32x4_t & c) : armNeonColor(c) { }
#endif
	
	/// Destructor.
	~REColor();
};

#endif /* __RECOLOR_H__ */

