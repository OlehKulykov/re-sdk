//
//  MyGameApplication.h
//  recoreTestApp
//
//  Created by Resident Evil on 16.11.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#ifndef recoreTestApp_MyGameApplication_h
#define recoreTestApp_MyGameApplication_h

#include "MyGameRootViewController.h"

class MyGameApplication : public REGUIApplication
{
private:
	REBOOL _isStarted;
public:
	const REBOOL isStarted() const { return _isStarted; }
	
	REBOOL start();
	
	MyGameApplication(MyGameRootViewController * rootViewController);
	virtual ~MyGameApplication();
};


#endif
