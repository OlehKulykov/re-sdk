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
		if (view->getScreenFrame().isPointInRect(screenX, screenY)) 
		{
			storeArray->add(view);
			REArrayObject * subViews = (REArrayObject *)view->getSubViewsArray();
			if (subViews) 
			{
				for (REUInt32 i = 0; i < subViews->count(); i++) 
				{
					REView * subView = (REView *)(*subViews)[i];
					if (REGUIApplicationPrivate::FindOnClickViewsPrivate(storeArray, subView, screenX, screenY))
					{
						return 1;
					}
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


/* REObject */
const REUInt32 REGUIApplication::getClassIdentifier() const
{
	return REGUIApplication::classIdentifier();
}

const REUInt32 REGUIApplication::classIdentifier()
{
	const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("REGUIApplication");
	return clasIdentif;
}

REBOOL REGUIApplication::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REGUIApplication::classIdentifier() == classIdentifier) ||
			(REObject::generateClassIdentifierFromClassName("IRERenderable") == classIdentifier));
}

void REGUIApplication::render()
{
	if (_renderDevice && _rootViewController) 
	{
		_renderDevice->StartRendering();
		_renderDevice->Enter2DMode();
		
		_rootViewController->render(); 
		
		_renderDevice->Leave2DMode();
		_renderDevice->EndRendering();
	}
}

void REGUIApplication::renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	if (_renderDevice && _rootViewController) 
	{
		_renderDevice->StartRendering();
		_renderDevice->Enter2DMode();
		
		_rootViewController->renderWithOffset(offsetX, offsetY); 
		
		_renderDevice->Leave2DMode();
		_renderDevice->EndRendering();
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
		RESize ratio( _renderDevice->GetScreenToRenderSizeRatio() );
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
		RESize ratio(_renderDevice->GetScreenToRenderSizeRatio());
		
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
		RESize ratio(_renderDevice->GetScreenToRenderSizeRatio());
		
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
	RERenderDevice * renderDevice = RERenderDevice::GetDefaultDevice();
	if (renderDevice) 
	{
		if (renderDevice->IsInitialized()) 
		{
			isOk = true; 
		}
		else if (windowHandle)
		{
			if (renderDevice->InitializeWithWindow(windowHandle)) 
			{
				isOk = true; 
			}
			else if (renderDevice->Initialize()) 
			{
				isOk = true;
			} 
		}
		else
		{
			if (renderDevice->Initialize()) 
			{
				isOk = true; 
			} 
			else if (renderDevice->InitializeWithWindow(NULL))
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

REGUIApplication::REGUIApplication() : REApplication(),
	_renderDevice(NULL),
	_rootViewController(NULL)
{
	
}

void REGUIApplication::onReleased()
{
	RE_SAFE_RELEASE(_rootViewController);
}

REGUIApplication::~REGUIApplication()
{
	
}

REGUIApplication * REGUIApplication::create()
{
	REGUIApplication * newGUIApp = new REGUIApplication();
	return newGUIApp;
}


