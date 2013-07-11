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


#ifndef __RECOREC_H__
#define __RECOREC_H__

#include "RECommonHeader.h"
#include "REString.h"

#ifdef __RE_OS_ANDROID__
#include <jni.h>
#endif

/// Core class
class __RE_PUBLIC_CLASS_API__ RECore
{
public:
	/// Check types defined correctly.
	/// Return positive value of everything correct, othervice zero
	static int isCorrectTypes();
	
	/// Returns c string with build information
	static const char * buildInfo();
	
#ifdef __RE_OS_ANDROID__
	/// Setting Andoroid JNI Environment
	static void setAndroidJNIEnvironment(JNIEnv * jniEnvironment);
	
	/// Getting Andoroid JNI Environment
	static JNIEnv * getAndroidJNIEnvironment();
	
	/// Setting Android Activity JNI Class
	static void setAndroidApplicationActivityJNIClass(jclass jniClass);
	
	/// Getting Android Activity JNI Class
	static jclass getAndroidApplicationActivityJNIClass();
#endif

	/// Returns string with preferences path
	static const REString & getPreferencesPath();
	
	/// Setting custom preferences path
	static REBOOL setPreferencesPath(const REString & newPrefPath);
};



#endif /* __RECOREC_H__ */



