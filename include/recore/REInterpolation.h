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
	static const REFloat32 linear(const REFloat32 p)
	{
		return p;
	}
	
	/// Returns quad ease in interpolation value of argument.
	static const REFloat32 quadEaseIn(const REFloat32 p)
	{
		return (p * p);
	}
	
	/// Returns quad ease out interpolation value of argument.
	static const REFloat32 quadEaseOut(const REFloat32 p)
	{
		return -(p * (p - 2.0f));
	}
	
	/// Returns quad ease in out interpolation value of argument.
	static const REFloat32 quadEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return (REInterpolation::quadEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f;
		}
		return (REInterpolation::quadEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns quad ease out in interpolation value of argument.
	static const REFloat32 quadEaseOutIn(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::quadEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::quadEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns cubic ease in interpolation value of argument.
	static const REFloat32 cubicEaseIn(const REFloat32 p)
	{
		return (p * p * p);
	}
	
	/// Returns cubic ease out interpolation value of argument.
	static const REFloat32 cubicEaseOut(const REFloat32 p)
	{
		const REFloat32 np = p - 1.0f;
		return ((np * np * np) + 1.0f);
	}
	
	/// Returns cubic ease in out interpolation value of argument.
	static const REFloat32 cubicEaseInOut(const REFloat32 p)
	{			
		if (p > 0.5f)
		{
			return ((REInterpolation::cubicEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::cubicEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns cubic ease out in interpolation value of argument.
	static const REFloat32 cubicEaseOutIn(const REFloat32 p)
	{ 
		if (p > 0.5f)
		{
			return ((REInterpolation::cubicEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::cubicEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns quartic ease in interpolation value of argument.
	static const REFloat32 quarticEaseIn(const REFloat32 p)
	{
		const REFloat32 pp = p * p;
		return (pp * pp);
	}
	
	/// Returns quartic ease out interpolation value of argument.
	static const REFloat32 quarticEaseOut(const REFloat32 p)
	{
		const REFloat32 np = p - 1.0f;
		const REFloat32 npnp = np * np;
		return -((npnp * npnp) - 1.0f);
	}
	
	/// Returns quartic ease in out interpolation value of argument.
	static const REFloat32 quarticEaseInOut(const REFloat32 p)
	{			
		if (p > 0.5f)
		{
			return ((REInterpolation::quarticEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::quarticEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns quartic ease out in interpolation value of argument.
	static const REFloat32 quarticEaseOutIn(const REFloat32 p)
	{ 
		if (p > 0.5f)
		{
			return ((REInterpolation::quarticEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::quarticEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns quantic ease in interpolation value of argument.
	static const REFloat32 quinticEaseIn(const REFloat32 p) 
	{
		const REFloat32 pp = p * p;
		return (pp * pp * p);
	}
	
	/// Returns quantic ease out interpolation value of argument.
	static const REFloat32 quinticEaseOut(const REFloat32 p)
	{
		const REFloat32 np = p - 1.0f;
		const REFloat32 npnp = np * np;
		return ((npnp * npnp * np) + 1.0f);
	}
	
	/// Returns quintic ease in out interpolation value of argument.
	static const REFloat32 quinticEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::quinticEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::quinticEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns quintic ease out in interpolation value of argument.
	static const REFloat32 quinticEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::quinticEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::quinticEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns sinusoidal ease in interpolation value of argument.
	static const REFloat32 sinusoidalEaseIn(const REFloat32 p)
	{
#if defined(HAVE_FUNCTION_COSF)		
		return (1.0f - cosf(MATH_PI_DIV_2 * p));
#else
		return (REFloat32)(1.0 - cos(MATH_PI_DIV_2 * p));
#endif		
	}
	
	/// Returns sinusoidal ease out interpolation value of argument.
	static const REFloat32 sinusoidalEaseOut(const REFloat32 p)
	{
#if defined(HAVE_FUNCTION_SINF)		
		return sinf(MATH_PI_DIV_2 * p);
#else
		return (REFloat32)sin(MATH_PI_DIV_2 * p);
#endif		
	}
	
	/// Returns sinusoidal ease in out interpolation value of argument.
	static const REFloat32 sinusoidalEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::sinusoidalEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::sinusoidalEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns sinusoidal ease out in interpolation value of argument.
	static const REFloat32 sinusoidalEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::sinusoidalEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::sinusoidalEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns exponential ease in interpolation value of argument.
	static const REFloat32 exponentialEaseIn(const REFloat32 p)
	{	 
#if defined(HAVE_FUNCTION_POWF)		
		return (powf(2.0f, 10.0f * (p - 1.0f)) - 0.001f);
#else
		return (REFloat32)(pow(2.0, 10.0 * (p - 1.0)) - 0.001);
#endif		
		
	}
	
	/// Returns exponential ease out interpolation value of argument.
	static const REFloat32 exponentialEaseOut(const REFloat32 p)
	{
#if defined(HAVE_FUNCTION_POWF)		
		return (1.001f * (1.0f - powf(2.0f, -10.0f * p)));
#else
		return (REFloat32)(1.001 * (1.0 - pow(2.0, -10.0 * p)));
#endif		
	}
	
	/// Returns exponential ease in out interpolation value of argument.
	static const REFloat32 exponentialEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::exponentialEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::exponentialEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns exponential ease out in interpolation value of argument.
	static const REFloat32 exponentialEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::exponentialEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::exponentialEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns circular ease in interpolation value of argument.
	static const REFloat32 circularEaseIn(const REFloat32 p)
	{
#if defined(HAVE_FUNCTION_SQRTF)		
		return -(sqrtf(1.0f - (p * p)) - 1.0f);
#else
		return (REFloat32)(-(sqrt(1.0 - (p * p)) - 1.0));
#endif		
	}
	
	/// Returns circular ease out interpolation value of argument.
	static const REFloat32 circularEaseOut(const REFloat32 p)
	{
		const REFloat32 np = p - 1.0f;
#if defined(HAVE_FUNCTION_SQRTF)		
		return sqrtf(1.0f - (np * np));
#else
		return (REFloat32)sqrt(1.0 - (np * np));
#endif		
	}
	
	/// Returns circular ease in out interpolation value of argument.
	static const REFloat32 circularEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::circularEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::circularEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns circular ease out in interpolation value of argument.
	static const REFloat32 circularEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::circularEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::circularEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns elastic ease in interpolation value of argument.
	static const REFloat32 elasticEaseIn(const REFloat32 p)
	{
#if defined(HAVE_FUNCTION_POWF) 
		const REFloat32 p1 = powf(2.0f, 10.0f * (p - 1.0f));
#else
		const REFloat64 p1 = pow(2.0, 10.0 * (p - 1.0));
#endif		
#if defined(HAVE_FUNCTION_SINF)
		const REFloat32 s1 = sinf(((p - 1.0f) - 0.075f) * MATH_2_MUL_PI / 0.3f);
#else
		const REFloat64 s1 = sin(((p - 1.0) - 0.075) * MATH_2_MUL_PI / 0.3);
#endif
		return (-1.0f * p1 * s1);
	}
	
	/// Returns elastic ease out interpolation value of argument.
	static const REFloat32 elasticEaseOut(const REFloat32 p)
	{
#if defined(HAVE_FUNCTION_POWF) 
		const REFloat32 p1 = powf(2.0f, -10.0f * p);
#else		
		const REFloat64 p1 = pow(2.0, -10.0 * p);
#endif		
#if defined(HAVE_FUNCTION_SINF)
		const REFloat32 s1 = sinf((p - 0.075f) * MATH_2_MUL_PI / 0.3f);
#else
		const REFloat64 s1 = sin((p - 0.075) * MATH_2_MUL_PI / 0.3);
#endif		
		return (p1 * s1 + 1.0f);
	}
	
	/// Returns elastic ease in out interpolation value of argument.
	static const REFloat32 elasticEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::elasticEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::elasticEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns elastic ease out in interpolation value of argument.
	static const REFloat32 elasticEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::elasticEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::elasticEaseOut(p * 2.0f) / 2.0f);
	}
	
	/// Returns back ease in interpolation value of argument.
	static const REFloat32 backEaseIn(const REFloat32 p)
	{
		return (p * p * ((2.7016f * p) - 1.7016f));
	}
	
	/// Returns back ease out interpolation value of argument.
	static const REFloat32 backEaseOut(const REFloat32 p)
	{
		const REFloat32 np = p - 1.0f;
		return (np * np) * ((2.7016f * (p - 1.0f)) + 1.7016f) + 1.0f;
	}
	
	
	/// Returns back ease in out interpolation value of argument.
	static const REFloat32 backEaseInOut(const REFloat32 p)
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::backEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::backEaseIn(p * 2.0f) / 2.0f);
	}
	
	/// Returns back ease out in interpolation value of argument.
	static const REFloat32 backEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::backEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::backEaseOut(p * 2.0f) / 2.0f);
	} 
	
	/// Returns bounce ease in interpolation value of argument.
	static const REFloat32 bounceEaseIn(const REFloat32 p)
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
	static const REFloat32 bounceEaseOut(const REFloat32 p)
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
	static const REFloat32 bounceEaseInOut(const REFloat32 p)
	{
		if (p > 0.5)
		{
			return ((REInterpolation::bounceEaseOut((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::bounceEaseIn(p * 2.0f) / 2.0f);
	}
	 
	/// Returns bounce ease out in interpolation value of argument.
	static const REFloat32 bounceEaseOutIn(const REFloat32 p) 
	{
		if (p > 0.5f)
		{
			return ((REInterpolation::bounceEaseIn((p - 0.5f) * 2.0f) / 2.0f) + 0.5f);
		}
		return (REInterpolation::bounceEaseOut(p * 2.0f) / 2.0f);
	}   
	
	
};

#endif /* __REINTERPOLATION_H__ */

