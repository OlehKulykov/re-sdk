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


#import "MyGLViewController.h"
#import "MyGLView.h"
#import "iOSAppInterlayer.h"

@interface MyGLViewController ()
{
   
}

@property (strong, nonatomic) EAGLContext * context;


- (void)setupGL;
- (void)tearDownGL;

@end

@implementation MyGLViewController

- (void) loadView
{
	EAGLContext * context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
	if (context) 
	{
		self.context = context;
		
		MyGLView * view = [[MyGLView alloc] initWithFrame:[[UIScreen mainScreen] bounds] context:_context];
		view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
		view.delegate = [iOSAppInterlayer defaultInterlayer];
		
		[self setView:view];
		
		self.preferredFramesPerSecond = 30;
		self.delegate = [iOSAppInterlayer defaultInterlayer];
	}
	else
	{
		NSLog(@"Failed to create ES context");
	}
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self setupGL];
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == _context)
	{
        [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
	{
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    }
	else
	{
        return YES;
    }
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:_context];
	
	[[iOSAppInterlayer defaultInterlayer] onSetupGL];
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:_context];
    
}

@end
