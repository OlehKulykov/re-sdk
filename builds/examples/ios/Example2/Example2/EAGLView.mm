
#import "EAGLView.h"
#import <QuartzCore/QuartzCore.h>
#include "MyRootViewController.h"

@interface EAGLView(PrivateMethods)
- (void) createFramebuffer;
- (void) deleteFramebuffer;
- (void) EAGLViewAddInit;
@end

@implementation EAGLView

@synthesize context;

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	MyGUIApplication * app = [self GetGUIApplication];
	if (app && ([touches count] == 1))
	{
		UITouch * touch = [touches anyObject];
		CGPoint point = [touch locationInView:self];
		
		const REFloat32 x = point.x * _scale;
		const REFloat32 y = point.y * _scale;
		
		app->OnClickOnScreenStart(x, y);
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	MyGUIApplication * app = [self GetGUIApplication];
	if (app && ([touches count] == 1))
	{
		UITouch * touch = [touches anyObject];
		CGPoint point = [touch locationInView:self];
		
		const REFloat32 x = point.x * _scale;
		const REFloat32 y = point.y * _scale;
		
		app->OnClickMoveOnScreen(x, y);
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	MyGUIApplication * app = [self GetGUIApplication];
	if (app && ([touches count] == 1))
	{
		UITouch * touch = [touches anyObject];
		CGPoint point = [touch locationInView:self];
		
		const REFloat32 x = point.x * _scale;
		const REFloat32 y = point.y * _scale;
		
		app->OnClickOnScreenEnd(x, y);
	}
}

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (MyGUIApplication *) GetGUIApplication
{
	if (_guiApplication) 
	{ 
		return _guiApplication; 
	}
	else
	{
		MyRootViewController * rootViewController = new MyRootViewController();
		if (rootViewController) 
		{
			MyGUIApplication * guiApp = new MyGUIApplication(rootViewController);
			if (guiApp) 
			{
				_rootViewController = rootViewController;
				_guiApplication = guiApp;
				return _guiApplication;
			}
			
			delete rootViewController;
		}
	}
	
	return NULL;
}

- (void) EAGLViewAddInit
{
	_guiApplication = NULL;
	_scale = 1.0f;
	int w = 320;
	int h = 480;
	const float ver = [[[UIDevice currentDevice] systemVersion] floatValue];
	// You can't detect screen resolutions in pre 3.2 devices, but they are all 320x480
	if (ver >= 3.2f)
	{
		UIScreen * mainscr = [UIScreen mainScreen];
		w = mainscr.currentMode.size.width;
		h = mainscr.currentMode.size.height;
	}
	
	if ((w >= 640) && (h >= 960)) // Retina display detected
	{
		// Set contentScale Factor to 2
		_scale = 2.0f;
		self.contentScaleFactor = _scale;
		// Also set our glLayer contentScale Factor to 2
		CAEAGLLayer * eaglLayer = (CAEAGLLayer *)self.layer;
		eaglLayer.contentsScale = _scale;
	}
	
	CAEAGLLayer * eaglLayer = (CAEAGLLayer *)self.layer;
	
	eaglLayer.opaque = TRUE;
	eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
									[NSNumber numberWithBool:FALSE], 
									kEAGLDrawablePropertyRetainedBacking,
									kEAGLColorFormatRGBA8, 
									kEAGLDrawablePropertyColorFormat,
									nil];
	
	[self setUserInteractionEnabled:YES];
}

- (id) init
{
	self = [super init];
	if (self) 
	{
		[self EAGLViewAddInit];
	}
	return self;
}

- (id) initWithFrame:(CGRect)fr
{
	self = [super initWithFrame:fr];
	if (self) 
	{
		[self EAGLViewAddInit];
	}
	return self;
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:.
- (id)initWithCoder:(NSCoder*)coder
{
    self = [super initWithCoder:coder];
	if (self) 
	{
		[self EAGLViewAddInit];
    }
    
    return self;
}

- (void)dealloc
{
	RE_SAFE_DELETE(_guiApplication);
	
    [self deleteFramebuffer];    
    [context release];
    [super dealloc];
}

- (void)setContext:(EAGLContext *)newContext
{
    if (context != newContext) 
	{
        [self deleteFramebuffer];
        
        [context release];
        context = [newContext retain];
        
        [EAGLContext setCurrentContext:nil];
    }
}

- (void) createFramebuffer
{
    if (context && !defaultFramebuffer) 
	{
        [EAGLContext setCurrentContext:context];
        
        // Create default framebuffer object.
        glGenFramebuffers(1, &defaultFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        // Create color render buffer and allocate backing store.
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
		}
		
		RERenderDevice * device = RERenderDevice::GetDefaultDevice();
		MyGUIApplication * app = [self GetGUIApplication];
		if (app && device) 
		{
			device->SetScreenSize((REFloat32)framebufferWidth, (REFloat32)framebufferHeight);
			device->SetRenderSize(2.666666f, 4.0f);
			device->SetFullScreen(true);
			device->SetClearColor(REColor(0.1f, 0.2f, 0.3f, 1.0f));
			device->Initialize();
		}
    }
}

- (void)deleteFramebuffer
{
    if (context)
	{
        [EAGLContext setCurrentContext:context];
        
        if (defaultFramebuffer) 
		{
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }
        
        if (colorRenderbuffer)
		{
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
    }
}

- (void) setFramebuffer
{
    if (context) 
	{
        [EAGLContext setCurrentContext:context];
        
        if (!defaultFramebuffer)
		{
            [self createFramebuffer];
        }
		
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        glViewport(0, 0, framebufferWidth, framebufferHeight);
    }
}

- (BOOL)presentFramebuffer
{
    BOOL success = FALSE;
    
    if (context)
	{
        [EAGLContext setCurrentContext:context];
        
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        
        success = [context presentRenderbuffer:GL_RENDERBUFFER];
    }
    
    return success;
}

- (void)layoutSubviews
{
    // The framebuffer will be re-created at the beginning of the next setFramebuffer method call.
    [self deleteFramebuffer];
}

@end
