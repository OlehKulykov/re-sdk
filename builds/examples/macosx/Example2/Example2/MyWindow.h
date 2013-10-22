

#import <AppKit/AppKit.h>
#import "MyOpenGLView.h"

@interface MyWindow : NSWindow

- (BOOL) IsFullScreenMode;

- (void) EnterFullScreen;

- (void) ExitFullScreen;

@end
