

#import <AppKit/AppKit.h>
#include "MyGUIApplication.h"
#include "MyRootViewController.h"

@interface MyOpenGLView : NSOpenGLView
{
@private	
	MyGUIApplication * _guiApplication;
	MyRootViewController * _rootViewController;
	
	NSTimer * _renderTimer;
}

- (MyGUIApplication *) GetGUIApplication;

- (void) StartGUIApplication;

@end
