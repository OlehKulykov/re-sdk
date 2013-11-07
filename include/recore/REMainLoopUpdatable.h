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


#ifndef __REMAINLOOPUPDATABLE_H__
#define __REMAINLOOPUPDATABLE_H__

#include "RECommonHeader.h"


/// Interface of main loop updatable object.
/// Object that implements this interface will updates from main loop.
/// For adding object to main loop and start update call addToMainLoop();
/// For removing object from main loop call removeFromMainLoop();
class __RE_PUBLIC_CLASS_API__ REMainLoopUpdatable
{
private:
	REUIdentifier _mainLoopUpdatableIdentifier;
	
public:
	/// Called from main thread with current time in seconds.
	virtual void update(const RETimeInterval currentTime) = 0;
	
	const REUIdentifier mainLoopUpdatableIdentifier() const;
	
	REBOOL isInMainLoop() const;
	
	/// Adds object to main loop.
	REBOOL addToMainLoop();
	
	/// Removes object from main loop.
	REBOOL removeFromMainLoop();
	
	REMainLoopUpdatable();
	
	/// Destructor.
	virtual ~REMainLoopUpdatable();
};


#endif /* __REMAINLOOPUPDATABLE_H__ */

