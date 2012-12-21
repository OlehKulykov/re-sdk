//
//  iOSAppIntrlayer.m
//  SimpleViewController
//
//  Created by Resident Evil on 21.12.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#import "iOSAppInterlayer.h"
#import "MyAppInterlayer.h"

@interface iOSAppInterlayer()
{
@protected
	MyAppInterlayer * _interLayer;
}

+ (void) initAppResources;

@end

@implementation iOSAppInterlayer

- (void) updateScreenFrame:(CGRect)frame
{
	_interLayer->SetNewScreenSize((int)frame.size.width, (int)frame.size.height);
}

- (void) onSetupGL
{
	_interLayer->Setup();
}

#pragma mark GLKViewControllerDelegate
- (void) glkViewControllerUpdate:(GLKViewController *)controller
{
	_interLayer->Update();
}

- (void) glkViewController:(GLKViewController *)controller willPause:(BOOL)pause
{
	if (pause)
	{
		_interLayer->Pause();
	}
	else
	{
		_interLayer->Resume();
	}
}

#pragma mark GLKViewDelegate
- (void) glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	_interLayer->Render();
}

- (id) init
{
	self = [super init];
	if (self)
	{
		_interLayer = new MyAppInterlayer();
		if (_interLayer)
		{
			if (_interLayer->IsCanUse())
			{
				[iOSAppInterlayer initAppResources];
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

+ (void) initAppResources
{
	NSString * resourcePath = nil;
	resourcePath = [[NSBundle bundleForClass:[self class]] pathForResource:@"data0"
																	ofType:@"dat"];
	MyAppInterlayer::AddAppResourcePath([resourcePath UTF8String]);
	
	
	resourcePath = [[NSBundle bundleForClass:[self class]] resourcePath];
	MyAppInterlayer::AddAppResourcePath([resourcePath UTF8String]);
}

+ (iOSAppInterlayer *) defaultInterlayer
{
	static iOSAppInterlayer * staticInstance = [[iOSAppInterlayer alloc] init];
	return staticInstance;
}


@end
