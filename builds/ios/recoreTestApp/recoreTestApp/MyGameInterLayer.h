//
//  MyGameInterLayer.h
//  recoreTestApp
//
//  Created by Resident Evil on 16.11.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#ifndef recoreTestApp_MyGameInterLayer_h
#define recoreTestApp_MyGameInterLayer_h


#include "MyGameApplication.h"
#include "MyGameRootViewController.h"


class MyGameInterLayer
{
private:
	void createGameAppWithViewController();
	
public:
	void update();
	void render();
	void pause();
	void resume();
	void setup();
	void setNewScreenSize(const int newWidth, const int newHeight);
	void clickStartedAtCoords(const REFloat32 x, const REFloat32 y);
	void clickMovedAtCoords(const REFloat32 x, const REFloat32 y);
	void clickEndedAtCoords(const REFloat32 x, const REFloat32 y);
	
	static REBOOL addGameResourcesPath(const char * resourcePath);

	static void setReadToBufferCallBack(REBOOL (*ReadToBuf)(const char *, REBuffer *));

	static void setIsExistsCallBack(REBOOL (*IsExists)(const char *));
	
	REPtr<MyGameApplication> gameApplication;
	REPtr<MyGameRootViewController> rootViewController;
	
	REBOOL isCanUse() const;
	
	MyGameInterLayer();
	~MyGameInterLayer();
};


#endif

