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
