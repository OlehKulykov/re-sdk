

#import "MyWindow.h"

@implementation MyWindow

- (BOOL) IsFullScreenMode 
{	
    return (self.styleMask & NSFullScreenWindowMask) ? YES : NO;
}

- (void) setStyleMask:(NSUInteger)styleMask 
{
    [super setStyleMask:styleMask];
}

- (void) EnterFullScreen
{	
    NSWindowCollectionBehavior behavior = [self collectionBehavior];
    behavior |= NSWindowCollectionBehaviorFullScreenPrimary;
    [self setCollectionBehavior:behavior];
	[self toggleFullScreen:self];
}

- (void) ExitFullScreen
{	
    NSWindowCollectionBehavior behavior = [self collectionBehavior];
    behavior ^= NSWindowCollectionBehaviorFullScreenPrimary;
    [self setCollectionBehavior:behavior];
	[self toggleFullScreen:self];
}

@end
