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


#ifndef __REGUIAPPLICATION_H__
#define __REGUIAPPLICATION_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REApplication.h"
#include "../recore/REPoint2.h"
#include "IRERenderable.h"
#include "RERenderDevice.h"
#include "REViewController.h"

#if (defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) || defined(__USING_RECORE_DYNAMIC_LIBRARY__))
/// Warning message 4251: Class 'REArray<T>' needs to have dll-interface to be used by clients of class.
/// Why disable 4251: Class 'REArray<T>' defined as private field and no clents can access it.
/// To access must use public methods.
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif
#endif

class __RE_PUBLIC_CLASS_API__ REGUIApplication : public REApplication, public IRERenderable
{
private:
	RERenderDevice * _renderDevice;
	REViewController * _rootViewController;
	REArray<REView *> _onClickFoundedViews;
	REArray<REView *> _userActionViews;
	REPoint2 _lastUserClickPos;
	REUInt32 _isLastUserClickMoving;

	REBOOL InitGUIApplication(REViewController * rootViewController, void * windowHandle);

protected:
	/// Constructs empty gui application.
	REGUIApplication();
	
	/// Destructor. If root view controller assigned than it's will be Release()'ed.
	virtual ~REGUIApplication();
	
public:
	/// Returns pointer to root view controller.
	REViewController * GetRootViewController() const;
	
	/// Setting new root view controller. 
	/// Previous view controller will be Release()'ed and 'newRootViewController' exists it will be Retain()ed.
	void SetRootViewController(REViewController * newRootViewController);
	
	/// Try to initialize base REApplication and than GUI application.
	virtual REBOOL Initialize();
	
	/// Try to initialize base REApplication and than GUI application with root view controller.
	/// If default RERenderDevice if not initialized it will try to init.
	/// 'rootViewController' will be Retain()'ed.
	virtual REBOOL Initialize(REViewController * rootViewController);
	
	/// Try to initialize base REApplication and than GUI application with root view controller
	/// and window handle.
	/// Window handle using when creating application using DirectX. 
	/// If default RERenderDevice if not initialized it will try to init.
	/// 'rootViewController' will be Retain()'ed.
	virtual REBOOL Initialize(REViewController * rootViewController, void * windowHandle);
	
	/// Checks is application successfully initialized and ready to work.
	virtual REBOOL IsOK() const;
	
	/// Suspends execution of the application
	/// Returns true if application paused and false on error or if application already paused
	virtual REBOOL Pause();
	
	/// Resumes execution of the application
	/// Returns true if application resumed and false on error or if application already resumed
	virtual REBOOL Resume();
	
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	/* IRERenderable */
	virtual void Render();
	virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
#ifdef __RE_OS_DESKTOP__
	/// Call when user mouse click moved on screen without clicking with 'screenX' and 'screenY' coords.
	/// 'screenX' and 'screenY' this is coordinates from top left of window using window coordinate system.
	/// Used when preprocessor directive __RE_OS_DESKTOP__ is defined.
	void OnMouseMoveOnScreen(const REFloat32 screenX, const REFloat32 screenY);
#endif
	
	/// Call when user click(mouse or touch) starts with 'screenX' and 'screenY' coords.
	/// 'screenX' and 'screenY' this is coordinates from top left of window using window coordinate system.
	void OnClickOnScreenStart(const REFloat32 screenX, const REFloat32 screenY);
	
	/// Call when user click(mouse or touch) end with 'screenX' and 'screenY' coords.
	/// 'screenX' and 'screenY' this is coordinates from top left of window using window coordinate system.
	void OnClickOnScreenEnd(const REFloat32 screenX, const REFloat32 screenY);
	
	/// Call when user click(mouse or touch) moving after starts and before end with 'screenX' and 'screenY' coords.
	/// 'screenX' and 'screenY' this is coordinates from top left of window using window coordinate system.
	void OnClickMoveOnScreen(const REFloat32 screenX, const REFloat32 screenY);
	
	/// Creates and returns new gui application object.
	static REGUIApplication * Create();
};


#endif /* __REGUIAPPLICATION_H__ */


