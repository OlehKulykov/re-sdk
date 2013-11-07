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


#ifndef __IREANIMATIONRESPONDENT_H__
#define __IREANIMATIONRESPONDENT_H__

#include "../recore/RECommonHeader.h"


/// Abstract class interface using for animation respondent objects. 
/// Objects that inherits this class will receive animation events when animating
class __RE_PUBLIC_CLASS_API__ IREAnimationRespondent
{
public:
	virtual void animationDidStarted(const REUIdentifier animationIdentifier) = 0;
	virtual void animationDidFinished(const REUIdentifier animationIdentifier) = 0;
	virtual void animationDidStep(const REFloat32 progress) = 0;
	
	virtual ~IREAnimationRespondent() { }
};


#endif /* __IREANIMATIONRESPONDENT_H__ */


