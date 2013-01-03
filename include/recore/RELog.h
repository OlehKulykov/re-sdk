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


#ifndef __RELOG_H__
#define __RELOG_H__

#include "RECommonHeader.h"
#include "REString.h"
#include <stdarg.h>

/// Class using for logining text messages.
class __RE_PUBLIC_CLASS_API__ RELog
{
public:
	/// Log message with arguments.
	static void Log(const char * logString, ...);
	
	/// Log message with arguments.
	static void LogA(const char * logString, va_list arguments);
};


#endif /* __RELOG_H__ */

