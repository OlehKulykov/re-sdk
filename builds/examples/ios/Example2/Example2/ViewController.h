
#import <UIKit/UIKit.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@interface ViewController : UIViewController
{
    EAGLContext * context;
	NSTimer * animationTimer;
	NSInteger animationFrameInterval;
	BOOL animating;
	BOOL displayLinkSupported;
}

@property (readonly, nonatomic, getter = isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

- (void) startAnimation;
- (void) stopAnimation;


@end

