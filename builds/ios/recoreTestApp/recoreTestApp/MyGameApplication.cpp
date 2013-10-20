//
//  MyGameApplication.cpp
//  recoreTestApp
//
//  Created by Resident Evil on 16.11.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#include "MyGameApplication.h"

REBOOL MyGameApplication::start()
{	
	if (!_isStarted)
	{
		_isStarted = true;
		
		MyGameRootViewController * rootViewController = (MyGameRootViewController *)this->getRootViewController();
		
		rootViewController->LoadByName(REString("mainmenu"), false);
	}
	return _isStarted;
}

MyGameApplication::MyGameApplication(MyGameRootViewController * rootViewController) : REGUIApplication(),
	_isStarted(false)
{
	REGUIApplication::setRootViewController(rootViewController);
}

MyGameApplication::~MyGameApplication()
{
	
}

