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


#include "../../include/recore/RECoreC.h"
#include "../../include/recore/REFileManager.h"
#include "../../include/recore/REFloat16.h"
#include "../../include/recore/RELog.h"
#include "../../include/recore/RECoreVersion.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

class RECorePrivate
{
	
public:
	static REString preferencesPath;
	static REBOOL isPreferencesPathInitialized;
#ifdef __RE_OS_ANDROID__
	static JNIEnv * androidJNIEnvironment;
	static jclass androidApplicationActivityJNIClass;
#endif
};

REString RECorePrivate::preferencesPath;
REBOOL RECorePrivate::isPreferencesPathInitialized = false;

#ifdef __RE_OS_ANDROID__
JNIEnv * RECorePrivate::androidJNIEnvironment = (JNIEnv *)0;
jclass RECorePrivate::androidApplicationActivityJNIClass = (jclass)0;
#endif

int RECore::isCorrectTypes()
{
	if (sizeof(REByte) != sizeof(REUByte)) 
	{
		RELog::log("ERROR: sizes of REByte and REUByte is different.");
		return 0;
	}

	if (sizeof(REUByte) != 1) 
	{
		RELog::log("ERROR: size of REUByte is not 1.");
		return 0;
	}
	
	if (sizeof(REInt16) != sizeof(REUInt16)) 
	{
		RELog::log("ERROR: sizes of REInt16 and REUInt16 is different.");
		return 0;
	}
	
	if (sizeof(REUInt16) != 2)
	{
		RELog::log("ERROR: size of REUByte is not 2.");
		return 0;
	}
	
	if (sizeof(REInt32) != sizeof(REUInt32)) 
	{
		RELog::log("ERROR: sizes of REInt32 and REUInt32 is different.");
		return 0;
	}
	
	if (sizeof(REUInt32) != 4)
	{
		RELog::log("ERROR: size of REUInt32 is not 4.");
		return 0;
	}
	
	if (sizeof(REInt64) != sizeof(REUInt64))
	{
		RELog::log("ERROR: sizes of REInt64 and REUInt64 is different.");
		return 0;
	}
	
	if (sizeof(REUInt64) != 8) 
	{
		RELog::log("ERROR: size of REUInt64 is not 8.");
		return 0;
	}
	
	if (sizeof(REFloat32) != 4)
	{
		RELog::log("ERROR: size of REFloat32 is not 4.");
		return 0;
	}
	
	if (sizeof(REFloat64) != 8)
	{
		RELog::log("ERROR: size of REFloat64 is not 8.");
		return 0;
	}
	
	if (sizeof(void*) != sizeof(REUIdentifier)) 
	{
		RELog::log("ERROR: sizes of pointer type and REUIdentifier is different.");
		return 0;
	}
	
	return 1;
}


#define XSTR(s) STR(s)
#define STR(s) #s

#define RECORE_VERSION_STRING XSTR(RECORE_VERSION_MAJOR) "." XSTR(RECORE_VERSION_MINOR) "." XSTR(RECORE_VERSION_RELEASE) " #" XSTR(RECORE_BUILD_NUMBER)


const char * RECore::buildInfo()
{ 
	static const char * info = 
	"\nRECore version: " 
	
	RECORE_VERSION_STRING 
	
	"\n"
	"Build date: " 
	
#if defined(__DATE__)	
	__DATE__
#else
	"unknown"
#endif
	
	" time: " 
	
#if defined(__TIME__)	
	__TIME__
#else
	"unknown"
#endif	
	"\n"
	
#if defined(__RE_ARCHITECTURE_ARM__)	
	"Architecture: arm \n"
#endif
	
#if defined(__RE_64BIT_PLATFORM__)	
	"Platform: 64 bit\n"
#elif defined(__RE_32BIT_PLATFORM__)
	"Platform: 32 bit\n"
#else
	"Platform: undefined\n"
#endif

	"\n";
	return info;
}

const REString & RECore::getPreferencesPath()
{
	return RECorePrivate::preferencesPath;
}

REBOOL RECore::setPreferencesPath(const REString & newPrefPath)
{
	REFileManager fManager;
	REBOOL isDir = false;
	if (fManager.isFileExistsAtPath(newPrefPath, &isDir)) 
	{
		if (isDir) 
		{
			if (fManager.isWritableFileAtPath(newPrefPath))
			{
				RECorePrivate::preferencesPath = newPrefPath;
				RECorePrivate::isPreferencesPathInitialized = true;
				return true;
			}
		}
	}
	return false;
}

#ifdef __RE_OS_ANDROID__
void RECore::setAndroidJNIEnvironment(JNIEnv * jniEnvironment)
{
	RECorePrivate::androidJNIEnvironment = jniEnvironment;
}

JNIEnv * RECore::getAndroidJNIEnvironment()
{
	return RECorePrivate::androidJNIEnvironment;
}

void RECore::setAndroidApplicationActivityJNIClass(jclass jniClass)
{
	RECorePrivate::androidApplicationActivityJNIClass = jniClass;
}

jclass RECore::getAndroidApplicationActivityJNIClass()
{
	return RECorePrivate::androidApplicationActivityJNIClass;
}
#endif /* __RE_OS_ANDROID__ */



