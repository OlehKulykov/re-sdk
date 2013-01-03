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


#include "../../include/remedia/private/REAudioPlayerPrivate_iphone.h"

#ifdef __RE_OS_IPHONE__

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <AVFoundation/AVAudioPlayer.h>
#endif

REBOOL REAudioPlayerPrivate_iphone::Play()
{
#ifdef __OBJC__
	if (_p) 
	{
		AVAudioPlayer * player = (AVAudioPlayer *)_p;
		if ([player play])
		{
			return true;
		}
	}
#endif
	return false;
}

REBOOL REAudioPlayerPrivate_iphone::IsPlaying() const
{
#ifdef __OBJC__
	if (_p) 
	{
		AVAudioPlayer * player = (AVAudioPlayer *)_p;
		if ( [player isPlaying] )
		{
			return true;
		}
	}
#endif
	return false;
}

REBOOL REAudioPlayerPrivate_iphone::Pause()
{
#ifdef __OBJC__
	if (_p) 
	{
		AVAudioPlayer * player = (AVAudioPlayer *)_p;
		[player pause];
		return true;
	}
#endif
	return false;
}

REBOOL REAudioPlayerPrivate_iphone::Stop()
{
#ifdef __OBJC__
	if (_p) 
	{
		AVAudioPlayer * player = (AVAudioPlayer *)_p;
		[player stop];
		return true;
	}
#endif
	return false;
}

REBOOL REAudioPlayerPrivate_iphone::SetLooped(const REBOOL isLooped)
{
#ifdef __OBJC__
	if (_p) 
	{
		AVAudioPlayer * player = (AVAudioPlayer *)_p;
		[player setNumberOfLoops: isLooped ? -1 : 0];
		return true;
	}
#endif
	return false;
}

REBOOL REAudioPlayerPrivate_iphone::IsLooped() const /* by default is not looped */
{
#ifdef __OBJC__
	if (_p) 
	{
		AVAudioPlayer * player = (AVAudioPlayer *)_p;
		NSInteger loops = [player numberOfLoops];
		if (loops < 0) 
		{
			return true;
		}
		return false;
	}
#endif	
	return false;
}

REBOOL REAudioPlayerPrivate_iphone::SetVolume(const REFloat32 newVolume)
{
#ifdef __OBJC__
	if (_p) 
	{
		AVAudioPlayer * player = (AVAudioPlayer *)_p;
		[player setVolume:(float)newVolume];
		return true;
	}
#endif
	return false;
}

const REFloat32 REAudioPlayerPrivate_iphone::GetVolume() const
{
#ifdef __OBJC__
	if (_p) 
	{
		AVAudioPlayer * player = (AVAudioPlayer *)_p;
		return (REFloat32)[player volume];		
	}
#endif
	return 0.0f;
}

REBOOL REAudioPlayerPrivate_iphone::InitWithData(const REData & soundFileData)
{
	_soundFileData.Clear();
	
#ifdef __OBJC__
	if (_p) 
	{
		AVAudioPlayer * player = (AVAudioPlayer *)_p;
		[player release];
		_p = NULL;
	}
#endif
	
	if ( soundFileData.IsEmpty() ) 
	{
		return false;
	}
	
	_soundFileData = soundFileData;
	
#ifdef __OBJC__
	NSData * fileData = [NSData dataWithBytesNoCopy:(void *)_soundFileData.GetBytes()
											 length:(NSUInteger)_soundFileData.GetSize()
									   freeWhenDone:NO];
	if (fileData)
	{
		AVAudioPlayer * player = [[AVAudioPlayer alloc] initWithData:fileData error:nil];
		if (player) 
		{
			[player prepareToPlay];
			_p = player;
			return true;
		}
	}
#endif	
	
	return false;
}

REBOOL REAudioPlayerPrivate_iphone::InitFromFilePath(const REString & filePath)
{
	_soundFileData.Clear();
	
	if (filePath.IsEmpty()) 
	{
		return false;
	}
	
#ifdef __OBJC__
	if (_p) 
	{
		AVAudioPlayer * player = (AVAudioPlayer *)_p;
		[player release];
		_p = NULL;
	}
	
	NSString * fileOBJString = [NSString stringWithUTF8String:filePath.UTF8String()];
	if (fileOBJString) 
	{
		NSFileManager * manager = [[NSFileManager alloc] init];
		if ([manager fileExistsAtPath:fileOBJString]) 
		{
			NSURL * url = [NSURL fileURLWithPath:fileOBJString];
			if (url) 
			{
				AVAudioPlayer * player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:nil];
				if (player) 
				{
					[manager release];
					[player prepareToPlay];
					_p = player;
					return true;
				}
			}
		}
		else
		{
			REData data;
			if (data.InitFromPath(filePath)) 
			{
				[manager release];
				return this->InitWithData(data);
			}
		}
		[manager release];
	}
	
#endif
	return false;
}

REAudioPlayerPrivate_iphone::REAudioPlayerPrivate_iphone() : REObject(),
	_p(NULL)
{
	
}

REAudioPlayerPrivate_iphone::~REAudioPlayerPrivate_iphone()
{
#ifdef __OBJC__
	if (_p) 
	{
		AVAudioPlayer * player = (AVAudioPlayer *)_p;
		[player stop];
		[player release];
		_p = NULL;
	}
#endif
}


#endif /* __RE_OS_IPHONE__ */


