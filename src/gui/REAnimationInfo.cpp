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


#include "../../include/regui/REAnimationInfo.h"

const REUInt32 REAnimationInfo::GetClassIdentifier() const
{
	return REAnimationInfo::ClassIdentifier();
}

const REUInt32 REAnimationInfo::ClassIdentifier()
{
	static const REUInt32 classIdentif = REObject::GenerateClassIdentifierFromClassName("REAnimationInfo");
	return classIdentif;
}

REBOOL REAnimationInfo::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REAnimationInfo::ClassIdentifier() == classIdentifier) ||
			REObject::IsImplementsClass(classIdentifier));
}

REClassMethod * REAnimationInfo::GetStartMethod() const
{
	return _startMethod;
}

REClassMethod * REAnimationInfo::GetStopMethod() const
{
	return _stopMethod;
}

void * REAnimationInfo::GetCustomData() const
{
	return _customData;
}

const RETimeInterval REAnimationInfo::GetTime() const
{
	return _time;
}

const RETimeInterval REAnimationInfo::GetStartDelay() const
{
	return _startDelay;
}

const REUInt32 REAnimationInfo::GetLoopsCount() const
{
	return _loopsCount;
}

const REAnimationType REAnimationInfo::GetAnimationType() const
{
	return (REAnimationType)_animationType;
}

const REAnimationLoopType REAnimationInfo::GetLoopType() const
{
	return (REAnimationLoopType)_loopType;
}

const REAnimationStopType REAnimationInfo::GetStopType() const
{
	return (REAnimationStopType)_stopType;
}

REAnimationInfo::REAnimationInfo() : REObject(),
	_startMethod(NULL),
	_stopMethod(NULL),
	_customData(NULL),
	_time(0.0),
	_startDelay(0.0),
	_loopsCount(0),
	_animationType(0),
	_loopType(0),
	_stopType(0)
{
	
}

void REAnimationInfo::OnReleased()
{
	RE_SAFE_DELETE(_startMethod);
	RE_SAFE_DELETE(_stopMethod);
	
	REObject::OnReleased();
}

REAnimationInfo::~REAnimationInfo()
{

}


