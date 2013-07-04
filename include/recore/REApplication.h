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


#ifndef __REAPPLICATION_H__
#define __REAPPLICATION_H__

#include "RECommonHeader.h"
#include "REAutoReleasePool.h"
#include "RETime.h"
#include "REObject.h"
#include "REString.h"
#include "REMutableString.h"

/// Class of recore basic application
class __RE_PUBLIC_CLASS_API__ REApplication : public REObject
{
private:
	REAutoReleasePool * _appAutoReleasePool;
	REMutableString _errorDescriptionString;
	
	REBOOL reapplicationInit();
	
protected:
	void addToErrorDescription(const REString & errorString);
	void clearErrorDescription();
	REApplication();
	virtual ~REApplication();
public:
	/* REObject */
	virtual const REUInt32 getClassIdentifier() const;
	static const REUInt32 classIdentifier();
	virtual REBOOL isImplementsClass(const REUInt32 classIdentifier) const;
	virtual void onReleased();
	
	/// Returns string with application error description string.
	const REString & getErrorDescriptionString() const;
	
	/// Returns name of application.
	virtual REString getName() const;
	
	/// Returns float value of application version
	virtual REFloat32 getVersion() const;
	
	/// Returns application description.
	virtual REString getDescription() const;
	
	/// Initializes application.
	virtual REBOOL initialize();
	
	/// Checks is application successfully initialized and ready to work
	virtual REBOOL isOK() const;
	
	/// Suspends execution of the application
	/// Returns true if application paused and false on error or if application already paused
	virtual REBOOL pause();
	
	/// Resumes execution of the application
	/// Returns true if application resumed and false on error or if application already resumed
	virtual REBOOL resume();
	
	/// Checks is application paused
	virtual REBOOL isPaused() const;
	
	/// Updates application logic. Call this method from thread in witch application was created.
	/// Need for updating application time, main loop objects and default auto release pool.
	void update();
};


#endif /* __REAPPLICATION_H__ */


