
#import "MyGLView.h"
#import "iOSAppInterlayer.h"

@interface MyGLView()

- (void) onFrameChanged;

@end

@implementation MyGLView

- (void) onFrameChanged
{
	CGRect screenFrame = [self bounds];
	
	const CGFloat scaleFactor = self.contentScaleFactor;
	
	screenFrame.size.width *= scaleFactor;
	screenFrame.size.height *= scaleFactor;
	
	[[iOSAppInterlayer defaultInterlayer] updateScreenFrame:screenFrame];
}

- (void) setFrame:(CGRect)newFrame
{
	[super setFrame:newFrame];
	
	[self onFrameChanged];
}

@end
