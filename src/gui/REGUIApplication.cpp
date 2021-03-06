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


#include "../../include/regui/REGUIApplication.h"
#include "../../include/regui/RECamera.h"
#include "../../include/regui/REView.h"
#include "../../include/regui/REButton.h"
#include "../../include/regui/RETransformedView.h"
#include "../../include/regui/RETexture.h"
#include "../../include/regui/RETextField.h"
#include "../../include/regui/REParticleView.h"
#include "../../include/regui/RELabel.h"
#include "../../include/regui/REFramedTextureObject.h"
#include "../../include/recore/RELog.h"

#define __DEBUG_MESSAGES_FOR_CREATE_SERIALIZABLE__

class REGUIApplicationInternal 
{	
public:
	static REGUIApplication * currentApplication;
	static void* creteREView();
	static void* creteREButton();
	static void* creteRETransformedView();
	static void* creteRETexture();
	static void* creteRETextField();
	static void* creteREParticleView();
	static void* creteRELabel();
//	static void* creteREFramedTexture();
};

REGUIApplication * REGUIApplicationInternal::currentApplication = NULL;
void* REGUIApplicationInternal::creteREView() { REView * v = REView::create(); return REPtrCast<void, REView>(v); }
void* REGUIApplicationInternal::creteREButton() { REButton * b = REButton::create(); return REPtrCast<void, REButton>(b); }
void* REGUIApplicationInternal::creteRETransformedView() { RETransformedView * v = RETransformedView::create(); return REPtrCast<void, RETransformedView>(v); }
void* REGUIApplicationInternal::creteRETexture() { RETexture * t = RETexture::create(); return REPtrCast<void, RETexture>(t); }
void* REGUIApplicationInternal::creteRETextField() { RETextField * tf = RETextField::create(); return REPtrCast<void, RETextField>(tf); }
void* REGUIApplicationInternal::creteREParticleView() { REParticleView * p = REParticleView::create(); return REPtrCast<void, REParticleView>(p); }
void* REGUIApplicationInternal::creteRELabel() { RELabel * l = RELabel::create(); return REPtrCast<void, RELabel>(l); }
//void* REGUIApplicationInternal::creteREFramedTexture() { REFramedTextureObject * t = REFramedTextureObject::Create(); return REPtrCast<void, REFramedTextureObject>(t); }


class REGUIApplicationPrivate
{
private:
	static REUInt32 FindOnClickViewsPrivate(REArray<REView *> * storeArray, 
											REView * view, 
											const REFloat32 screenX, 
											const REFloat32 screenY);
public:	
	static void FindOnClickViews(REArray<REView *> * onClickViews,
								 REArray<REView *> * userActionViews, 
								 REView * view, 
								 const REFloat32 screenX, 
								 const REFloat32 screenY);
};

