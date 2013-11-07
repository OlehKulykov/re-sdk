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


#include "../../include/recore/RERandomizer.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

#ifndef RAND_MAX
#define	RAND_MAX 0x7fffffff
#endif

/* 0 to 32767 */
REInt32 RERandomizer::intValue() const
{
	return rand();
}

/* lowRange to upRange */
REInt32 RERandomizer::intValueInRange(const REInt32 lowRange, const REInt32 upRange) const
{
	return ((rand() % upRange) + lowRange);
}

/* 0.0f to 0.9(9)f */
REFloat32 RERandomizer::floatValue() const
{
	return (REFloat32)((REFloat64)rand() / (REFloat64)(RAND_MAX + 1.0));
}

/* lowRange to upRange */	
REFloat32 RERandomizer::floatValueInRange(const REFloat32 lowRange, const REFloat32 upRange) const
{
	return (REFloat32)((((REFloat64)rand() / (REFloat64)(RAND_MAX + 1.0)) * (upRange - lowRange)) + lowRange);
}

RERandomizer::RERandomizer()
{
	static REBOOL isNeedInitialize = true;
	if (isNeedInitialize) 
	{
		/* initialize random seed: */
		srand ( (unsigned int)time(NULL) );
		isNeedInitialize = false;
	}
}

RERandomizer::~RERandomizer()
{

}

