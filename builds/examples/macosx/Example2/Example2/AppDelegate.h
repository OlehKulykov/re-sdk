

#import <Cocoa/Cocoa.h>
#import "MyWindow.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet MyWindow * window;

@property (assign) IBOutlet MyOpenGLView * openGLView;

@end
