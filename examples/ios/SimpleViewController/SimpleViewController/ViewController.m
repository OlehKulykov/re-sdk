
#import "ViewController.h"
#import "MyGLView.h"
#import "iOSAppInterlayer.h"

@interface ViewController ()
{
   
}

@property (strong, nonatomic) EAGLContext * context;


- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController

- (void)dealloc
{
    [_context release];
    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1] autorelease];

    if (!self.context)
	{
        NSLog(@"Failed to create ES context");
    }
    
    MyGLView * view = (MyGLView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
	
	self.preferredFramesPerSecond = 30;
	self.delegate = [iOSAppInterlayer defaultInterlayer];
	view.delegate = [iOSAppInterlayer defaultInterlayer];
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context)
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

- (void) viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	
	int hhhh = [(GLKView *)self.view drawableHeight];
	int wwww = [(GLKView *)self.view drawableWidth];
	
	wwww = 0;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
	
	[[iOSAppInterlayer defaultInterlayer] onSetupGL];
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
}

@end
