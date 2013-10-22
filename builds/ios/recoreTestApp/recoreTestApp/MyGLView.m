//
//  MyGLView.m
//  recoreTestApp
//
//  Created by Resident Evil on 16.11.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#import "MyGLView.h"
#import "MyGameGLKDelegate.h"

@interface MyGLView()

- (void) onFrameChanged;
- (CGPoint) pointOfTouch:(UITouch *)touch;

@end

@implementation MyGLView

- (CGPoint) pointOfTouch:(UITouch *)touch
{
	const CGFloat scaleFactor = self.contentScaleFactor;

	CGPoint point = [touch locationInView:self];
	
	point.x *= scaleFactor;
	point.y *= scaleFactor;
	
	return point;
}

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch * touch = [touches anyObject];
	if ([touches count] == 1)
	{
		const CGPoint point = [self pointOfTouch:touch];
		
		[[MyGameGLKDelegate defaultDelegate] touchOnScreenBeganAtCoordX:point.x
																   andY:point.y];
	}
}

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch * touch = [touches anyObject];
	if ([touches count] == 1)
	{
		const CGPoint point = [self pointOfTouch:touch];
		
		[[MyGameGLKDelegate defaultDelegate] touchOnScreenMovedAtCoordX:point.x
																   andY:point.y];
	}
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch * touch = [touches anyObject];
	if ([touches count] == 1)
	{
		const CGPoint point = [self pointOfTouch:touch];
		
		[[MyGameGLKDelegate defaultDelegate] touchOnScreenEndedAtCoordX:point.x
																   andY:point.y];
	}
}

- (void) onFrameChanged
{
	CGRect screenFrame = [self bounds];
	
	const CGFloat scaleFactor = self.contentScaleFactor;
	
	screenFrame.size.width *= scaleFactor;
	screenFrame.size.height *= scaleFactor;
	
	[[MyGameGLKDelegate defaultDelegate] updateScreenFrame:screenFrame];
}

- (void) setFrame:(CGRect)frame
{
	[super setFrame:frame];
	
	[self onFrameChanged];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
