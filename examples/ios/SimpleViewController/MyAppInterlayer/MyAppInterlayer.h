//
//  MyAppInterlayer.h
//  SimpleViewController
//
//  Created by Resident Evil on 21.12.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#ifndef SimpleViewController_MyAppInterlayer_h
#define SimpleViewController_MyAppInterlayer_h

#include "MyApp.h"
#include "MyRootViewController.h"

class MyAppInterlayer
{
private:
	void CreateMyAppWithRootViewController();
	
public:
	RERetainProperty<MyApp *> myApp;
	RERetainProperty<MyRootViewController *> myRootViewController;
	
	void Update();
	void Render();
	void Pause();
	void Resume();
	void Setup();
	void SetNewScreenSize(const int newWidth, const int newHeight);
	REBOOL IsCanUse() const;
	
	MyAppInterlayer();
	~MyAppInterlayer();
	
	static REBOOL AddAppResourcePath(const char * resourcePath);
};


#endif
