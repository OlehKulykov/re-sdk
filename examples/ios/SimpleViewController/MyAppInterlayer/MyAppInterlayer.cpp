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

void MyAppInterlayer::Update()
{
	myApp->Update();
}

void MyAppInterlayer::Render()
{
	myApp->Render();
}

void MyAppInterlayer::Pause()
{
	myApp->Pause();
}

void MyAppInterlayer::Resume()
{
	myApp->Resume();
}

void MyAppInterlayer::Setup()
{
	RERenderDevice * device = RERenderDevice::GetDefaultDevice();
	if (device)
	{
		device->SetRenderSize(MyApp::RendererWidth(), MyApp::RendererHeight());
				
		device->SetClearColor(REColor(0.0f, 0.0f, 0.0f, 1.0f));
	}
	
	myApp->Initialize();
	
	if (!myApp->IsStarted())
	{
		myApp->Start();
	}
}

void MyAppInterlayer::SetNewScreenSize(const int newWidth, const int newHeight)
{
	RERenderDevice * device = RERenderDevice::GetDefaultDevice();
	if (device)
	{
		device->SetScreenSize((REFloat32)newWidth, (REFloat32)newHeight);
	}
}

REBOOL MyAppInterlayer::IsCanUse() const
{
	return ( myApp.IsNotNull() && myRootViewController.IsNotNull() );
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

REBOOL MyAppInterlayer::AddAppResourcePath(const char * resourcePath)
{
	if (resourcePath)
	{
		REResourcesStorage storage;
		return storage.AddResourcesPath(REString(resourcePath));
	}
	return false;
}


