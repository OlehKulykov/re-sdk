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

const char * RECore::buildInfo()
{
	static const char * info =
	"RECore version: "  "\n"
	"Build date: " __DATE__ " time: " __TIME__ "\n"
	"Features: \n"
	"- Static callbacks for reading and checking existance of application resources: "
#ifdef __RE_USING_STATIC_CALLBACKS_FOR_RESOURCES_STORAGE__
	"yes\n"
#else
	"no\n"
#endif
	
	"- Posix threads in REThread object: "
#ifdef __RE_TRY_USE_PTHREADS__
	"yes\n"
#else
	"no\n"
#endif
	
	"- Posix memalign for alocating memory. Used in REMem: "
#ifdef __RE_USING_POSIX_MEMALIGN__
	"yes\n"
#else
	"no\n"
#endif

	"- Zlib compression/decompression for REZLIBCompression: "
#ifdef __RE_RECORE_NO_ZLIB_COMPRESSION_SUPPORT__
	"no\n"
#else
	"yes\n"
#endif
	
	"- LZMA compression/decompression for RELZMACompression: "
#ifdef __RE_RECORE_NO_LZMA_COMPRESSION_SUPPORT__
	"no\n"
#else
	"yes\n"
#endif
	
	"- Reading(decoding)/writing(encode) PNG image format: "
#ifdef __RE_RECORE_NO_PNG_IMAGE_SUPPORT__
	"no\n"
#else
	"yes\n"
#endif
	
	"- Reading(decoding) JPEG image format: "
#ifdef __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__
	"no\n"
#else
	"yes\n"
#endif
		
	"- Reading(decoding)/writing(encode) WEBP image format: "
#ifdef __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__
	"no\n"
#else
	"yes\n"
#endif
	
	"- REFloat16 as 16 bit float: "
#ifdef __RE_RECORE_NO_FLOAT16_SUPPORT__
	"no\n"
#else
	"yes\n"
#endif
	
	"- YUV to RGB/A color conversion: "
#ifdef __RE_CORE_NO_YUV_TO_RGB_CONVERSION__
	"no\n"
#else
	"yes\n"
#endif
	
	"- Reading/writing to sqlite database: "
#ifdef __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__
	"no\n"
#else
	"yes\n"
#endif
	
	"- REString functionality witch converts special symbols to it's html presentation: "
#ifdef __RE_RECORE_NO_STRING_DECODE_ENCODE_SPECIAL_HTML_CHARACTERS__
	"no\n"
#else
	"yes\n"
#endif
	
	"- URL logical parts positions and length will be stored using 16bit unsigned integer: "
#ifdef __RE_USING_UINT16_FOR_URL_MAX_LENGTH__
	"yes\n"
#else
	"no\n"
#endif
	
	"- Exception handling: "
#ifdef __RE_USING_EXCEPTIONS__
	"yes\n"
#else
	"no\n"
#endif
	
	"- Pyro particles: "
#if defined(__RE_USING_ADITIONAL_PYRO_PARTICLES_LIBRARY__) || defined(__RE_USING_SYSTEM_PYRO_PARTICLES_LIBRARY__)
	"yes\n"
#else
	"no\n"
#endif
	
	"- OpenAL support: "
#ifdef __RE_USING_OPENAL__
	"yes\n"
#else
	"no\n"
#endif
	
	"- Render device: "
#if defined(__RE_USING_DIRECTX9__)
	"DirectX 9\n"
#elif defined(__RE_USING_OPENGL__)	
	"OpenGL\n"
#elif defined(__RE_USING_OPENGL_ES__)	
	"OpenGL ES "
	
#if defined(__RE_USING_OPENGL_ES_1__)
	"1\n"
#elif defined(__RE_USING_OPENGL_ES_2__)
	"2\n"
#else
	"none\n"
#endif
	
#else
	"none\n"
#endif
	
	"- RECore objects can be initialzed from url: "
#ifdef __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
	"yes\n"
#else
	"no\n"
#endif
	
#if defined(__RE_USING_VORBIS_LIBRARY__) || defined(__RE_USING_TREMOR_LIBRARY__)
	"- Handling vorbis stream via: "
#if defined(__RE_USING_VORBIS_LIBRARY__)
	"vorbis library\n"
#elif defined(__RE_USING_TREMOR_LIBRARY__)
	"thremor library\n"
#endif
#endif
	
	"Additional third-party libraries:\n"
#if defined(__RE_TRY_USE_SYSTEM_ZLIB_LIBRARY__) || defined(__RE_USING_ADITIONAL_ZLIB_LIBRARY__)
	"- Zlib. http://zlib.net\n"
#endif
	
#if defined(__RE_USING_ADITIONAL_PNG_LIBRARY__) || defined(__RE_TRY_USE_SYSTEM_PNG_LIBRARY__)
	"- Png. http://www.libpng.org\n"
#endif
	
#if defined(__RE_USING_ADITIONAL_SQLITE_LIBRARY__) || defined(__RE_USING_SYSTEM_SQLITE_LIBRARY__)
	"- SQLite. http://www.sqlite.org\n"
#endif
	
#if defined(__RE_USING_ADITIONAL_JPEG_LIBRARY__) || defined(__RE_TRY_USE_SYSTEM_JPEG_LIBRARY__)
	"- Jpeg. http://www.ijg.org\n"
#endif

#if defined(__RE_USING_ADITIONAL_OPENJPEG_LIBRARY__) || defined(__RE_TRY_USE_SYSTEM_OPENJPEG_LIBRARY__)
	"- OpenJPEG. http://www.openjpeg.org\n"
#endif

#if defined(__RE_USING_ADITIONAL_WEBP_LIBRARY__) || defined(__RE_TRY_USE_SYSTEM_WEBP_LIBRARY__)
	"- WebP. https://code.google.com/p/webp\n"
#endif

#if defined(__RE_USING_ADITIONAL_LZMA_LIBRARY__) || defined(__RE_TRY_USE_SYSTEM_LZMA_LIBRARY__)
	"- LZMA. http://www.7-zip.org/sdk.html\n"
#endif
	
#if defined(__RE_USING_ADITIONAL_FREETYPE_LIBRARY__) || defined(__RE_USING_SYSTEM_FREETYPE_LIBRARY__)
	"- FreeType. http://www.freetype.org/index2.html\n"
#endif
	
#if defined(__RE_USING_ADITIONAL_TINYXML_LIBRARY__) || defined(__RE_USING_SYSTEM_TINYXML_LIBRARY__)
	"- TinyXML. http://sourceforge.net/projects/tinyxml\n"
#endif
	
#if defined(__RE_USING_ADITIONAL_CURL_LIBRARY__) || defined(__RE_USING_SYSTEM_CURL_LIBRARY__)
	"- cURL. http://curl.haxx.se\n"
#endif
	
#if defined(__RE_USING_ADITIONAL_OGG_LIBRARY__) || defined(__RE_USING_SYSTEM_OGG_LIBRARY__)
	"- Ogg. http://www.xiph.org\n"
#endif

#if defined(__RE_USING_ADITIONAL_VORBIS_LIBRARY__) || defined(__RE_USING_SYSTEM_VORBIS_LIBRARY__)
	"- Vorbis. http://www.xiph.org\n"
#endif
	
#if defined(__RE_USING_ADITIONAL_THEORA_LIBRARY__) || defined(__RE_USING_SYSTEM_THEORA_LIBRARY__)
	"- Theora. http://www.xiph.org\n"
#endif
	
#if defined(__RE_USING_ADITIONAL_TREMOR_LIBRARY__) || defined(__RE_USING_SYSTEM_TREMOR_LIBRARY__)
	"- Tremor. http://www.xiph.org\n"
#endif
	
#ifndef __RE_CORE_NO_YUV_TO_RGB_CONVERSION__
	"- YUV2RGB. http://www.wss.co.uk/pinknoise/yuv2rgb/index.html\n"
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



