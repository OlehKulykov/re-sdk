//
//  MyGameApplication.cpp
//  recoreTestApp
//
//  Created by Resident Evil on 16.11.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#include "MyGameApplication.h"

void * ___newREView()
{
	RELabel * v = RELabel::create();
	return REPtrCast<void, RELabel>(v);
}

void * ___newRETexure()
{
	REFramedTextureObject * t = REFramedTextureObject::Create();
	return REPtrCast<void, REFramedTextureObject>(t);
}

REBOOL MyGameApplication::start()
{	
	if (!_isStarted)
	{
		_isStarted = true;
		
		MyGameRootViewController * rootViewController = (MyGameRootViewController *)this->getRootViewController();
		
		rootViewController->loadByName(REString("mainmenu"), false);
	}
	return _isStarted;
}

MyGameApplication::MyGameApplication(MyGameRootViewController * rootViewController) : REGUIApplication(),
	_isStarted(false)
{
	REApplication::currentApplication()->registerNewClassForNameCallback(___newREView, "REView");
	REView * v = REApplication::currentApplication()->createClassWithClassName<REView>("REView");
	
	REApplication::currentApplication()->registerNewClassForNameCallback(___newRETexure, "RETextureObject");
	
	
	REGUIApplication::setRootViewController(rootViewController);
}

MyGameApplication::~MyGameApplication()
{
	
}

