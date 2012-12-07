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


#ifndef __RECPUFEATURES_H__
#define __RECPUFEATURES_H__

#include "RECommonHeader.h"

/// Types of CPU features
typedef enum _reCPUFeature
{
	/// Default state. Not initialized
	RECPUFeatureNone = 0,
	
	/// State when features allready initialized
	RECPUFeatureIsInitialized = 1,

	/// CPU supports MMX instructions
	RECPUFeatureMMX = (1 << 1),
	
	/// CPU supports 3DNow instructions
	RECPUFeature3DNow = (1 << 2),
	
	/// CPU supports SSE instructions
	RECPUFeatureSSE = (1 << 3),
	
	/// CPU supports SSE2 instructions
	RECPUFeatureSSE2 = (1 << 4),
	
	/// CPU supports SSE3 instructions
	RECPUFeatureSSE3 = (1 << 5),
	
	/// CPU supports SSE4 1 instructions
	RECPUFeatureSSE4_1 = (1 << 6),
	
	/// CPU supports SSE4 2 instructions
	RECPUFeatureSSE4_2 = (1 << 7),
	
	/// CPU supports ARM NEON instructions
	RECPUFeatureARM_NEON = (1 << 8)
} RECPUFeature;

/// Class used for detecting CPU features
class __RE_PUBLIC_CLASS_API__ RECPUFeatures
{
public:
	/// Checks is CPU has feature
	/// 'flag' is RECPUFeature type
	static REBOOL CheckFlag(const REInt32 flag);
};


#endif /* __RECPUFEATURES_H__ */


