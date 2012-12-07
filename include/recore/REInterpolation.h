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


#ifndef __REINTERPOLATION_H__
#define __REINTERPOLATION_H__

#include "RECommonHeader.h"
#include "REMath.h"

/// Class used for constructing new value within of a discrete of in point.
/// Param is 'p' with range [0, 1].
/// Result is interpolated value.
/// http://en.wikipedia.org/wiki/B%C3%A9zier%5Fcurve
class __RE_PUBLIC_CLASS_API__ REInterpolation
{
public:
	/// Returns linear interpolation value of argument.
	static const REFloat32 Linear(const REFloat32 p)
	{
		return p;
	}
	
	/// Returns quad ease in interpolation value of argument.
	static const REFloat32 QuadEaseIn(const REFloat32 p)
	{
		return (p * p);
	}
	
	/// Returns quad ease out interpolation value of argument.
	static const REFloat32 QuadEaseOut(const REFloat32 p)
	{
		return -(p * (p - 2.0f));
	}
	
	/// Returns quad ease in out interpolation value of argument.
	static const REFloat32 QuadEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return (REInterpolation::QuadEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f;
		}
		return (REInterpolation::QuadEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns quad ease out in interpolation value of argument.
	static const REFloat32 QuadEaseOutIn(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::QuadEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::QuadEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns cubic ease in interpolation value of argument.
	static const REFloat32 CubicEaseIn(const REFloat32 p)
	{
		return (p * p * p);
	}
	
	/// Returns cubic ease out interpolation value of argument.
	static const REFloat32 CubicEaseOut(const REFloat32 p)
	{
		const REFloat32 np = p - 1.0f;
		return ((np * np * np) + 1.0f);
	}
	
	/// Returns cubic ease in out interpolation value of argument.
	static const REFloat32 CubicEaseInOut(const REFloat32 p)
	{			
		if (p > 0.5f)
		{
			return ((REInterpolation::CubicEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::CubicEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns cubic ease out in interpolation value of argument.
	static const REFloat32 CubicEaseOutIn(const REFloat32 p)
	{ 
		if (p > 0.5f)
		{
			return ((REInterpolation::CubicEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::CubicEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns quartic ease in interpolation value of argument.
	static const REFloat32 QuarticEaseIn(const REFloat32 p)
	{
		return (p * p * p * p);
	}
	
	/// Returns quartic ease out interpolation value of argument.
	static const REFloat32 QuarticEaseOut(const REFloat32 p)
	{
		const REFloat32 np = p - 1.0f;
		return -((np * np * np * np) - 1.0f);
	}
	
	/// Returns quartic ease in out interpolation value of argument.
	static const REFloat32 QuarticEaseInOut(const REFloat32 p)
	{			
		if (p > 0.5f)
		{
			return ((REInterpolation::QuarticEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::QuarticEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns quartic ease out in interpolation value of argument.
	static const REFloat32 QuarticEaseOutIn(const REFloat32 p)
	{ 
		if (p > 0.5f)
		{
			return ((REInterpolation::QuarticEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::QuarticEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns quantic ease in interpolation value of argument.
	static const REFloat32 QuinticEaseIn(const REFloat32 p) 
	{
		return (p * p * p * p * p);
	}
	
	/// Returns quantic ease out interpolation value of argument.
	static const REFloat32 QuinticEaseOut(const REFloat32 p)
	{
		const REFloat32 np = p - 1.0f;
		return ((np * np * np * np * np) + 1.0f);
	}
	
	/// Returns quintic ease in out interpolation value of argument.
	static const REFloat32 QuinticEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::QuinticEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::QuinticEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns quintic ease out in interpolation value of argument.
	static const REFloat32 QuinticEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::QuinticEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::QuinticEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns sinusoidal ease in interpolation value of argument.
	static const REFloat32 SinusoidalEaseIn(const REFloat32 p)
	{
		return (REFloat32)(1.0 - cos(MATH_PI_DIV_2 * p));
	}
	
	/// Returns sinusoidal ease out interpolation value of argument.
	static const REFloat32 SinusoidalEaseOut(const REFloat32 p)
	{
		return (REFloat32)sin(MATH_PI_DIV_2 * p);
	}
	
	/// Returns sinusoidal ease in out interpolation value of argument.
	static const REFloat32 SinusoidalEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::SinusoidalEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::SinusoidalEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns sinusoidal ease out in interpolation value of argument.
	static const REFloat32 SinusoidalEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::SinusoidalEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::SinusoidalEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns exponential ease in interpolation value of argument.
	static const REFloat32 ExponentialEaseIn(const REFloat32 p)
	{	 
		return (powf(2.0f, 10.0f * (p - 1.0f)) - 0.001f);
	}
	
	/// Returns exponential ease out interpolation value of argument.
	static const REFloat32 ExponentialEaseOut(const REFloat32 p)
	{
		return (1.001f * (1.0f - powf(2.0f, -10.0f * p)));
	}
	
	/// Returns exponential ease in out interpolation value of argument.
	static const REFloat32 ExponentialEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::ExponentialEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::ExponentialEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns exponential ease out in interpolation value of argument.
	static const REFloat32 ExponentialEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::ExponentialEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::ExponentialEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns circular ease in interpolation value of argument.
	static const REFloat32 CircularEaseIn(const REFloat32 p)
	{
		return -(sqrtf(1.0f - (p * p)) - 1.0f);
	}
	
	/// Returns circular ease out interpolation value of argument.
	static const REFloat32 CircularEaseOut(const REFloat32 p)
	{
		const REFloat32 np = p - 1.0f;
		return sqrtf(1.0f - (np * np));
	}
	
	/// Returns circular ease in out interpolation value of argument.
	static const REFloat32 CircularEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::CircularEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::CircularEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns circular ease out in interpolation value of argument.
	static const REFloat32 CircularEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::CircularEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::CircularEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns elastic ease in interpolation value of argument.
	static const REFloat32 ElasticEaseIn(const REFloat32 p)
	{
		return (-1.0f * powf(2.0f, 10.0f * (p - 1.0f)) * sinf(((p - 1.0f) - 0.075f) * MATH_2_MUL_PI / 0.3f));
	}
	
	/// Returns elastic ease out interpolation value of argument.
	static const REFloat32 ElasticEaseOut(const REFloat32 p)
	{
		return powf(2.0f, -10.0f * p) * sinf((p - 0.075f) * MATH_2_MUL_PI / 0.3f) + 1.0f;
	}
	
	/// Returns elastic ease in out interpolation value of argument.
	static const REFloat32 ElasticEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::ElasticEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::ElasticEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns elastic ease out in interpolation value of argument.
	static const REFloat32 ElasticEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::ElasticEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::ElasticEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns back ease in interpolation value of argument.
	static const REFloat32 BackEaseIn(const REFloat32 p)
	{
		return (p * p * ((2.7016f * p) - 1.7016f));
	}
	
	/// Returns back ease out interpolation value of argument.
	static const REFloat32 BackEaseOut(const REFloat32 p)
	{
		const REFloat32 np = p - 1.0f;
		return (np * np) * ((2.7016f * (p - 1.0f)) + 1.7016f) + 1.0f;
	}
	
	
	/// Returns back ease in out interpolation value of argument.
	static const REFloat32 BackEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::BackEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::BackEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns back ease out in interpolation value of argument.
	static const REFloat32 BackEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::BackEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::BackEaseOut(p * 2.0f) / 2.0f);
	} 
	
	/// Returns bounce ease in interpolation value of argument.
	static const REFloat32 BounceEaseIn(const REFloat32 p)
	{
		REFloat32 np = 1.0f - p;
		if (np < (1.0f / 2.75f))
		{
			np = 7.5625f * np * np;
		}
		else if (np < (2.0f / 2.75f))
		{
			np = np - (1.5f / 2.75f);
			np = 7.5625f * np * np + 0.75f;
		}
		else if (np < (2.5f / 2.75f))
		{
			np = np - (2.25f / 2.75f);
			np = 7.5625f * np * np + 0.9375f;
		}
		else
		{
			np = np - (2.625f / 2.75f);
			np = 7.5625f * np * np + 0.984375f;
		}
		return (1.0f - np);
	}
	
	/// Returns bounce ease out interpolation value of argument.
	static const REFloat32 BounceEaseOut(const REFloat32 p)
	{
		REFloat32 np = p;
		if (np < 1.0f / 2.75f)
		{
			np = 7.5625f * np * np;
		}
		else if (np < 2.0f / 2.75f)
		{
			np = np - (1.5f / 2.75f);
			np = 7.5625f * np * np + 0.75f;
		}
		else if (np < 2.5f / 2.75f)
		{
			np = np - (2.25f / 2.75f);
			np = 7.5625f * np * np + 0.9375f;
		}   
		else 
		{ 
			np = np - (2.625f / 2.75f);
			np = 7.5625f * np * np + 0.984375f;
		}
		return np;
	}
	 
	/// Returns bounce ease in out interpolation value of argument.
	static const REFloat32 BounceEaseInOut(const REFloat32 p)
	{
		if (p > 0.5)
		{
			return ((REInterpolation::BounceEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::BounceEaseIn(p * 2.0f) / 2.0f);
	}
	 
	/// Returns bounce ease out in interpolation value of argument.
	static const REFloat32 BounceEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::BounceEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::BounceEaseOut(p * 2.0f) / 2.0f);
	}   
	
	
};

#endif /* __REINTERPOLATION_H__ */

