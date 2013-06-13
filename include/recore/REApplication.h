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

/// Class of recore basic application
class __RE_PUBLIC_CLASS_API__ REApplication : public REObject
{
private:
	REAutoReleasePool * _appAutoReleasePool;
	REString _errorDescriptionString;
	
	REBOOL REApplicationInit();
	
protected:
	void AddToErrorDescription(const REString & errorString);
	void ClearErrorDescription();
	REApplication();
	virtual ~REApplication();
public:
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	/// Returns string with application error description string.
	const REString & GetErrorDescriptionString() const;
	
	/// Returns name of application.
	virtual REString GetName() const;
	
	/// Returns float value of application version
	virtual REFloat32 GetVersion() const;
	
	/// Returns application description.
	virtual REString GetDescription() const;
	
	/// Initializes application.
	virtual REBOOL Initialize();
	
	/// Checks is application successfully initialized and ready to work
	virtual REBOOL IsOK() const;
	
	/// Suspends execution of the application
	/// Returns true if application paused and false on error or if application already paused
	virtual REBOOL Pause();
	
	/// Resumes execution of the application
	/// Returns true if application resumed and false on error or if application already resumed
	virtual REBOOL Resume();
	
	/// Checks is application paused
	virtual REBOOL IsPaused() const;
	
	/// Updates application logic. Call this method from thread in witch application was created.
	/// Need for updating application time, main loop objects and default auto release pool.
	void Update();
};


#endif /* __REAPPLICATION_H__ */


