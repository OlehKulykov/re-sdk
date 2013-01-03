/*
 *   Copyright 2012 - 2013 Kulikov Oleg
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */


#import "MyGLView.h"
#import "iOSAppInterlayer.h"

@interface MyGLView()

- (void) onFrameChanged;

@end

@implementation MyGLView

- (void) onFrameChanged
{
	CGRect screenFrame = [self bounds];
	CGFloat scaleFactor = self.contentScaleFactor;
	
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
