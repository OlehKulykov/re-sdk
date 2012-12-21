//
//  iOSAppIntrlayer.h
//  SimpleViewController
//
//  Created by Resident Evil on 21.12.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>

@interface iOSAppInterlayer : NSObject <GLKViewDelegate, GLKViewControllerDelegate>

- (void) onSetupGL;

- (void) updateScreenFrame:(CGRect)frame;

+ (iOSAppInterlayer *) defaultInterlayer;

@end
