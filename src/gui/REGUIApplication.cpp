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
	if (view->IsVisible() && (view->IsRespondsForUserAction() || (!view->IsInterceptsUserAction()))) 
	{
		if (view->GetScreenFrame().IsPointInRect(screenX, screenY)) 
		{
			storeArray->Add(view);
			REObjectsArray * subViews = (REObjectsArray *)view->GetSubViewsArray();
			if (subViews) 
			{
				for (REUInt32 i = 0; i < subViews->Count(); i++) 
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
	onClickViews->Clear();
	userActionViews->Clear();
	REGUIApplicationPrivate::FindOnClickViewsPrivate(onClickViews, view, screenX, screenY);
	REUInt32 i = onClickViews->Count();
	while (i) 
	{
		i--;
		REView * v = (*onClickViews)[i];
		userActionViews->Add(v);
		if (v->IsInterceptsUserAction())
		{
			return;
		}
	}
}


/* REObject */
const REUInt32 REGUIApplication::GetClassIdentifier() const
{
	return REGUIApplication::ClassIdentifier();
}

const REUInt32 REGUIApplication::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REGUIApplication");
	return clasIdentif;
}

REBOOL REGUIApplication::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REGUIApplication::ClassIdentifier() == classIdentifier) ||
			(REObject::GenerateClassIdentifierFromClassName("IRERenderable") == classIdentifier) ||
			REApplication::IsImplementsClass(classIdentifier));
}

void REGUIApplication::Render()
{
	if (_renderDevice && _rootViewController) 
	{
		_renderDevice->StartRendering();
		_renderDevice->Enter2DMode();
		
		_rootViewController->Render(); 
		
		_renderDevice->Leave2DMode();
		_renderDevice->EndRendering();
	}
}

void REGUIApplication::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	if (_renderDevice && _rootViewController) 
	{
		_renderDevice->StartRendering();
		_renderDevice->Enter2DMode();
		
		_rootViewController->RenderWithOffset(offsetX, offsetY); 
		
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
		_userActionViews[i]->UserCursorMoved(x, y);
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
	 v->UserCursorMoved(x, y);
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
 const REObjectsArray * sv = view->GetSubViewsArray();
 if (sv) 
 {
 REObjectsArray * subViews = const_cast<REObjectsArray *>(sv);
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

void REGUIApplication::OnClickOnScreenStart(const REFloat32 screenX, const REFloat32 screenY)
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
		for (REUInt32 i = 0; i < _userActionViews.Count(); i++) 
		{
			_userActionViews[i]->UserActionClickDidStart(_lastUserClickPos.x, _lastUserClickPos.y);
		}
	}
}

void REGUIApplication::OnClickOnScreenEnd(const REFloat32 screenX, const REFloat32 screenY)
{
	if (_userActionViews.Count() && _renderDevice) 
	{
		RESize ratio(_renderDevice->GetScreenToRenderSizeRatio());
		
		const REFloat32 x = ratio.width * screenX;
		const REFloat32 y = ratio.height * screenY;
		
		for (REUInt32 i = 0; i < _userActionViews.Count(); i++) 
		{
			_userActionViews[i]->UserActionClickDidEnd(_lastUserClickPos.x, _lastUserClickPos.y, x, y);
		}
		
		if (_isLastUserClickMoving)
		{
			_isLastUserClickMoving = 0;
			
			if ((fabs(_lastUserClickPos.x - x) > 0.0) || (fabs(_lastUserClickPos.y - y) > 0.0))
			{
				for (REUInt32 i = 0; i < _userActionViews.Count(); i++) 
				{
					_userActionViews[i]->UserActionClickMovingDidEnd(_lastUserClickPos.x, _lastUserClickPos.y, x, y);
				}
			}
		}
	}
}

void REGUIApplication::OnClickMoveOnScreen(const REFloat32 screenX, const REFloat32 screenY)
{
	if (_userActionViews.Count() && _renderDevice) 
	{
		RESize ratio(_renderDevice->GetScreenToRenderSizeRatio());
		
		const REFloat32 x = ratio.width * screenX;
		const REFloat32 y = ratio.height * screenY;
		
		if ( (_lastUserClickPos.x != x) || (_lastUserClickPos.y != y) )
		{
			_isLastUserClickMoving = 1;
			for (REUInt32 i = 0; i < _userActionViews.Count(); i++)
			{
				_userActionViews[i]->UserActionClickMoving(_lastUserClickPos.x, _lastUserClickPos.y, x, y);
			}
		}
		
	}
}

REBOOL REGUIApplication::InitGUIApplication(REViewController * rootViewController, void * windowHandle)
{
	if (rootViewController) 
	{
		this->SetRootViewController(rootViewController);
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
	
	this->AddToErrorDescription(REString("Can't initialize application default RERenderDevice."));
	return false;
}

REViewController * REGUIApplication::GetRootViewController() const
{
	return _rootViewController;
}

/// Setting new root view controller. 
/// Previous view controller will be Release()'ed and 'newRootViewController' exists it will be Retain()ed.
void REGUIApplication::SetRootViewController(REViewController * newRootViewController)
{
	RE_SAFE_RETAIN(_rootViewController, newRootViewController);
}

/// Try to initialize base REApplication and than GUI application.
REBOOL REGUIApplication::Initialize()
{
	return this->InitGUIApplication(NULL, NULL);
}

/// Try to initialize base REApplication and than GUI application with root view controller.
/// If default RERenderDevice if not initialized it will try to init.
/// 'rootViewController' will be Retain()'ed.
REBOOL REGUIApplication::Initialize(REViewController * rootViewController)
{
	return this->InitGUIApplication(rootViewController, NULL);
}

/// Try to initialize base REApplication and than GUI application with root view controller
/// and window handle.
/// Window handle using when creating application using DirectX. 
/// If default RERenderDevice if not initialized it will try to init.
/// 'rootViewController' will be Retain()'ed.
REBOOL REGUIApplication::Initialize(REViewController * rootViewController, void * windowHandle)
{
	return this->InitGUIApplication(rootViewController, windowHandle);
}

/// Checks is application successfully initialized and ready to work.
REBOOL REGUIApplication::IsOK() const
{
	if (REApplication::IsOK()) 
	{
		if (_renderDevice && _rootViewController) 
		{
			return true;
		}
	}
	return false;
}

/// Suspends execution of the application
REBOOL REGUIApplication::Pause()
{
	return REApplication::Pause();
}

/// Resumes execution of the application
REBOOL REGUIApplication::Resume()
{
	return REApplication::Resume();
}

REGUIApplication::REGUIApplication() : REApplication(),
	_renderDevice(NULL),
	_rootViewController(NULL)
{
	
}

void REGUIApplication::OnReleased()
{
	RE_SAFE_RELEASE(_rootViewController);
	
	REApplication::OnReleased();
}

REGUIApplication::~REGUIApplication()
{
	
}

REGUIApplication * REGUIApplication::Create()
{
	REGUIApplication * newGUIApp = new REGUIApplication();
	return newGUIApp;
}


