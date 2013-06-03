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


#ifndef __RECONFIGMACOSX_H__
#define __RECONFIGMACOSX_H__
#ifdef __RE_OS_MACOSX__


// Core.
/// Version string
#define __RE_CORE_VERSION_STRING__ "0.1.0"

/// Using static callbacks for reading and checking existance of application resources
#define __RE_USING_STATIC_CALLBACKS_FOR_RESOURCES_STORAGE__

/// Using posix threads in REThread object.
#define __RE_USING_PTHREADS__

/// Using posix memalign for alocating memory. Used in REMem "posix_memalign"
#define __RE_USING_POSIX_MEMALIGN__

/// Disables zlib compression/decompression for REZLIBCompression. Requires zlib library.
//#define __RE_RECORE_NO_ZLIB_COMPRESSION_SUPPORT__

/// Disables lzma compression/decompression for RELZMACompression. Requires lzma library.
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

/// Using zlib library from addlibs folder. Will include as "addlibs/zlib.h". All zlib wrappers must be compiled.
#define __RE_USING_ADITIONAL_ZLIB_LIBRARY__

/// Using system provided zlib library. Will include as <zlib.h>
//#define __RE_USING_SYSTEM_ZLIB_LIBRARY__

/// Using png library from addlibs folder. Will include as "addlibs/png.h". All png wrappers must be compiled.
#define __RE_USING_ADITIONAL_PNG_LIBRARY__

/// Using system provided png library. Will include as <png.h>
//#define __RE_USING_SYSTEM_PNG_LIBRARY__

/// Using jpeg library from addlibs folder. Will include as "addlibs/jpeg.h". All jpeg wrappers must be compiled.
#define __RE_USING_ADITIONAL_JPEG_LIBRARY__

/// Using system provided jpeg library. Will include as <jpeglib.h>
//#define __RE_USING_SYSTEM_JPEG_LIBRARY__

/// Using openjpeg library from addlibs folder. Will include as "addlibs/openjpeg.h". All openjpeg wrappers must be compiled.
#define __RE_USING_ADITIONAL_OPENJPEG_LIBRARY__

/// Using system provided openjpeg library. Will include as <openjpeg.h>
//#define __RE_USING_SYSTEM_OPENJPEG_LIBRARY__

/// Using webp library from addlibs folder. Will include as "addlibs/webp.h". All webp wrappers must be compiled.
#define __RE_USING_ADITIONAL_WEBP_LIBRARY__

/// Using system provided webp library. Will include as <webp/types.h>, <webp/mux.h>, <webp/decode.h> and <webp/encode.h>
//#define __RE_USING_SYSTEM_WEBP_LIBRARY__

/// Using sqlite library from addlibs folder. Will include as "addlibs/sqlite.h". All sqlite wrappers must be compiled.
//#define __RE_USING_ADITIONAL_SQLITE_LIBRARY__

/// Using system provided sqlite library. Will include as <sqlite3.h>
#define __RE_USING_SYSTEM_SQLITE_LIBRARY__

/// Disables reading/writing to sqlite database. Requires sqlite3 library. Classes RESQLite... will do nothing.
//#define __RE_CORE_NO_SQLITE_DATABASE_SUPPORT__

/// Using lzma library from addlibs folder. Will include as "addlibs/lzma.h". All lzma wrappers must be compiled.
#define __RE_USING_ADITIONAL_LZMA_LIBRARY__

/// Using system provided lzma library. Will include as <LzmaLib.h>
//#define __RE_USING_SYSTEM_LZMA_LIBRARY__

/// Using FreeType library from addlibs folder. Will include as "addlibs/freetype.h". All freetype wrappers must be compiled.
#define __RE_USING_ADITIONAL_FREETYPE_LIBRARY__

/// Using system provided FreeType library. Will include as <ft2build.h> and <freetype/freetype.h>
//#define __RE_USING_SYSTEM_FREETYPE_LIBRARY__

