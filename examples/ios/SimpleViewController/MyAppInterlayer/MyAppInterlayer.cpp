//
//  MyAppInterlayer.cpp
//  SimpleViewController
//
//  Created by Resident Evil on 21.12.12.
//  Copyright (c) 2012 n. All rights reserved.
//

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


