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

- (BOOL) initInterlayer
{
	MyAppInterlayer * interLayer = new MyAppInterlayer();
	if (interLayer)
	{
		if (interLayer->IsCanUse())
		{
			[iOSAppInterlayer initAppResources];
			_interLayer = interLayer;
			return YES;
		}
	}
	
	RE_SAFE_DELETE(interLayer);
	return NO;
}

- (id) init
{
	self = [super init];
	if (self)
	{
		if ([self initInterlayer])
		{
			return self;
		}
	}
	return nil;
}

- (void) dealloc
{
	RE_SAFE_DELETE(_interLayer);
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
