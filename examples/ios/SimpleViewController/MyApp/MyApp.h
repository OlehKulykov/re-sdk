//
//  MyApp.h
//  SimpleViewController
//
//  Created by Resident Evil on 21.12.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#ifndef SimpleViewController_MyApp_h
#define SimpleViewController_MyApp_h

#include "MyRootViewController.h"

class MyApp : public REGUIApplication
{
protected:
	REBOOL _isStarted;
public:
	const REBOOL IsStarted() const;
	
	REBOOL Start();
	
	
	MyApp(MyRootViewController * rootViewController = NULL);
	virtual ~MyApp();
	
	
	static REFloat32 RendererWidth();
	static REFloat32 RendererHeight();
};

#endif