/// Using TinyXML library from addlibs folder. Will include as "addlibs/tinyxml.h". All TinyXML wrappers must be compiled.
#define __RE_USING_ADITIONAL_TINYXML_LIBRARY__

/// Using system provided TinyXML library. Will include as <tinyxml.h>
//#define __RE_USING_SYSTEM_TINYXML_LIBRARY__

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

/// RECore objects can be initialzed from url.
//#define __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__

/// Using exception handling in a C++.
#define __RE_USING_EXCEPTIONS__

// Rendering. Using for GUI library in Render device, textures etc.
/// Render device use DirectX9. Using for desktop.
//#define __RE_USING_DIRECTX9__

/// Render device use OpenGL. Using for desktop.
#define __RE_USING_OPENGL__

/// Render device use OpenGL ES. Using for mobile devices.
//#define __RE_USING_OPENGL_ES__

/// Mark version OpenGL ES as 1.0/1.1. If not selected 1.0/1.1 of 2.0 checker will select 1.0/1.1 version.
//#define __RE_USING_OPENGL_ES_1__

/// Mark version OpenGL ES as 2.0. If not selected 1.0/1.1 of 2.0 checker will select 1.0/1.1 version.
//#define __RE_USING_OPENGL_ES_2__

/// Using Pyro Particles library from addlibs folder. Will include as "addlibs/pyroparticles.h". All pyro wrappers must be compiled.
#define __RE_USING_ADITIONAL_PYRO_PARTICLES_LIBRARY__

/// Using system provided Pyro Particles library. Will include as <Pyro.h> and <PyroD3D9.h> or <PyroOpenGL.h> or <PyroOpenGLES.h>
//#define __RE_USING_SYSTEM_PYRO_PARTICLES_LIBRARY__


// Sound and music.
/// Using OpenAL library for sound output.
#define __RE_USING_OPENAL__

/// Using ogg library from addlibs folder. Will include as "addlibs/ogg.h". All ogg wrappers must be compiled.
#define __RE_USING_ADITIONAL_OGG_LIBRARY__

/// Using system provided ogg library. Will include as <ogg/ogg.h>
//#define __RE_USING_SYSTEM_OGG_LIBRARY__

/// Using vorbis library from addlibs folder. Will include as "addlibs/vorbis.h". All vorbis wrappers must be compiled.
#define __RE_USING_ADITIONAL_VORBIS_LIBRARY__

/// Using system provided vorbis library. Will include as <vorbis/codec.h> and <vorbis/vorbisfile.h>
//#define __RE_USING_SYSTEM_VORBIS_LIBRARY__

/// Using theora library from addlibs folder. Will include as "addlibs/theora.h". All theora wrappers must be compiled.
#define __RE_USING_ADITIONAL_THEORA_LIBRARY__

/// Using system provided theora library. Will include as <theora/theoradec.h>
//#define __RE_USING_SYSTEM_THEORA_LIBRARY__

/// Using tremor library from addlibs folder. Will include as "addlibs/tremor.h". All tremor wrappers must be compiled.
//#define __RE_USING_ADITIONAL_TREMOR_LIBRARY__

/// Using system provided tremor library. Will include as <tremor/ivorbiscodec.h> and <tremor/ivorbisfile.h>
//#define __RE_USING_SYSTEM_TREMOR_LIBRARY__

/// Using vorbis library for handling vorbis stream
#define __RE_USING_VORBIS_LIBRARY__

/// Using thremor library for handling vorbis stream
//#define __RE_USING_TREMOR_LIBRARY__


// Common
/// Mean target is desktop hardware. PC or Mac.
//#define __RE_OS_DESKTOP__

/// This config will not checked.
//#define __RE_DISABLE_CONFIG_CHECKER__




#endif /* __RE_OS_MACOSX__ */
#endif /* __RECONFIGMACOSX_H__ */