REUInt32 REGUIApplicationPrivate::FindOnClickViewsPrivate(REArray<REView *> * storeArray, 
														  REView * view, 
														  const REFloat32 screenX, 
														  const REFloat32 screenY)
{
	if (view->isVisible() && (view->isRespondsForUserAction() || (!view->isInterceptsUserAction()))) 
	{
		if (view->screenFrame().isPointInRect(screenX, screenY)) 
		{
			storeArray->add(view);
			REList<REView *>::Iterator iter(view->subviewsIterator());
			while (iter.next()) 
			{
				if (REGUIApplicationPrivate::FindOnClickViewsPrivate(storeArray, iter.value(), screenX, screenY))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

void REGUIApplicationPrivate::FindOnClickViews(REArray<REView *> * onClickViews,
											   REArray<REView *> * userActionViews, 
											   REView * view, 
											   const REFloat32 screenX, 
											   const REFloat32 screenY)
{
	onClickViews->clear();
	userActionViews->clear();
	REGUIApplicationPrivate::FindOnClickViewsPrivate(onClickViews, view, screenX, screenY);
	REUInt32 i = onClickViews->count();
	while (i) 
	{
		i--;
		REView * v = (*onClickViews)[i];
		userActionViews->add(v);
		if (v->isInterceptsUserAction())
		{
			return;
		}
	}
}

void * REGUIApplication::createSerializableObjectWithDictionary(const RETypedPtr & dictionary)
{
	REDictionary * dict = dictionary.dictionary();
	if (dict) 
	{
		REString * className = dict->valueForKey(RESerializable::classNameKey()).string();
		if (className) 
		{
#if defined(__RE_DEBUG_MODE__) && defined(__DEBUG_MESSAGES_FOR_CREATE_SERIALIZABLE__) 
			RELog::log("GUI create '%s'", className->UTF8String());
#endif			
			void * object = this->createObjectWithClassName(className->UTF8String());
			if (object) 
			{
				RESerializable * serializableFromVoid = REPtrCast<RESerializable, void>(object);
				if (serializableFromVoid) 
				{
					RESerializable * serializable = dynamic_cast<RESerializable *>(serializableFromVoid);
					if (serializable)
					{
#if defined(__RE_DEBUG_MODE__) && defined(__DEBUG_MESSAGES_FOR_CREATE_SERIALIZABLE__) 
						const unsigned long long ___v0 = (unsigned long long)object;
						const unsigned long long ___v1 = (unsigned long long)serializableFromVoid;
						const unsigned long long ___v2 = (unsigned long long)serializable;
						const bool ___allOK = ((___v0 == ___v1) && (___v0 == ___v2));
						if (!___allOK)
						{
							RELog::log("GUI create '%s' void[%llu], ptrCast[%llu], dynCast[%llu]", className->UTF8String(), ___v0, ___v1, ___v2);
						}
#endif			
						return object;
					}
				}
			}
		}
	}
	return NULL;
}

void REGUIApplication::render()
{
	if (_renderDevice && _rootViewController) 
	{
		_renderDevice->startRendering();
		_renderDevice->enter2DMode();
		
		_rootViewController->render(); 
		
		_renderDevice->leave2DMode();
		_renderDevice->endRendering();
	}
}

void REGUIApplication::renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	if (_renderDevice && _rootViewController) 
	{
		_renderDevice->startRendering();
		_renderDevice->enter2DMode();
		
		_rootViewController->renderWithOffset(offsetX, offsetY); 
		
		_renderDevice->leave2DMode();
		_renderDevice->endRendering();
	}
}

#ifdef __RE_OS_DESKTOP__
void REGUIApplication::OnMouseMoveOnScreen(const REFloat32 screenX, const REFloat32 screenY)
{
	RESize ratio( _renderDevice->GetScreenToRenderSizeRatio() );
	const REFloat32 x = ratio.width * screenX;
	const REFloat32 y = ratio.height * screenY;
	
    /*
	this->FindSubViewForCoords(_rootViewController, x, y);
	for (REUInt32 i = 0; i < _userActionViews.Count(); i++) 
	{
		_userActionViews[i]->userCursorMoved(x, y);
	}
    */
	/*
	 if ( _renderDevice && _rootViewController ) 
	 {
	 if (_rootViewController->IsHasSubViews()) 
	 {
	 RESize ratio( _renderDevice->GetScreenToRenderSizeRatio() );
	 
	 const REFloat32 x = ratio.width * screenX;
	 const REFloat32 y = ratio.height * screenY;
	 
	 REView * v = this->FindSubViewLastViewForCoords(_rootViewController, x, y);
	 if (v)
	 {
	 v->userCursorMoved(x, y);
	 }
	 }
	 }
	 */
	
	/*
	 if (_isOK) 
	 {
	 RESize ratio(_defaultRenderDevice->GetScreenToRenderSizeRatio());
	 
	 const REFloat32 x = ratio.width * screenX;
	 const REFloat32 y = ratio.height * screenY;
	 
	 GameController * contr = (GameController*)_appController;
	 contr->UserActionMouseMove(x, y);
	 }
	 */
}
#endif

/// Searching screen top view from 'view'.
/// Method don't check 'view' for NULL.
/// Method must called when all pointer exists and 'view'->IsHasSubViews();
/*
 REView * REGUIApplication::FindSubViewForCoords(REView * view, const REFloat32 screenX, const REFloat32 screenY)
 {
 REView * returnedView = NULL;
 if (view->IsVisible() && view->IsRespondsForUserAction()) 
 {
 if (view->GetScreenFrame().IsPointInRect(screenX, screenY)) 
 {
 returnedView = view;
 const REArrayObject * sv = view->GetSubViewsArray();
 if (sv) 
 {
 REArrayObject * subViews = const_cast<REArrayObject *>(sv);
 for (REUInt32 i = 0; i < subViews->Count(); i++) 
 {
 REView * subView = (REView *)(*subViews)[i];
 REView * findedView = this->FindSubViewForCoords(subView, screenX, screenY);
 if (findedView) { returnedView = findedView; }
 }
 }
 }
 }
 return returnedView;
 }
 */

void REGUIApplication::onClickOnScreenStart(const REFloat32 screenX, const REFloat32 screenY)
{
	if (_renderDevice && _rootViewController) 
	{
		RESize ratio( _renderDevice->screenToRenderSizeRatio() );
		_lastUserClickPos.x = ratio.width * screenX;
		_lastUserClickPos.y = ratio.height * screenY;
		_isLastUserClickMoving = 0;
		REGUIApplicationPrivate::FindOnClickViews(&_onClickFoundedViews, 
												  &_userActionViews, 
												  _rootViewController, 
												  _lastUserClickPos.x, 
												  _lastUserClickPos.y);
		for (REUInt32 i = 0; i < _userActionViews.count(); i++) 
		{
			_userActionViews[i]->userActionClickDidStart(_lastUserClickPos.x, _lastUserClickPos.y);
		}
	}
}

void REGUIApplication::onClickOnScreenEnd(const REFloat32 screenX, const REFloat32 screenY)
{
	if (_userActionViews.count() && _renderDevice) 
	{
		RESize ratio(_renderDevice->screenToRenderSizeRatio());
		
		const REFloat32 x = ratio.width * screenX;
		const REFloat32 y = ratio.height * screenY;
		
		for (REUInt32 i = 0; i < _userActionViews.count(); i++) 
		{
			_userActionViews[i]->userActionClickDidEnd(_lastUserClickPos.x, _lastUserClickPos.y, x, y);
		}
		
		if (_isLastUserClickMoving)
		{
			_isLastUserClickMoving = 0;
			
			if ((fabs(_lastUserClickPos.x - x) > 0.0) || (fabs(_lastUserClickPos.y - y) > 0.0))
			{
				for (REUInt32 i = 0; i < _userActionViews.count(); i++) 
				{
					_userActionViews[i]->userActionClickMovingDidEnd(_lastUserClickPos.x, _lastUserClickPos.y, x, y);
				}
			}
		}
	}
}

void REGUIApplication::onClickMoveOnScreen(const REFloat32 screenX, const REFloat32 screenY)
{
	if (_userActionViews.count() && _renderDevice) 
	{
		RESize ratio(_renderDevice->screenToRenderSizeRatio());
		
		const REFloat32 x = ratio.width * screenX;
		const REFloat32 y = ratio.height * screenY;
		
		if ( (_lastUserClickPos.x != x) || (_lastUserClickPos.y != y) )
		{
			_isLastUserClickMoving = 1;
			for (REUInt32 i = 0; i < _userActionViews.count(); i++)
			{
				_userActionViews[i]->userActionClickMoving(_lastUserClickPos.x, _lastUserClickPos.y, x, y);
			}
		}
		
	}
}

REBOOL REGUIApplication::initGUIApplication(REViewController * rootViewController, void * windowHandle)
{
	if (rootViewController) 
	{
		this->setRootViewController(rootViewController);
	}
	
	REBOOL isOk = false;
	RERenderDevice * renderDevice = RERenderDevice::defaultDevice();
	if (renderDevice) 
	{
		if (renderDevice->isInitialized()) 
		{
			isOk = true; 
		}
		else if (windowHandle)
		{
			if (renderDevice->initializeWithWindow(windowHandle)) 
			{
				isOk = true; 
			}
			else if (renderDevice->initialize()) 
			{
				isOk = true;
			} 
		}
		else
		{
			if (renderDevice->initialize()) 
			{
				isOk = true; 
			} 
			else if (renderDevice->initializeWithWindow(NULL))
			{
				isOk = true; 
			}
		}
	}
	
	if (isOk) 
	{
		_renderDevice = renderDevice;
		return true;
	}
	
	return false;
}

REViewController * REGUIApplication::getRootViewController() const
{
	return _rootViewController;
}

/// Setting new root view controller. 
/// Previous view controller will be Release()'ed and 'newRootViewController' exists it will be Retain()ed.
void REGUIApplication::setRootViewController(REViewController * newRootViewController)
{
	RE_SAFE_RETAIN(_rootViewController, newRootViewController);
}

/// Try to initialize base REApplication and than GUI application.
REBOOL REGUIApplication::initialize()
{
	return this->initGUIApplication(NULL, NULL);
}

/// Try to initialize base REApplication and than GUI application with root view controller.
/// If default RERenderDevice if not initialized it will try to init.
/// 'rootViewController' will be Retain()'ed.
REBOOL REGUIApplication::initialize(REViewController * rootViewController)
{
	return this->initGUIApplication(rootViewController, NULL);
}

/// Try to initialize base REApplication and than GUI application with root view controller
/// and window handle.
/// Window handle using when creating application using DirectX. 
/// If default RERenderDevice if not initialized it will try to init.
/// 'rootViewController' will be Retain()'ed.
REBOOL REGUIApplication::initialize(REViewController * rootViewController, void * windowHandle)
{
	return this->initGUIApplication(rootViewController, windowHandle);
}

/// Checks is application successfully initialized and ready to work.
REBOOL REGUIApplication::isOK() const
{
	if (REApplication::isOK()) 
	{
		if (_renderDevice && _rootViewController) 
		{
			return true;
		}
	}
	return false;
}

/// Suspends execution of the application
REBOOL REGUIApplication::pause()
{
	return REApplication::pause();
}

/// Resumes execution of the application
REBOOL REGUIApplication::resume()
{
	return REApplication::resume();
}

void REGUIApplication::registerGUIApplicationClassNames()
{
	this->registerNewClassForNameCallback(REGUIApplicationInternal::creteREView, "REView");
	this->registerNewClassForNameCallback(REGUIApplicationInternal::creteREButton, "REButton");
	this->registerNewClassForNameCallback(REGUIApplicationInternal::creteRETransformedView, "RETransformedView");
	this->registerNewClassForNameCallback(REGUIApplicationInternal::creteRETexture, "RETexture");
	this->registerNewClassForNameCallback(REGUIApplicationInternal::creteRETextField, "RETextField");
	this->registerNewClassForNameCallback(REGUIApplicationInternal::creteREParticleView, "REParticleView");
	this->registerNewClassForNameCallback(REGUIApplicationInternal::creteRELabel, "RELabel");
//	this->registerNewClassForNameCallback(REGUIApplicationInternal::creteREFramedTexture, "REFramedTextureObject");
}

REGUIApplication::REGUIApplication() : REApplication(),
	_renderDevice(NULL),
	_rootViewController(NULL)
{
	if (!REGUIApplicationInternal::currentApplication) 
	{
		REGUIApplicationInternal::currentApplication = this;
	}
	this->registerGUIApplicationClassNames();
}

void REGUIApplication::onReleased()
{
	RE_SAFE_RELEASE(_rootViewController);
}

REGUIApplication::~REGUIApplication()
{
	if (REGUIApplicationInternal::currentApplication == this) 
	{
		REGUIApplicationInternal::currentApplication = NULL;
	}
}

REGUIApplication * REGUIApplication::currentApplication()
{
	return REGUIApplicationInternal::currentApplication;
}


