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


#ifndef __REEDITABLEANIMATIONINFO_H__
#define __REEDITABLEANIMATIONINFO_H__

#include "../../recore/RECommonHeader.h"
#include "../../recore/REArray.h"
#include "../REAnimationInfo.h"
#include "../REView.h"
#include "REAnimationFloatParams.h"
#include "RETypedAnimation.h"

#if (defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) || defined(__USING_RECORE_DYNAMIC_LIBRARY__))
/// Warning message 4251: Class 'REArray<T>' needs to have dll-interface to be used by clients of class.
/// Why disable 4251: Class 'REArray<T>' defined as private field and no clents can access it.
/// To access must use public methods.
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif
#endif

class __RE_PUBLIC_CLASS_API__ REEditableAnimationInfo : public REAnimationInfo
{
private:
	REArray<REAnimationFloatParams *> _viewParams;
	REAnimationBase * _animation;
	
	static REAnimationBase * createAnimationForTypeWithInfoAndParams(const REAnimationType type,
																	 REEditableAnimationInfo * info,
																	 REArray<REAnimationFloatParams *> * params);
	static REAnimationFloatParams * findParamForObjectInParams(REObject * view, REArray<REAnimationFloatParams *> * params);
	static REFloat32 getProgressForObjectAnimationInParams(REObject * animatedObject,
														   REAnimationBase * animation,
														   REArray<REAnimationFloatParams *> * params);
	static REBOOL stopAllAnimationForObjectWithStopType(REObject * animatedObject,
														const REAnimationStopType stopType,
														REArray<REAnimationFloatParams *> * params);
protected:
	REEditableAnimationInfo(void * customDataOrNull);	
	virtual ~REEditableAnimationInfo();
public:
	const REFloat32 getProgress(REObject * animatedObject);
	REBOOL stopAllAnimationForView(REObject * view, const REAnimationStopType stopType);
	void pause();
	void Continue();
	REBOOL executeAnimation();
	REBOOL addFloatParam(REObject * view, 
						 REUInt16 * animationsCounter,
						 REFloat32 * param, 
						 const REFloat32 startValue, 
						 const REFloat32 endValue);	
	virtual const REUInt32 getObjectsCount() const;
	virtual void getObjects(REArray<REObject *> * views) const;
	virtual REBOOL isEmpty() const;
	void decrementCounterAndReleaseAllViews();
	void setStartMethod(REClassMethod * m);
	void setStopMethod(REClassMethod * m);
	void setTime(const RETimeInterval t) { _time = t; }
	void setStartDelay(const RETimeInterval t) { _startDelay = t; }
	void setLoopsCount(const REUInt32 v) { _loopsCount = v; }
	void setAnimationType(const REAnimationType v) { _animationType = (REUByte)v; }
	void setLoopType(const REAnimationLoopType v) { _loopType = (REUByte)v; }
	void setStopType(const REAnimationStopType v) { _stopType = (REUByte)v; }
	
	/* REObject */
	virtual void onReleased();
	
	static REEditableAnimationInfo * createWithCustomData(void * customDataOrNull);	
};


#endif /* __REEDITABLEANIMATIONINFO_H__ */


