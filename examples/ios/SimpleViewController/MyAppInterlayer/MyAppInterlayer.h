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
