
#import <UIKit/UIKit.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#include "MyGUIApplication.h"
#include "MyRootViewController.h"

@class EAGLContext;

@interface EAGLView : UIView 
{
    GLint framebufferWidth;
    GLint framebufferHeight;
    GLuint defaultFramebuffer;
	GLuint colorRenderbuffer;
@private	
	MyGUIApplication * _guiApplication;
	MyRootViewController * _rootViewController;
	CGFloat _scale;
}

@property (nonatomic, retain) EAGLContext *context;

- (void) setFramebuffer;
- (BOOL) presentFramebuffer;
- (MyGUIApplication *) GetGUIApplication;

@end
