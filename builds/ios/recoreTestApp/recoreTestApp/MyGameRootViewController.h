//
//  MyGameRootViewController.h
//  recoreTestApp
//
//  Created by Resident Evil on 16.11.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#ifndef recoreTestApp_MyGameRootViewController_h
#define recoreTestApp_MyGameRootViewController_h


class MyGameRootViewController : public REViewController
{
	
	
public:
	void __Test();
	
	virtual void render();
	virtual void renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
	MyGameRootViewController();
	virtual ~MyGameRootViewController();
	
};


#endif
