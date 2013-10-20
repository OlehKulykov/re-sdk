//
//  MyGameInterLayer.cpp
//  recoreTestApp
//
//  Created by Resident Evil on 16.11.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#include "MyGameInterLayer.h"

/*

*/
 
 
#define MY_GAME_RENDER_WIDTH (2.66666f)
#define MY_GAME_RENDER_HEIGHT (4.0f)

void MyGameInterLayer::clickStartedAtCoords(const REFloat32 x, const REFloat32 y)
{
	gameApplication->onClickOnScreenStart(x, y);
}

void MyGameInterLayer::clickMovedAtCoords(const REFloat32 x, const REFloat32 y)
{
	gameApplication->onClickMoveOnScreen(x, y);
}

void MyGameInterLayer::clickEndedAtCoords(const REFloat32 x, const REFloat32 y)
{
	gameApplication->onClickOnScreenEnd(x, y);
}

void MyGameInterLayer::update()
{
	gameApplication->update();
}

void MyGameInterLayer::render()
{
	gameApplication->render();
}

void MyGameInterLayer::pause()
{
	gameApplication->pause();
}

void MyGameInterLayer::resume()
{
	gameApplication->resume();
}

void MyGameInterLayer::setup()
{
	RERenderDevice * device = RERenderDevice::GetDefaultDevice();
	if (device)
	{
		device->SetRenderSize(MY_GAME_RENDER_WIDTH, MY_GAME_RENDER_HEIGHT);
		
#if defined(__RE_IPHONE__)
		device->SetFullScreen(true);
#endif
		
		device->SetClearColor(REColor(0.0f, 0.0f, 0.0f, 1.0f));
	}
	
	gameApplication->initialize();
		
	if (!gameApplication->isStarted())
	{
		gameApplication->start();
	}
}

void MyGameInterLayer::setNewScreenSize(const int newWidth, const int newHeight)
{
	RERenderDevice * device = RERenderDevice::GetDefaultDevice();
	if (device)
	{
		device->SetScreenSize((REFloat32)newWidth, (REFloat32)newHeight);
	}
}

REBOOL MyGameInterLayer::isCanUse() const
{
	return (gameApplication.isNotEmpty() && rootViewController.isNotEmpty());
}

void MyGameInterLayer::createGameAppWithViewController()
{
	MyGameRootViewController * rootVC = new MyGameRootViewController();
	if (rootVC)
	{
		MyGameApplication * gameApp = new MyGameApplication(rootVC);
		if (gameApp)
		{
			gameApplication = gameApp;
			rootViewController = rootVC;
			
			rootViewController->setRespondsForUserAction(true);
			rootViewController->setFrame(RERect(0.0f, 0.0f, MY_GAME_RENDER_WIDTH, MY_GAME_RENDER_HEIGHT));
		}
	}
	RE_SAFE_RELEASE(rootVC);
}

REBOOL MyGameInterLayer::addGameResourcesPath(const char * resourcePath)
{
	if (resourcePath)
	{
		REResourcesStorage storage;
		return storage.addResourcesPath(REString(resourcePath));
	}
	return false;
}

void MyGameInterLayer::setReadToBufferCallBack(REBOOL (*ReadToBuf)(const char *, REBuffer *))
{
	REResourcesStorage storage;
	storage.setReadToBufferCallBack(ReadToBuf);
}

void MyGameInterLayer::setIsExistsCallBack(REBOOL (*IsExists)(const char *))
{
	REResourcesStorage storage;
	storage.setIsExistsCallBack(IsExists);
}

MyGameInterLayer::MyGameInterLayer()
{
	this->createGameAppWithViewController();
}

MyGameInterLayer::~MyGameInterLayer()
{
	gameApplication = NULL;
	rootViewController = NULL;
}



