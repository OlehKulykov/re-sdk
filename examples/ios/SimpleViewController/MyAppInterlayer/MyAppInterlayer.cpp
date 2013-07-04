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


#include "MyAppInterlayer.h"

void MyAppInterlayer::update()
{
	myApp->update();
}

void MyAppInterlayer::render()
{
	myApp->render();
}

void MyAppInterlayer::pause()
{
	myApp->pause();
}

void MyAppInterlayer::resume()
{
	myApp->resume();
}

void MyAppInterlayer::setup()
{
	RERenderDevice * device = RERenderDevice::GetDefaultDevice();
	if (device)
	{
		device->SetRenderSize(MyApp::rendererWidth(), MyApp::rendererHeight());
				
		device->SetClearColor(REColor(0.0f, 0.0f, 0.0f, 1.0f));
	}
	
	myApp->initialize();
	
	if (!myApp->isStarted())
	{
		myApp->start();
	}
}

void MyAppInterlayer::setNewScreenSize(const int newWidth, const int newHeight)
{
	RERenderDevice * device = RERenderDevice::GetDefaultDevice();
	if (device)
	{
		device->SetScreenSize((REFloat32)newWidth, (REFloat32)newHeight);
	}
}

REBOOL MyAppInterlayer::isCanUse() const
{
	return ( myApp.isNotNull() && myRootViewController.isNotNull() );
}

void MyAppInterlayer::CreateMyAppWithRootViewController()
{
	MyRootViewController * rootViewController = new MyRootViewController();
	if (rootViewController)
	{
		MyApp * app = new MyApp(rootViewController);
		if (app)
		{
			myApp = app;
			myRootViewController = rootViewController;
		}
		
		RE_SAFE_RELEASE(app);
	}
	
	RE_SAFE_RELEASE(rootViewController);
}

MyAppInterlayer::MyAppInterlayer()
{
	this->CreateMyAppWithRootViewController();
}

MyAppInterlayer::~MyAppInterlayer()
{
	
}

REBOOL MyAppInterlayer::addAppResourcePath(const char * resourcePath)
{
	if (resourcePath)
	{
		REResourcesStorage storage;
		return storage.addResourcesPath(REString(resourcePath));
	}
	return false;
}


