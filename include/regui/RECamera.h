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


#ifndef __RECAMERA_H__
#define __RECAMERA_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REObject.h"
#include "../recore/REVector3.h"
#include "../recore/REMatrix4.h"
#include "../recore/REQuaternion.h"


class __RE_PUBLIC_CLASS_API__ RECamera : public REObject
{
protected:
	
public:
	REMatrix4 projection;
	REMatrix4 modelView;
	
	/// Apply camera projection and modelView matrices to environment.
	virtual void apply();
	
	/// Returns from environment projection matrix.
	virtual REMatrix4 getAppliedProjectionMatrix();
	
	/// Returns from environment modelView matrix.
	virtual REMatrix4 getAppliedModelViewMatrix();
	
	
	RECamera();
	virtual ~RECamera();
};


#endif /* __RECAMERA_H__ */


