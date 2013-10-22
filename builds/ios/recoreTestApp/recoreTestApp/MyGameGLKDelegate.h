//
//  MyGameGLKDelegate.h
//  recoreTestApp
//
//  Created by Resident Evil on 16.11.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>


@interface MyGameGLKDelegate : NSObject <GLKViewDelegate, GLKViewControllerDelegate>


- (void) touchOnScreenBeganAtCoordX:(const CGFloat)x andY:(const CGFloat)y;

- (void) touchOnScreenMovedAtCoordX:(const CGFloat)x andY:(const CGFloat)y;

- (void) touchOnScreenEndedAtCoordX:(const CGFloat)x andY:(const CGFloat)y;

- (void) setupGL;

- (void) updateScreenFrame:(const CGRect)frame;

+ (MyGameGLKDelegate *) defaultDelegate;

@end
