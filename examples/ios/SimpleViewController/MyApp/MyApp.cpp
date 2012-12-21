//
//  MyApp.cpp
//  SimpleViewController
//
//  Created by Resident Evil on 21.12.12.
//  Copyright (c) 2012 n. All rights reserved.
//

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


