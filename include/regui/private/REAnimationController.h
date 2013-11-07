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


#ifndef __REANIMATIONCONTROLLER_H__
#define __REANIMATIONCONTROLLER_H__

#include "../../recore/RECommonHeader.h"
#include "../../recore/REArray.h"
#include "../../recore/REList.h"
#include "REEditableAnimationInfo.h"

#if (defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) || defined(__USING_RECORE_DYNAMIC_LIBRARY__))
/// Warning message 4251: Class 'REArray<T>' needs to have dll-interface to be used by clients of class.
/// Why disable 4251: Class 'REArray<T>' defined as private field and no clents can access it.
/// To access must use public methods.
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif
#endif

class __RE_PUBLIC_CLASS_API__ REAnimationController : public REObject
{
private:
	//REList<REEditableAnimationInfo *> _setuping;
	//REList<REEditableAnimationInfo *> _working;
	
	REArray<REEditableAnimationInfo *> _setuping;
	REArray<REEditableAnimationInfo *> _working;
	
protected:
	//REEditableAnimationInfo * _lastInfo;
	REBOOL _isPaused;

	void trySelfRelease();
	static REAnimationController * _defaultController;

	REAnimationController();
	virtual ~REAnimationController();
public:	
	REBOOL stopAllAnimationForView(REObject * view, const REAnimationStopType stopType, REBOOL isNeedCallDelegate);
	REEditableAnimationInfo * lastInfo();
	void animationFinishedSuccessfully(REAnimationInfo * animationInfo);
	REUIdentifier startSetupNewAnimation(void * data);
	REBOOL addFloatParam(REObject * view, 
						 REUInt16 * animationsCounter,
						 REFloat32 * param, 
						 const REFloat32 startValue, 
						 const REFloat32 endValue);
	REBOOL executeLastAnimation();
	void pause();
	void Continue();
	const REFloat32 getProgress(REObject * animatedObject);
	
	static REAnimationController * defaultController();
	static void releaseDefaultController();
};


#endif /* __REANIMATIONCONTROLLER_H__ */


