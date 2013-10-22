//
//  IOSFileReaderCallBacks.m
//  recoreTestApp
//
//  Created by Resident Evil on 24.11.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#include "IOSFileReaderCallBacks.h"
#import "AppDelegate.h"

NSString * IOSFileReaderCallBacks::GetFullPath(const char * filePath)
{
	NSString * neededPath = [NSString stringWithUTF8String:filePath];
	if (neededPath)
	{
		return [[NSBundle bundleForClass:[AppDelegate class]] pathForResource:[[neededPath lastPathComponent] stringByDeletingPathExtension]
																			 ofType:[neededPath pathExtension]
																		inDirectory:[neededPath stringByDeletingLastPathComponent]];
	}
	return nil;
}

REBOOL IOSFileReaderCallBacks::ReadFullPathToBuffer(NSString * fullPath, REBuffer * destinationBuffer)
{
	NSError * error = nil;
	NSData * fileData = [NSData dataWithContentsOfFile:fullPath options:NSDataReadingUncached error:&error];
	if (!error && fileData)
	{
		const size_t fileSize = (size_t)[fileData length];
		if (destinationBuffer->resize(fileSize, false))
		{
			memcpy(destinationBuffer->buffer(), [fileData bytes], fileSize);
			return true;
		}
	}
	return false;
}

REBOOL IOSFileReaderCallBacks::ReadFileCallBack(const char * filePath, REBuffer * destinationBuffer)
{
	if (filePath && destinationBuffer)
	{
		NSString * fullPath = IOSFileReaderCallBacks::GetFullPath(filePath);
		if (fullPath)
		{
			return IOSFileReaderCallBacks::ReadFullPathToBuffer(fullPath, destinationBuffer);
		}
	}
	return false;
}

REBOOL IOSFileReaderCallBacks::IsFileExistsCallBack(const char * filePath)
{
	if (filePath)
	{
		NSString * fullPath = IOSFileReaderCallBacks::GetFullPath(filePath);
		return (fullPath != nil);
	}
	return false;
}

