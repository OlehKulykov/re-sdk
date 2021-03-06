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
#include "REViewController1.h"
#include "RETexture.h"

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

	REBOOL initGUIApplication(REViewController * rootViewController, void * windowHandle);

protected:
	virtual void registerGUIApplicationClassNames();
	
	/// Constructs empty gui application.
	REGUIApplication();
	
	/// Destructor. If root view controller assigned than it's will be Release()'ed.
	virtual ~REGUIApplication();
	
public:
	/// Returns pointer to root view controller.
	REViewController * getRootViewController() const;
	
	/// Setting new root view controller. 
	/// Previous view controller will be Release()'ed and 'newRootViewController' exists it will be Retain()ed.
	void setRootViewController(REViewController * newRootViewController);
	
	/// Try to initialize base REApplication and than GUI application.
	virtual REBOOL initialize();
	
	/// Try to initialize base REApplication and than GUI application with root view controller.
	/// If default RERenderDevice if not initialized it will try to init.
	/// 'rootViewController' will be Retain()'ed.
	virtual REBOOL initialize(REViewController * rootViewController);
	
	/// Try to initialize base REApplication and than GUI application with root view controller
	/// and window handle.
	/// Window handle using when creating application using DirectX. 
	/// If default RERenderDevice if not initialized it will try to init.
	/// 'rootViewController' will be Retain()'ed.
	virtual REBOOL initialize(REViewController * rootViewController, void * windowHandle);
	
	/// Checks is application successfully initialized and ready to work.
	virtual REBOOL isOK() const;
	
	/// Suspends execution of the application
	/// Returns true if application paused and false on error or if application already paused
	virtual REBOOL pause();
	
	/// Resumes execution of the application
	/// Returns true if application resumed and false on error or if application already resumed
	virtual REBOOL resume();
	
	/* REObject */
	virtual void onReleased();
	
	/* IRERenderable */
	virtual void render();
	virtual void renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
#ifdef __RE_OS_DESKTOP__
	/// Call when user mouse click moved on screen without clicking with 'screenX' and 'screenY' coords.
	/// 'screenX' and 'screenY' this is coordinates from top left of window using window coordinate system.
	/// Used when preprocessor directive __RE_OS_DESKTOP__ is defined.
	void OnMouseMoveOnScreen(const REFloat32 screenX, const REFloat32 screenY);
#endif
	
	/// Call when user click(mouse or touch) starts with 'screenX' and 'screenY' coords.
	/// 'screenX' and 'screenY' this is coordinates from top left of window using window coordinate system.
	void onClickOnScreenStart(const REFloat32 screenX, const REFloat32 screenY);
	
	/// Call when user click(mouse or touch) end with 'screenX' and 'screenY' coords.
	/// 'screenX' and 'screenY' this is coordinates from top left of window using window coordinate system.
	void onClickOnScreenEnd(const REFloat32 screenX, const REFloat32 screenY);
	
	/// Call when user click(mouse or touch) moving after starts and before end with 'screenX' and 'screenY' coords.
	/// 'screenX' and 'screenY' this is coordinates from top left of window using window coordinate system.
	void onClickMoveOnScreen(const REFloat32 screenX, const REFloat32 screenY);
		
	/// Creates RESerializable class.
	/// Class name for class trying to get from dictionary using RESerializable::classNameKey() key.
	/// If key founded trying to create object using REApplication createClassWithClassName method.
	/// If you want to create custom class by name, you shoud register callback with registerNewClassForNameCallback method.
	/// Return pointer of the class also dynamicaly casted for RESerializable class, but returned source void pointer.
	void * createSerializableObjectWithDictionary(const RETypedPtr & dictionary);
	
	/// Creates RESerializable class with casting to reqired type.
	/// For detailed description look createSerializableObjectWithDictionary method.
	template <class T> T * createSerializableClassWithDictionary(const RETypedPtr & dictionary)
	{
		void * object = this->createSerializableObjectWithDictionary(dictionary);
		return REPtrCast<T, void>(object);
	}
	
	/// Returns gui application object.
	static REGUIApplication * currentApplication();
};


#endif /* __REGUIAPPLICATION_H__ */


