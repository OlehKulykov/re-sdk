/*
 *   Copyright 2012 Kulikov Oleg
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


#ifndef __RECONFIGBADA_H__
#define __RECONFIGBADA_H__
#ifdef __RE_OS_BADA__


// Core.
/// Using posix threads in REThread object.
//#define __RE_USING_PTHREADS__

/// Using posix memalign for alocating memory. Used in REMem "posix_memalign"
//#define __RE_USING_POSIX_MEMALIGN__

/// Disables zlib compression/decompression from REZLIBCompression. Requires zlib library.
//#define __RE_RECORE_NO_ZLIB_COMPRESSION_SUPPORT__

/// Disables lzma compression/decompression from RELZMACompression. Requires lzma library.
//#define __RE_RECORE_NO_LZMA_COMPRESSION_SUPPORT__

/// Disables reading(decoding)/writing(encode) PNG image format. Requires png library.
//#define __RE_RECORE_NO_PNG_IMAGE_SUPPORT__

/// Disables reading(decoding) JPEG image format. Requires jpeg library.
//#define __RE_RECORE_NO_JPEG_IMAGE_SUPPORT__

/// Disables reading(decoding) JPEG2000 image format. Requires openjpeg library.
//#define __RE_RECORE_NO_JPEG2000_IMAGE_SUPPORT__

/// Disables reading(decoding)/writing(encode) WEBP image format. Requires webp library.
//#define __RE_RECORE_NO_WEBP_IMAGE_SUPPORT__

/// Disables REFloat16 as 16 bit float, in this case type REFloat16 will be defined as REFloat32 (32 bit float) for compability.
/// FLOAT16_MAX will be FLT_MAX and FLOAT16_MIN will be FLT_MIN
//#define __RE_RECORE_NO_FLOAT16_SUPPORT__

/// Disables yuv to rgb/a color conversion. Class REYUVtoRGB will do nothing.
//#define __RE_CORE_NO_YUV_TO_RGB_CONVERSION__

/// Using zlib library from addlibs folder. Will include as "addlibs/zlib.h". All zlib wrapers must be compiled.
#define __RE_USING_ADITIONAL_ZLIB_LIBRARY__

/// Using system provided zlib library. Will include as <zlib.h>
//#define __RE_USING_SYSTEM_ZLIB_LIBRARY__

/// Using png library from addlibs folder. Will include as "addlibs/png.h". All png wrapers must be compiled.
#define __RE_USING_ADITIONAL_PNG_LIBRARY__

/// Using system provided png library. Will include as <png.h>
//#define __RE_USING_SYSTEM_PNG_LIBRARY__

/// Using sqlite library from addlibs folder. Will include as "addlibs/sqlite.h". All sqlite wrapers must be compiled.
#define __RE_USING_ADITIONAL_SQLITE_LIBRARY__

/// Using system provided png library. Will include as <sqlite3.h>
//#define __RE_USING_SYSTEM_SQLITE_LIBRARY__

/// Disables reading/writing to sqlite database. Requires sqlite3 library. Classes RESQLite... will do nothing.
//#define __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__

/// Removes from REString functionality witch converts special symbols to it's html presentation
/// and back
/// #define __RE_RECORE_NO_STRING_DECODE_ENCODE_SPECIAL_HTML_CHARACTERS__

/// Mean holding url logical parts positions and length will be stored using 16bit unsigned integer (REUInt16)
/// Othervise using 32bit unsigned integer (REUInt32). This will reduce memory size for each REURL object.
#define __RE_USING_UINT16_FOR_URL_MAX_LENGTH__

/// Using curl library from addlibs folder.
/// Will include as "addlibs/curl.h". All curl wrapers must be compiled.
//#define __RE_USING_ADITIONAL_CURL_LIBRARY__

/// Using system provided curl library. Will include as <curl/curl.h>
//#define __RE_USING_SYSTEM_CURL_LIBRARY__

///
//#define __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__

/// Using exception handling in a C++.
//#define __RE_USING_EXCEPTIONS__ 

// Rendering. Using for GUI library in Render device, textures etc.
/// Render device use DirectX9. Using for desktop.
//#define __RE_USING_DIRECTX9__

/// Render device use OpenGL. Using for desktop.
//#define __RE_USING_OPENGL__

/// Render device use OpenGL ES. Using for mobile devices.
#define __RE_USING_OPENGL_ES__

/// Mark version OpenGL ES as 1.0/1.1. If not selected 1.0/1.1 of 2.0 checker will select 1.0/1.1 version.
#define __RE_USING_OPENGL_ES_1__

/// Mark version OpenGL ES as 2.0. If not selected 1.0/1.1 of 2.0 checker will select 1.0/1.1 version.
//#define __RE_USING_OPENGL_ES_2__

/// Using Pyro particles.
//#define __RE_USING_PYRO_PARTICLES__


// Sound and music.
/// Using OpenAL library for sound output.
#define __RE_USING_OPENAL__

//#define __RE_USING_SYSTEM_OGG_LIBRARY__
#define __RE_USING_ADITIONAL_OGG_LIBRARY__

//#define __RE_USING_SYSTEM_VORBIS_LIBRARY__
#define __RE_USING_ADITIONAL_VORBIS_LIBRARY__

//#define __RE_USING_SYSTEM_THEORA_LIBRARY__
#define __RE_USING_ADITIONAL_THEORA_LIBRARY__


//#define __RE_USING_TREMOR_LIBRARY__
#define __RE_USING_VORBIS_LIBRARY__

// Common
/// Mean target is desktop hardware. PC or Mac.
//#define __RE_OS_DESKTOP__

/// This config will not checked.
//#define __RE_DISABLE_CONFIG_CHECKER__





#endif /* __RE_OS_BADA__ */
#endif /* __RECONFIGBADA_H__ */


