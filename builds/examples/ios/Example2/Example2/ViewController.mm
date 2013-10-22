
#import <QuartzCore/QuartzCore.h>

#import "ViewController.h"
#import "EAGLView.h"

#include "MyGUIApplication.h"
#include "MyRootViewController.h"

@interface ViewController()
@property (nonatomic, retain) EAGLContext * context;
@property (nonatomic, assign) CADisplayLink * displayLink;
- (void) drawView:(id)sender;
@end



@implementation ViewController

@synthesize animating;
@synthesize context;
@synthesize displayLink;

- (void) CustomInit
{
	displayLinkSupported = NO;	
    animationTimer = nil;
    self.displayLink = nil;
	animating = NO;
    animationFrameInterval = 1;
	
    EAGLContext * aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    if (!aContext)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:aContext])
        NSLog(@"Failed to set ES context current");
    
	self.context = aContext;
	[aContext release];
	
    [(EAGLView *)self.view setContext:context];
    [(EAGLView *)self.view setFramebuffer];
	
	UIScreen * mainScren = [UIScreen mainScreen];
	if (mainScren) 
		displayLinkSupported = [mainScren respondsToSelector:@selector(displayLinkWithTarget:selector:)];
}

- (void) loadView
{
	EAGLView * eaglView = [[EAGLView alloc] init];
	[eaglView setFrame:[[UIScreen mainScreen] bounds]];
	[self setView:eaglView];
	[eaglView release];
}

- (id) init
{
	self = [super init];
	if (self) 
	{
		[self CustomInit];
	}
	return self;
}

- (void)dealloc
{ 
    // Tear down context.
    if ([EAGLContext currentContext] == context)
	{
        [EAGLContext setCurrentContext:nil];
    }
	
    [context release];
    
    [super dealloc];
}

- (void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
    [self startAnimation];
}

- (void)viewWillDisappear:(BOOL)animated
{
	[super viewWillDisappear:animated];
    [self stopAnimation];
}

- (void)viewDidUnload
{
	[super viewDidUnload];

    // Tear down context.
    if ([EAGLContext currentContext] == context)
	{
        [EAGLContext setCurrentContext:nil];
	}
	self.context = nil;	
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
//	Frame interval defines how many display frames must pass between each time the display link fires.
//	The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
    if (frameInterval >= 1) 
	{
        animationFrameInterval = frameInterval;
        
        if (animating) 
		{
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void) startAnimation
{
    if (!animating) 
	{
		if (displayLinkSupported) 
		{
			CADisplayLink * aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
			if (aDisplayLink) 
			{
				[aDisplayLink setFrameInterval:animationFrameInterval];
				[aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
				self.displayLink = aDisplayLink;
				animating = YES;
			}
		}
		else
		{
			animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) 
															  target:self 
															selector:@selector(drawView:) 
															userInfo:nil 
															 repeats:YES];
			if (animationTimer) 
			{
				animating = YES;
			}
		}
	}
	
	if (animating) 
	{
		MyGUIApplication * app = [(EAGLView *)self.view GetGUIApplication];
		if (app) 
		{
			app->Initialize();
			app->Start();
		}
	}
}

- (void) stopAnimation
{
    if (animating) 
	{
		if (self.displayLink) 
		{
			[self.displayLink invalidate];
			self.displayLink = nil;
			animating = NO;
		}
        
		if (animationTimer) 
		{
			[animationTimer invalidate];
            animationTimer = nil;
			animating = NO;
		}
		
    }
}

- (void) drawFrame
{
    [(EAGLView *)self.view setFramebuffer];
	
	MyGUIApplication * app = [(EAGLView *)self.view GetGUIApplication];
	if (app) 
	{
		app->Update();
		
		app->Render();
	}
	
    [(EAGLView *)self.view presentFramebuffer];
}

- (void) drawView:(id)sender
{
    [self drawFrame];
}

@end
