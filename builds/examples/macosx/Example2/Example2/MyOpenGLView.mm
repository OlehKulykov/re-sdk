

#import "MyOpenGLView.h"

#import <OpenGL/gl.h>
#import <OpenGL/OpenGL.h>

@interface MyOpenGLView(Private)

- (void) RenderScene;

@end

@implementation MyOpenGLView

// left mouse down
- (void)mouseDown:(NSEvent *)theEvent
{
	MyGUIApplication * app = [self GetGUIApplication];
	if (app) 
	{
		const NSPoint eventLocation = [theEvent locationInWindow];
		const REFloat32 x = (REFloat32)eventLocation.x;
		const REFloat32 y = (REFloat32)(self.frame.size.height - eventLocation.y);
		app->OnClickOnScreenStart(x, y);
	}
}

// User has moved the mouse with the left button pressed.
- (void)mouseDragged:(NSEvent *)theEvent
{
	MyGUIApplication * app = [self GetGUIApplication];
	if (app) 
	{
		const NSPoint eventLocation = [theEvent locationInWindow];
		const REFloat32 x = (REFloat32)eventLocation.x;
		const REFloat32 y = (REFloat32)(self.frame.size.height - eventLocation.y);
#ifdef __RE_OS_DESKTOP__		
		app->OnMouseMoveOnScreen(x, y);
#endif		
		app->OnClickMoveOnScreen(x, y);
	}
}

// Mouse has moved specified.
- (void)mouseUp:(NSEvent *)theEvent
{
	MyGUIApplication * app = [self GetGUIApplication];
	if (app) 
	{
		const NSPoint eventLocation = [theEvent locationInWindow];
		const REFloat32 x = (REFloat32)eventLocation.x;
		const REFloat32 y = (REFloat32)(self.frame.size.height - eventLocation.y);
		app->OnClickOnScreenEnd(x, y);
	}
}

// Mouse has moved specified.
- (void)mouseMoved:(NSEvent *)theEvent
{
#ifdef __RE_OS_DESKTOP__	
	MyGUIApplication * app = [self GetGUIApplication];
	if (app) 
	{
		const NSPoint eventLocation = [theEvent locationInWindow];
		const REFloat32 x = (REFloat32)eventLocation.x;
		const REFloat32 y = (REFloat32)(self.frame.size.height - eventLocation.y);
		app->OnMouseMoveOnScreen(x, y);
	}
#endif	
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

- (void) StartGUIApplication
{
	MyGUIApplication * app = [self GetGUIApplication];
	if (app) 
	{
		if (!app->IsStarted()) 
		{
			app->Start();
		}
	}
}

- (void) RenderTimerFireMethod:(NSTimer*)theTimer
{
	[self setNeedsDisplay:YES];
}

- (void) update
{
	[super update];
	
	RERenderDevice * device = RERenderDevice::GetDefaultDevice();
	MyGUIApplication * app = [self GetGUIApplication];
	if (app && device) 
	{
		CGRect selfFrame = [self frame];
		device->SetScreenSize((REFloat32)selfFrame.size.width, (REFloat32)selfFrame.size.height);
		device->SetRenderSize(2.666666f, 4.0f);
		device->SetFullScreen(true);
	}
}

- (void) reshape
{
	[super reshape];
	
	RERenderDevice * device = RERenderDevice::GetDefaultDevice();
	MyGUIApplication * app = [self GetGUIApplication];
	if (app && device) 
	{
		CGRect selfFrame = [self frame];
		device->SetScreenSize((REFloat32)selfFrame.size.width, (REFloat32)selfFrame.size.height);
		device->SetRenderSize(2.666666f, 4.0f);
		device->SetFullScreen(true);
	}
	
	_renderTimer = [NSTimer scheduledTimerWithTimeInterval:1.0 / 60.0 
													target:self
												  selector:@selector(RenderTimerFireMethod:)
												  userInfo:nil
												   repeats:YES];
}

- (void) prepareOpenGL
{
	[super prepareOpenGL];
	
	GLint swapInt = 1;
	[[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
	
	RERenderDevice * device = RERenderDevice::GetDefaultDevice();
	MyGUIApplication * app = [self GetGUIApplication];
	if (app && device) 
	{
		CGRect selfFrame = [self frame];
		device->SetScreenSize((REFloat32)selfFrame.size.width, (REFloat32)selfFrame.size.height);
		device->SetRenderSize(2.666666f, 4.0f);
		device->SetFullScreen(true);
		device->SetClearColor(REColor(0.1f, 0.2f, 0.3f, 1.0f));
		device->Initialize();
	}
}

- (void) RenderScene
{
	MyGUIApplication * app = [self GetGUIApplication];
	if (app) 
	{
		app->Update();
		
		app->Render();
	}
}

- (void) drawRect:(NSRect)theRect
{	
	[[self openGLContext] makeCurrentContext];
	
	[self RenderScene];
	
	[[self openGLContext] flushBuffer];

}

- (void) dealloc
{
	RE_SAFE_DELETE(_guiApplication);
	
	[super dealloc];
}

@end
