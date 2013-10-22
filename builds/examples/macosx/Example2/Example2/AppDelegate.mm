

#import "AppDelegate.h"

#include <recore.h>

@implementation AppDelegate

@synthesize window = _window;
@synthesize openGLView = _openGLView;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{	
	// Insert code here to initialize your application
	REResourcesStorage resourcesStorage;
	NSString * dataPath = [[NSBundle mainBundle] pathForResource:@"data0" ofType:@"dat"];
	if (dataPath) 
	{
		resourcesStorage.AddResourcesPath(REString([dataPath UTF8String]));
	}
	
	dataPath = [[NSBundle mainBundle] pathForResource:@"media0" ofType:@"dat"];
	if (dataPath) 
	{
		resourcesStorage.AddResourcesPath(REString([dataPath UTF8String]));
	}
	
	// To make fullscreen. You call it when needed.
	//[self.window EnterFullScreen];
	
	[self.openGLView StartGUIApplication];
}

@end
