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


#include "../../include/recore/RELog.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

#if defined(HAVE_ANDROID_LOG_H)
#include <android/log.h>
#define LOG_TAG "RECore"
#endif

void RELog::log(const char * logString, ...)
{
	if (logString)
	{
		va_list args;
		va_start(args, logString);
		
#if defined(HAVE_ANDROID_LOG_H)		
		__android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, logString, args);
#else		
		RELog::logA(logString, args);
#endif		
		
		va_end(args);
	}
}

#if !defined(__RE_OS_IPHONE__) && !defined(__RE_OS_MACOSX__)
void RELog::logA(const char * logString, va_list arguments)
{
	if (logString)
	{
#if defined(HAVE_ANDROID_LOG_H)				
		__android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, logString, arguments);
#else		
		printf("\n");
		vprintf(logString, arguments);
#endif		
	}
}
#endif


