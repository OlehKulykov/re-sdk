/*
 *   Copyright 2012 - 2013 Kulikov Oleg
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


#include "MyApp.h"

const REBOOL MyApp::IsStarted() const
{
	return _isStarted;
}

REBOOL MyApp::Start()
{
	if (!_isStarted)
	{
		_isStarted = true;
		
		MyRootViewController * rootViewController = (MyRootViewController *)this->GetRootViewController();
		
		rootViewController->LoadByName(REString("mainmenu"), false);
	}
	return _isStarted;
}

MyApp::MyApp(MyRootViewController * rootViewController) : REGUIApplication(),
	_isStarted(false)
{
	if (rootViewController)
	{
		REGUIApplication::SetRootViewController(rootViewController);
		
		rootViewController->SetRespondsForUserAction(true);
		
		RERect controllerFrame(0.0f, 0.0f, MyApp::RendererWidth(), MyApp::RendererHeight());
		rootViewController->SetFrame(controllerFrame);
	}
}

MyApp::~MyApp()
{
	
}

REFloat32 MyApp::RendererWidth()
{
	return 2.66666f;
}

REFloat32 MyApp::RendererHeight()
{
	return 4.0f;
}


