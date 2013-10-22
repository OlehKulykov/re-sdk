//
//  IOSFileReaderCallBacks.h
//  recoreTestApp
//
//  Created by Resident Evil on 24.11.12.
//  Copyright (c) 2012 n. All rights reserved.
//

class IOSFileReaderCallBacks
{
private:
	static NSString * GetFullPath(const char * filePath);
	static REBOOL ReadFullPathToBuffer(NSString * fullPath, REBuffer * destinationBuffer);
public:
	static REBOOL ReadFileCallBack(const char * filePath, REBuffer * destinationBuffer);
	static REBOOL IsFileExistsCallBack(const char * filePath);
};

