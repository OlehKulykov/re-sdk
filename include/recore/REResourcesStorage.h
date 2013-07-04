/*
 *   Copyright 2012 - 2013 Kulykov Oleh
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


#ifndef __RERESOURCESSTORAGE_H__
#define __RERESOURCESSTORAGE_H__

#include "RECommonHeader.h"
#include "REBuffer.h"
#include "REString.h"
#include "REArray.h"
#include "REMutex.h"


/// Class user for managing applicaition resources.
/// REData use this class for initializing
class __RE_PUBLIC_CLASS_API__ REResourcesStorage
{
public:
#ifdef __RE_USING_STATIC_CALLBACKS_FOR_RESOURCES_STORAGE__	
	/// Setting callback for reading resource to buffer
	/// Note: use this method if __RE_USING_STATIC_CALLBACKS_FOR_RESOURCES_STORAGE__ is defined
	void setReadToBufferCallBack(REBOOL (*ReadToBuf)(const char *, REBuffer *));
	
	/// Setting callback for checking is resource exists 
	/// Note: use this method if __RE_USING_STATIC_CALLBACKS_FOR_RESOURCES_STORAGE__ is defined
	void setIsExistsCallBack(REBOOL (*IsExists)(const char *));
#endif	

	/// Adding resource container. 
	/// User may add path to zip archive or path to directory.
	REBOOL addResourcesPath(const REString & resourcesPath);
	
	/// Reading from avaiable resources first ocurence of resourcePath to buffer object.
	/// 'resourcePath' will be looked at allready added containers(zip's or directories).
	REBOOL readToBuffer(const REString & resourcePath, REBuffer * toBuffer);
	
	/// Check is resource with path exists
	REBOOL isExists(const REString & resourcePath);
	
	/// Removes all added resources.
	void clear();
	
	REResourcesStorage();
	~REResourcesStorage();
};


#endif /* __RERESOURCESSTORAGE_H__ */

