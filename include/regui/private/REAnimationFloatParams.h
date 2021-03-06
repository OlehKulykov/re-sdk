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


#ifndef __REANIMATIOFLOATPARAMS_H__
#define __REANIMATIOFLOATPARAMS_H__

#include "../../recore/RECommonHeader.h"
#include "../../recore/REArray.h"
#include "../../recore/REObject.h"
#include "../REView.h"
#include "../IREAnimationRespondent.h"

class __RE_PUBLIC_CLASS_API__ REAnimationFloatParams : public REObject
{	
public:
	typedef struct _paramStruct
	{
		REFloat32 * paramPtr;
		REFloat32 start;
		REFloat32 end;
		REFloat32 length;
	} ParamStruct; 
	
	REObject * object;
	REUInt16 * objectAnimationsCounter;
	IREAnimationRespondent * respondent;
	REArray<REAnimationFloatParams::ParamStruct> params;
	
	void toStartValues()
	{
		for (REUInt32 i = 0; i < params.count(); i++) 
		{
			(*params[i].paramPtr) = params[i].start;
		}
	}
	
	void toEndValues()
	{
		for (REUInt32 i = 0; i < params.count(); i++) 
		{
			(*params[i].paramPtr) = params[i].end;
		}
	}
	
	void setStopValues(const REAnimationStopType stopType)
	{
		switch (stopType) 
		{
			case REAnimationStopTypeImmediatelyToStart:
				this->toStartValues();
				break;
			case REAnimationStopTypeImmediatelyToEnd:
				this->toEndValues();
				break;	
			default:
				break;
		}
	}
	
	void incCounter()
	{
		if (objectAnimationsCounter)
		{
			(*objectAnimationsCounter) = (*objectAnimationsCounter) + 1;
		}
	}
	
	void decCounter()
	{
		if (objectAnimationsCounter)
		{
			(*objectAnimationsCounter) = (*objectAnimationsCounter) - 1;
		}
	}
	
	REAnimationFloatParams() : REObject(), object(NULL), objectAnimationsCounter(NULL), respondent(NULL) { }
	virtual ~REAnimationFloatParams() { }
};

#endif /* __REANIMATIOFLOATPARAMS_H__ */


