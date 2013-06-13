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


#ifndef __REANIMATIONINFO_H__
#define __REANIMATIONINFO_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REObject.h"
#include "../recore/REObjectsArray.h"
#include "../recore/REClassMethod.h"
#include "IREAnimation.h"



/// Class using for getting information about animation.
/// Animation methods setted using 'SetDidStartMethod(REClassMethod *)' and 'SetDidStopMethod(REClassMethod *)'
/// called with pointer to this animation info object.
class __RE_PUBLIC_CLASS_API__ REAnimationInfo : public REObject
{
protected:
	REClassMethod * _startMethod;
	REClassMethod * _stopMethod;
	void * _customData;
	RETimeInterval _time;
	RETimeInterval _startDelay;
	REUInt32 _loopsCount;
	REUByte _animationType;
	REUByte _loopType;
	REUByte _stopType;
	
	REAnimationInfo();
	virtual ~REAnimationInfo();
public: 
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	/// Returns count of animated objects.
	virtual const REUInt32 GetObjectsCount() const = 0;
	
	/// Copyes animated objects to array without retaining.
	virtual void GetObjects(REArray<REObject *> * views) const = 0;
	
	/// Checks info for containing animated objects.
	virtual REBOOL IsEmpty() const = 0;
	
	/// Returns pointer to method called when animation started.
	REClassMethod * GetStartMethod() const;
	
	/// Returns pointer to method called when animation finished.
	REClassMethod * GetStopMethod() const;
	
	/// Returns pointer to custom data assigned on setuping new animation.
	void * GetCustomData() const;
	
	/// Returns time of animation.
	const RETimeInterval GetTime() const;
	
	/// Returns time of delay before starting animation.
	const RETimeInterval GetStartDelay() const;
	
	/// Returns animation loops count. 'RENotFound' - mean looped without loops count.
	const REUInt32 GetLoopsCount() const;
	
	/// Returns type of animation.
	const REAnimationType GetAnimationType() const;
	
	/// Returns type of animation looping.
	const REAnimationLoopType GetLoopType() const;
	
	/// Return type of animation stoping.
	const REAnimationStopType GetStopType() const;
};


#endif /* __REANIMATIONINFO_H__ */


