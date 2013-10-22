//
//  MyGameGLKDelegate.m
//  recoreTestApp
//
//  Created by Resident Evil on 16.11.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#import "MyGameGLKDelegate.h"
#import "MyGameInterLayer.h"
#import "IOSFileReaderCallBacks.h"


@interface MyGameGLKDelegate()
{
@private
	MyGameInterLayer * _interLayer;
}

+ (void) InitGameResources;

@end


@implementation MyGameGLKDelegate


- (void) touchOnScreenBeganAtCoordX:(const CGFloat)x andY:(const CGFloat)y
{
	_interLayer->clickStartedAtCoords(x, y);
}

- (void) touchOnScreenMovedAtCoordX:(const CGFloat)x andY:(const CGFloat)y
{
	_interLayer->clickMovedAtCoords(x, y);
}

- (void) touchOnScreenEndedAtCoordX:(const CGFloat)x andY:(const CGFloat)y
{
	_interLayer->clickEndedAtCoords(x, y);
}

- (void) setupGL
{
	_interLayer->setup();
}

- (void) updateScreenFrame:(const CGRect)frame
{
	_interLayer->setNewScreenSize((int)frame.size.width, (int)frame.size.height);
}

#pragma mark GLKViewControllerDelegate
- (void)glkViewControllerUpdate:(GLKViewController *)controller
{
	_interLayer->update();
}

- (void)glkViewController:(GLKViewController *)controller willPause:(BOOL)pause
{
	if (pause)
	{
		_interLayer->pause();
	}
	else
	{
		_interLayer->resume();
	}
}

#pragma mark GLKViewDelegate
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	_interLayer->render();
}

- (id) init
{
	self = [super init];
	if (self)
	{
		_interLayer = new MyGameInterLayer();
		
		assert(_interLayer);
		
		if (_interLayer)
		{
			if (_interLayer->isCanUse())
			{
				[MyGameGLKDelegate InitGameResources];
				return self;
			}
		}
		
		RE_SAFE_DELETE(_interLayer);
		[self release];
	}
	return nil;
}

- (void) dealloc
{
	RE_SAFE_DELETE(_interLayer);
	
	[super dealloc];
}

+ (MyGameGLKDelegate *) defaultDelegate
{
	static MyGameGLKDelegate * staticInstance = [[MyGameGLKDelegate alloc] init];
	return staticInstance;
}

+ (void) InitGameResources
{
	NSString * resourcePath = [[NSBundle bundleForClass:[MyGameGLKDelegate class]] pathForResource:@"data" ofType:@"dat"];
	if (resourcePath)
	{
		if (!MyGameInterLayer::addGameResourcesPath([resourcePath UTF8String]))
		{
			NSLog(@"Error of adding game resource path: %@", resourcePath);
		}
	}
	
	
	MyGameInterLayer::setReadToBufferCallBack(IOSFileReaderCallBacks::ReadFileCallBack);
	MyGameInterLayer::setIsExistsCallBack(IOSFileReaderCallBacks::IsFileExistsCallBack);
}

@end
