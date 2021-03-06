#
#   Copyright 2012 - 2013 Kulykov Oleh
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#

cmake_minimum_required(VERSION 2.8.12)

project(RECoreSDK)

INCLUDE(CheckLibraryExists)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckIncludeFile)

INCLUDE(CheckTypeSize)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckIncludeFileCXX)

check_include_file("stdlib.h" HAVE_STDLIB_H)
if(HAVE_STDLIB_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES stdlib.h)
endif(HAVE_STDLIB_H)

CHECK_INCLUDE_FILE("inttypes.h" HAVE_INTTYPES_H)
if(HAVE_INTTYPES_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES inttypes.h)
endif(HAVE_INTTYPES_H)

CHECK_INCLUDE_FILE("sys/types.h" HAVE_SYS_TYPES_H)
if(HAVE_SYS_TYPES_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES sys/types.h)
endif(HAVE_SYS_TYPES_H)

CHECK_INCLUDE_FILE("stddef.h" HAVE_STDDEF_H)
if(HAVE_STDDEF_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES stddef.h)
endif(HAVE_STDDEF_H)

CHECK_INCLUDE_FILE("stdbool.h" HAVE_STDBOOL_H)
if(HAVE_STDBOOL_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES stdbool.h)
endif(HAVE_STDBOOL_H)

CHECK_INCLUDE_FILE("stdint.h" HAVE_STDINT_H)
if(HAVE_STDINT_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES stdint.h)
endif(HAVE_STDINT_H)

CHECK_INCLUDE_FILE("stdarg.h" HAVE_STDARG_H)
if(HAVE_STDARG_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES stdarg.h)
endif(HAVE_STDARG_H)

CHECK_INCLUDE_FILE("limits.h" HAVE_LIMITS_H)
if(HAVE_LIMITS_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES limits.h)
endif(HAVE_LIMITS_H)

CHECK_INCLUDE_FILE("float.h" HAVE_FLOAT_H)
if(HAVE_FLOAT_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES float.h)
endif(HAVE_FLOAT_H)

CHECK_INCLUDE_FILE("math.h" HAVE_MATH_H)
if(HAVE_MATH_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES math.h)
endif(HAVE_MATH_H)

CHECK_INCLUDE_FILE("wchar.h" HAVE_WCHAR_H)
if(HAVE_WCHAR_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES wchar.h)
endif(HAVE_WCHAR_H)

CHECK_INCLUDE_FILE("malloc.h" HAVE_MALLOC_H)
if(HAVE_MALLOC_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES malloc.h)
endif(HAVE_MALLOC_H)

CHECK_INCLUDE_FILE("memory.h" HAVE_MEMORY_H)
if(HAVE_MEMORY_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES memory.h)
endif(HAVE_MEMORY_H)

CHECK_INCLUDE_FILE("string.h" HAVE_STRING_H)
if(HAVE_STRING_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES string.h)
endif(HAVE_STRING_H)


CHECK_INCLUDE_FILE("mach/mach_time.h" HAVE_MACH_MACH_TIME_H)
CHECK_INCLUDE_FILE("sys/time.h" HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILE("time.h" HAVE_TIME_H)
CHECK_INCLUDE_FILE("sys/time.h" HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILE("locale.h" HAVE_LOCALE_H)
CHECK_INCLUDE_FILE("sys/stat.h" HAVE_SYS_STAT_H)
if(HAVE_SYS_STAT_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES sys/stat.h)
endif(HAVE_SYS_STAT_H)

CHECK_INCLUDE_FILE("fcntl.h" HAVE_FCNTL_H)
CHECK_INCLUDE_FILE("pthread.h" HAVE_PTHREAD_H)
CHECK_INCLUDE_FILE("sys/resource.h" HAVE_SYS_RESOURCE_H)
CHECK_INCLUDE_FILE("direct.h" HAVE_DIRECT_H)
if(HAVE_DIRECT_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES direct.h)
endif(HAVE_DIRECT_H)

CHECK_INCLUDE_FILE("byteswap.h" HAVE_BYTESWAP_H)
CHECK_INCLUDE_FILE("lua.h" HAVE_LUA_H)
CHECK_INCLUDE_FILE("lualib.h" HAVE_LUA_LIB_H)

CHECK_INCLUDE_FILE("errno.h" HAVE_ERRNO_H)
if(HAVE_ERRNO_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES errno.h)
endif(HAVE_ERRNO_H)

CHECK_INCLUDE_FILE("sys/errno.h" HAVE_SYS_ERRNO_H)
if(HAVE_SYS_ERRNO_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES sys/errno.h)
endif(HAVE_SYS_ERRNO_H)

CHECK_INCLUDE_FILE("io.h" HAVE_IO_H)
if(HAVE_IO_H)
	LIST(APPEND CMAKE_EXTRA_INCLUDE_FILES io.h)
endif(HAVE_IO_H)

CHECK_INCLUDE_FILE("setjmp.h" HAVE_SETJMP_H)
CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD_H)
CHECK_INCLUDE_FILE("zlib.h" HAVE_ZLIB_H)
CHECK_INCLUDE_FILE("bzlib.h" HAVE_BZLIB_H)
CHECK_INCLUDE_FILE("png.h" HAVE_PNG_H)
CHECK_INCLUDE_FILE("jpeglib.h" HAVE_JPEGLIB_H)
CHECK_INCLUDE_FILE("openjpeg/openjpeg.h" HAVE_OPENJPEG_OPENJPEG_H)
CHECK_INCLUDE_FILE("openjpeg.h" HAVE_OPENJPEG)
CHECK_INCLUDE_FILE("webp/types.h" HAVE_WEBP_TYPES_H)
CHECK_INCLUDE_FILE("webp/mux.h" HAVE_WEBP_MUX_H)
CHECK_INCLUDE_FILE("webp/decode.h" HAVE_WEBP_DECODE_H)
CHECK_INCLUDE_FILE("webp/encode.h" HAVE_WEBP_ENCODE_H)
CHECK_INCLUDE_FILE("sqlite3.h" HAVE_SQLITE3_H)
CHECK_INCLUDE_FILE("libpq-fe.h" HAVE_LIBPQ_FE_H)
CHECK_INCLUDE_FILE("postgresql/libpq-fe.h" HAVE_POSTGRESQL_LIBPQ_FE_H)
CHECK_INCLUDE_FILE("ft2build.h" HAVE_FT2BUILD_H)
CHECK_INCLUDE_FILE("freetype/freetype.h" HAVE_FREETYPE_FREETYPE_H)
CHECK_INCLUDE_FILE("freetype2/freetype/freetype.h" HAVE_FREETYPE2_FREETYPE_FREETYPE_H)
CHECK_INCLUDE_FILE("lzma/lzma.h" HAVE_LZMA_LZMA_H)
CHECK_INCLUDE_FILE("lzma.h" HAVE_LZMA_H)
CHECK_INCLUDE_FILE("LzmaLib.h" HAVE_LZMALIB_H)
CHECK_INCLUDE_FILE("vorbis/codec.h" HAVE_VORBIS_CODEC_H)
CHECK_INCLUDE_FILE("vorbis/vorbisfile.h" HAVE_VORBIS_VORBISFILE_H)
CHECK_INCLUDE_FILE("theora/theoradec.h" HAVE_THEORA_THEORADEC_H)
CHECK_INCLUDE_FILE("tremor/ivorbiscodec.h" HAVE_TREMOR_IVORBISCODEC_H)
CHECK_INCLUDE_FILE("tremor/ivorbisfile.h" HAVE_TREMOR_IVORBISFILE_H)
CHECK_INCLUDE_FILE("curl/curl.h" HAVE_CURL_CURL_H)
CHECK_INCLUDE_FILE("ogg/ogg.h" HAVE_OGG_OGG_H)
CHECK_INCLUDE_FILE("jni.h" HAVE_JNI_H)
CHECK_INCLUDE_FILE("android/log.h" HAVE_ANDROID_LOG_H)
CHECK_INCLUDE_FILE("arm_neon.h" HAVE_ARM_NEON_H)
CHECK_INCLUDE_FILE("dispatch/dispatch.h" HAVE_DISPATCH_DISPATCH_H)


# Check OpenGL/ES headers
CHECK_INCLUDE_FILE("gl/gl.h" HAVE_GL_GL_H)
CHECK_INCLUDE_FILE("gl/glu.h" HAVE_GL_GLU_H)
CHECK_INCLUDE_FILE("OpenGL/OpenGL.h" HAVE_OPENGL_OPENGL_H)
CHECK_INCLUDE_FILE("OpenGL/gl.h" HAVE_OPENGL_GL_H)
CHECK_INCLUDE_FILE("OpenGL/glu.h" HAVE_OPENGL_GLU_H)
CHECK_INCLUDE_FILE("OpenGLES/ES1/gl.h" HAVE_OPENGLES_ES1_GL_H)
CHECK_INCLUDE_FILE("OpenGLES/ES1/glext.h" HAVE_OPENGLES_ES1_GLEXT_H)
CHECK_INCLUDE_FILE("OpenGLES/ES2/gl.h" HAVE_OPENGLES_ES2_GL_H)
CHECK_INCLUDE_FILE("OpenGLES/ES2/glext.h" HAVE_OPENGLES_ES2_GLEXT_H)
CHECK_INCLUDE_FILE("GL/gl.h" HAVE__GL_GL_H)
CHECK_INCLUDE_FILE("GL/glu.h" HAVE__GL_GLU_H)
CHECK_INCLUDE_FILE("GL/glut.h" HAVE__GL_GLUT_H)
CHECK_INCLUDE_FILE("GLES/gl.h" HAVE_GLES_GL_H)
CHECK_INCLUDE_FILE("GLES/glext.h" HAVE_GLES_GLEXT_H)
CHECK_INCLUDE_FILE("gl.h" HAVE_GL_H)
CHECK_INCLUDE_FILE("glext.h" HAVE_GLEXT_H)
CHECK_INCLUDE_FILE("glmacro.h" HAVE_GLMACRO_H)

MESSAGE(CMAKE_EXTRA_INCLUDE_FILES ": " ${CMAKE_EXTRA_INCLUDE_FILES})

# To check the size of a primitive type:
CHECK_TYPE_SIZE("void*" SIZEOF_VOIDP_T)
IF(SIZEOF_VOIDP_T AND (NOT VOIDP_TYPE))
	SET(VOIDP_TYPE void*)
ENDIF()

CHECK_TYPE_SIZE("int8_t" SIZEOF_INT8_T)
IF(SIZEOF_INT8_T AND (NOT INT8_TYPE))
	SET(INT8_TYPE int8_t)
ENDIF()

CHECK_TYPE_SIZE("uint8_t" SIZEOF_UINT8_T)
IF(SIZEOF_UINT8_T AND (NOT UINT8_TYPE))
	SET(UINT8_TYPE uint8_t)
ENDIF()

CHECK_TYPE_SIZE("u_int8_t" SIZEOF_U_INT8_T)
IF(SIZEOF_U_INT8_T AND (NOT UINT8_TYPE))
	SET(UINT8_TYPE u_int8_t)
ENDIF()

CHECK_TYPE_SIZE("int16_t" SIZEOF_INT16_T)
IF(SIZEOF_INT16_T AND (NOT INT16_TYPE))
	SET(INT16_TYPE int16_t)
ENDIF()

CHECK_TYPE_SIZE("uint16_t" SIZEOF_UINT16_T)
IF(SIZEOF_UINT16_T AND (NOT UINT16_TYPE))
	SET(UINT16_TYPE uint16_t)
ENDIF()

CHECK_TYPE_SIZE("u_int16_t" SIZEOF_U_INT16_T)
IF(SIZEOF_U_INT16_T AND (NOT UINT16_TYPE))
	SET(UINT16_TYPE u_int16_t)
ENDIF()

CHECK_TYPE_SIZE("int32_t" SIZEOF_INT32_T)
IF(SIZEOF_INT32_T AND (NOT INT32_TYPE))
	SET(INT32_TYPE int32_t)
ENDIF()

CHECK_TYPE_SIZE("uint32_t" SIZEOF_UINT32_T)
IF(SIZEOF_UINT32_T AND (NOT UINT32_TYPE))
	SET(UINT32_TYPE uint32_t)
ENDIF()

CHECK_TYPE_SIZE("int64_t" SIZEOF_INT64_T)
IF(SIZEOF_INT64_T AND (NOT INT64_TYPE))
	SET(INT64_TYPE int64_t)
ENDIF()

CHECK_TYPE_SIZE("uint64_t" SIZEOF_UINT64_T)
IF(SIZEOF_UINT64_T AND (NOT UINT64_TYPE))
	SET(UINT64_TYPE uint64_t)
ENDIF()

CHECK_TYPE_SIZE("u_int64_t" SIZEOF_U_INT64_T)
IF(SIZEOF_U_INT64_T AND (NOT UINT64_TYPE))
	SET(UINT64_TYPE u_int64_t)
ENDIF()

CHECK_TYPE_SIZE("uintptr_t" SIZEOF_UINTPTR_T)
IF(SIZEOF_UINTPTR_T AND (NOT UINTPTR_TYPE))
	SET(UINTPTR_TYPE uintptr_t)
ENDIF()


CHECK_TYPE_SIZE("char" SIZEOF_CHAR)
IF(SIZEOF_CHAR AND (NOT INT8_TYPE))
	SET(INT8_TYPE char)
ENDIF()

CHECK_TYPE_SIZE("unsigned char" SIZEOF_UNSIGNED_CHAR)
IF(SIZEOF_UNSIGNED_CHAR AND (NOT UINT8_TYPE))
	SET(UINT8_TYPE "unsigned char")
ENDIF()

CHECK_TYPE_SIZE("short" SIZEOF_SHORT)
IF(SIZEOF_SHORT AND (NOT INT16_TYPE) AND (SIZEOF_SHORT EQUAL 2))
	SET(INT16_TYPE short)
ENDIF()

CHECK_TYPE_SIZE("unsigned short" SIZEOF_UNSIGNED_SHORT)
IF(SIZEOF_UNSIGNED_SHORT AND (NOT UINT16_TYPE) AND (SIZEOF_UNSIGNED_SHORT EQUAL 2))
	SET(UINT16_TYPE "unsigned short")
ENDIF()

CHECK_TYPE_SIZE("int" SIZEOF_INT)
IF(SIZEOF_INT AND (NOT INT32_TYPE) AND (SIZEOF_INT EQUAL 4))
	SET(INT32_TYPE int)
ENDIF()

CHECK_TYPE_SIZE("unsigned int" SIZEOF_UNSIGNED_INT)
IF(SIZEOF_UNSIGNED_INT AND (NOT UINT32_TYPE) AND (SIZEOF_UNSIGNED_INT EQUAL 4))
	SET(UINT32_TYPE "unsigned int")
ENDIF()

CHECK_TYPE_SIZE("long int" SIZEOF_LONG_INT)
IF(SIZEOF_LONG_INT AND (SIZEOF_LONG_INT EQUAL 4) AND (NOT INT32_TYPE))
	SET(INT32_TYPE "long int")
ENDIF()
IF(SIZEOF_LONG_INT AND (SIZEOF_LONG_INT EQUAL 8) AND (NOT INT64_TYPE))
	SET(INT64_TYPE "long int")
ENDIF()


CHECK_TYPE_SIZE("unsigned long int" SIZEOF_UNSIGNED_LONG_INT)
IF(SIZEOF_UNSIGNED_LONG_INT AND (SIZEOF_UNSIGNED_LONG_INT EQUAL 4) AND (NOT UINT32_TYPE))
	SET(UINT32_TYPE "unsigned long int")
ENDIF()
IF(SIZEOF_UNSIGNED_LONG_INT AND (SIZEOF_UNSIGNED_LONG_INT EQUAL 8) AND (NOT UINT64_TYPE))
	SET(UINT64_TYPE "unsigned long int")
ENDIF()

CHECK_TYPE_SIZE("long long int" SIZEOF_LONG_LONG_INT)
IF(SIZEOF_LONG_LONG_INT AND (SIZEOF_LONG_LONG_INT EQUAL 4) AND (NOT INT32_TYPE))
	SET(INT32_TYPE "long long int")
ENDIF()
IF(SIZEOF_LONG_LONG_INT AND (SIZEOF_LONG_LONG_INT EQUAL 8) AND (NOT INT64_TYPE))
	SET(INT64_TYPE "long long int")
ENDIF()

CHECK_TYPE_SIZE("unsigned long long int" SIZEOF_UNSIGNED_LONG_LONG_INT)
IF(SIZEOF_UNSIGNED_LONG_LONG_INT AND (SIZEOF_UNSIGNED_LONG_LONG_INT EQUAL 4) AND (NOT UINT32_TYPE))
	SET(UINT32_TYPE "unsigned long long int")
ENDIF()
IF(SIZEOF_UNSIGNED_LONG_LONG_INT AND (SIZEOF_UNSIGNED_LONG_LONG_INT EQUAL 8) AND (NOT UINT64_TYPE))
	SET(UINT64_TYPE "unsigned long long int")
ENDIF()


CHECK_TYPE_SIZE("float_t" SIZEOF_FLOAT_T)
IF(SIZEOF_FLOAT_T AND (NOT FLOAT32_TYPE) AND (SIZEOF_FLOAT_T EQUAL 4))
	SET(FLOAT32_TYPE float_t)
ENDIF()

CHECK_TYPE_SIZE("double_t" SIZEOF_DOUBLE_T)
IF(SIZEOF_DOUBLE_T AND (NOT FLOAT64_TYPE) AND (SIZEOF_DOUBLE_T EQUAL 8))
	SET(FLOAT32_TYPE double_t)
ENDIF()

CHECK_TYPE_SIZE("long double_t" SIZEOF_LONG_DOUBLE_T)
IF(SIZEOF_LONG_DOUBLE_T AND (NOT FLOAT64_TYPE) AND (SIZEOF_LONG_DOUBLE_T EQUAL 8))
	SET(FLOAT64_TYPE "long double_t")
ENDIF()
IF(SIZEOF_LONG_DOUBLE_T AND (NOT FLOAT128_TYPE) AND (SIZEOF_LONG_DOUBLE_T EQUAL 16))
	SET(FLOAT128_TYPE "long double_t")
	SET(HAVE_FLOAT128 1)
ENDIF()

CHECK_TYPE_SIZE("float" SIZEOF_FLOAT)
IF(SIZEOF_FLOAT AND (NOT FLOAT32_TYPE) AND (SIZEOF_FLOAT EQUAL 4))
	SET(FLOAT32_TYPE float)
ENDIF()

CHECK_TYPE_SIZE("double" SIZEOF_DOUBLE)
IF(SIZEOF_DOUBLE AND (NOT FLOAT64_TYPE) AND (SIZEOF_DOUBLE EQUAL 8))
	SET(FLOAT32_TYPE double)
ENDIF()

CHECK_TYPE_SIZE("long double" SIZEOF_LONG_DOUBLE)
IF(SIZEOF_LONG_DOUBLE AND (NOT FLOAT64_TYPE) AND (SIZEOF_LONG_DOUBLE EQUAL 8))
	SET(FLOAT64_TYPE "long double")
ENDIF()
IF(SIZEOF_LONG_DOUBLE AND (NOT FLOAT128_TYPE) AND (SIZEOF_LONG_DOUBLE EQUAL 16))
	SET(FLOAT128_TYPE "long double")
	SET(HAVE_FLOAT128 1)
ENDIF()

CHECK_TYPE_SIZE("bool" SIZEOF_BOOL)
IF(SIZEOF_BOOL)
	SET(BOOL_TYPE bool)
ENDIF()

CHECK_TYPE_SIZE("__int128_t" SIZEOF___INT128_T)
IF(SIZEOF___INT128_T AND (NOT INT128_TYPE) AND (SIZEOF___INT128_T EQUAL 16))
	SET(INT128_TYPE __int128_t)
ENDIF()
CHECK_TYPE_SIZE("int128_t" SIZEOF_INT128_T)
IF(SIZEOF_INT128_T AND (NOT INT128_TYPE) AND (SIZEOF_INT128_T EQUAL 16))
	SET(INT128_TYPE int128_t)
ENDIF()

CHECK_TYPE_SIZE("__uint128_t" SIZEOF___UINT128_T)
IF(SIZEOF___UINT128_T AND (NOT UINT128_TYPE) AND (SIZEOF___UINT128_T EQUAL 16))
	SET(UINT128_TYPE __uint128_t)
ENDIF()
CHECK_TYPE_SIZE("uint128_t" SIZEOF_UINT128_T)
IF(SIZEOF_UINT128_T AND (NOT UINT128_TYPE) AND (SIZEOF_UINT128_T EQUAL 16))
	SET(UINT128_TYPE uint128_t)
ENDIF()

IF(INT128_TYPE AND UINT128_TYPE)
	SET(HAVE_INTEGERS128 1)
ENDIF()


IF(SIZEOF_VOIDP_T EQUAL 4)
	SET(__RE_32BIT_PLATFORM__ 1)
ENDIF()
IF(SIZEOF_VOIDP_T EQUAL 8)
	SET(__RE_64BIT_PLATFORM__ 1)
ENDIF()

CHECK_TYPE_SIZE("struct stat" SIZEOF_STRUCT_STAT)
IF(SIZEOF_STRUCT_STAT)
	SET(HAVE_STRUCT_STAT 1)
ENDIF()

CHECK_TYPE_SIZE("struct _stat" SIZEOF_STRUCT__STAT)
IF(SIZEOF_STRUCT__STAT)
	SET(HAVE_STRUCT__STAT 1)
ENDIF()


# functions
check_function_exists(posix_memalign HAVE_FUNCTION_POSIX_MEMALIGN)
check_function_exists(usleep HAVE_FUNCTION_USLEEP)
check_function_exists(getrlimit HAVE_FUNCTION_GETRLIMIT)
check_function_exists(pthread_attr_setscope HAVE_FUNCTION_PTHREAD_ATTR_SETSCOPE)
check_function_exists(pthread_attr_setstacksize HAVE_FUNCTION_PTHREAD_ATTR_SETSTACKSIZE)
check_function_exists(pthread_cancel HAVE_FUNCTION_PTHREAD_CANCEL)
check_function_exists(_stat HAVE_FUNCTION__STAT)
check_function_exists(stat HAVE_FUNCTION_STAT)
check_function_exists(_wstat HAVE_FUNCTION__WSTAT)
check_function_exists(access HAVE_FUNCTION_ACCESS)
check_function_exists(_access HAVE_FUNCTION__ACCESS)
check_function_exists(_waccess HAVE_FUNCTION__WACCESS)
check_function_exists(_mkdir HAVE_FUNCTION__MKDIR)
check_function_exists(mkdir HAVE_FUNCTION_MKDIR)
check_function_exists(_wmkdir HAVE_FUNCTION__WMKDIR)
check_function_exists(clock_gettime HAVE_FUNCTION_CLOCK_GETTIME)
check_function_exists(fabsf HAVE_FUNCTION_FABSF)
check_function_exists(fabs HAVE_FUNCTION_FABS)
check_function_exists(floorf HAVE_FUNCTION_FLOORF)
check_function_exists(floor HAVE_FUNCTION_FLOOR)
check_function_exists(powf HAVE_FUNCTION_POWF)
check_function_exists(pow HAVE_FUNCTION_POW)
check_function_exists(cosf HAVE_FUNCTION_COSF)
check_function_exists(cos HAVE_FUNCTION_COS)
check_function_exists(sinf HAVE_FUNCTION_SINF)
check_function_exists(sin HAVE_FUNCTION_SIN)
check_function_exists(sqrtf HAVE_FUNCTION_SQRTF)
check_function_exists(sqrt HAVE_FUNCTION_SQRT)
check_function_exists(_wfopen HAVE_FUNCTION__WFOPEN)
check_function_exists(_wfopen_s HAVE_FUNCTION__WFOPEN_S)
check_function_exists(fopen_s HAVE_FUNCTION_FOPEN_S)
check_function_exists(sscanf_s HAVE_FUNCTION_SSCANF_S)
check_function_exists(sprintf_s HAVE_FUNCTION_SPRINTF_S)
check_function_exists(vsnprintf HAVE_FUNCTION_VSNPRINTF)
check_function_exists(vsprintf_s HAVE_FUNCTION_VSPRINTF_S)
check_function_exists(clock_gettime HAVE_FUNCTION_CLOCK_GETTIME)

check_function_exists(dispatch_sync HAVE_FUNCTION_DISPATCH_SYNC)
check_function_exists(dispatch_async HAVE_FUNCTION_DISPATCH_ASYNC)
check_function_exists(dispatch_get_global_queue HAVE_FUNCTION_DISPATCH_GET_GLOBAL_QUEUE)
check_function_exists(dispatch_after HAVE_FUNCTION_DISPATCH_AFTER)

# OpenGL functions
check_function_exists(gluBuild2DMipmaps HAVE_FUNCTION_GLUBUILD2DMIPMAPS)
check_function_exists(glTexImage2D HAVE_FUNCTION_GLTEXIMAGE2D)
check_function_exists(glGenerateMipmapOES HAVE_FUNCTION_GLGENERATEMIPMAPOES)
check_function_exists(glOrtho HAVE_FUNCTION_GLORTHO)
check_function_exists(glOrthof HAVE_FUNCTION_GLORTHOF)
check_function_exists(glColor3f HAVE_FUNCTION_GLCOLOR3F)
check_function_exists(gluPerspective HAVE_FUNCTION_GLUPERSPECTIVE)


## Check for specific apple platforms
macro(check_apple_platform)
  if(APPLE)
    include(CheckCSourceCompiles)
    check_c_source_compiles("
      #include \"TargetConditionals.h\"
      int main(int argc, char *argv[]) {
      #if !TARGET_OS_MAC
        -1
      #endif
      }
    " OS_OSX)
    check_c_source_compiles("
      #include \"TargetConditionals.h\"
      int main(int argc, char *argv[]) {
      #if !TARGET_OS_IPHONE
        -1
      #endif
      }
    " OS_IPHONE)
  endif()
endmacro() 

check_apple_platform()
if(OS_OSX)
	message("Found OSX")
	set(__RE_OS_MACOSX__ 1)
else()
#	message("Found no OSX")
endif()

if(OS_IPHONE)
	set(__RE_OS_IPHONE__ 1)
	message("Found IPHONE")
else()
#  message("Found no IPHONE")
endif() 

if(WIN32)
    set(__RE_OS_WINDOWS__ 1)
    message("Found Windows")
else()
	
endif()


SET(__RE_SDK_HAVE_CONFIG__ 1)


# Functionality
IF(NO_ZLIB_COMPRESSION)
	SET(__RE_RECORE_NO_ZLIB_COMPRESSION_SUPPORT__ 1)
ENDIF(NO_ZLIB_COMPRESSION)


IF(NO_LZMA_COMPRESSION)
	SET(__RE_RECORE_NO_LZMA_COMPRESSION_SUPPORT__ 1)
ENDIF(NO_LZMA_COMPRESSION)


IF(NO_PNG)
	SET(__RE_RECORE_NO_PNG_IMAGE_SUPPORT__ 1)
ENDIF(NO_PNG)


IF(NO_JPEG)
	SET(__RE_RECORE_NO_JPEG_IMAGE_SUPPORT__ 1)
ENDIF(NO_JPEG)


IF(NO_WEBP)
	SET(__RE_RECORE_NO_WEBP_IMAGE_SUPPORT__ 1)
ENDIF(NO_WEBP)


IF(NO_FLOAT16)
	SET(__RE_RECORE_NO_FLOAT16_SUPPORT__ 1)
ENDIF(NO_FLOAT16)


IF(NO_YUVTORGB)
	SET(__RE_CORE_NO_YUV_TO_RGB_CONVERSION__ 1)
ENDIF(NO_YUVTORGB)


IF(NO_SQLITE)
	SET(__RE_CORE_NO_SQLITE_DATABASE_SUPPORT__ 1)
ENDIF(NO_SQLITE)


IF(NO_POSTGRESQL)
	SET(__RE_CORE_NO_POSTGRESQL_DATABASE_SUPPORT__ 1)
ENDIF(NO_POSTGRESQL)


IF(NO_STRINGHTMLCHARS)
	SET(__RE_RECORE_NO_STRING_DECODE_ENCODE_SPECIAL_HTML_CHARACTERS__ 1)
ENDIF(NO_STRINGHTMLCHARS)


IF(NO_LOADURL)
	SET(__RE_RECORE_NO_LOAD_FROM_URL__ 1)
ENDIF(NO_LOADURL)



configure_file("${PROJECT_SOURCE_DIR}/recore_sdk_config.h.in" 
				"${PROJECT_BINARY_DIR}/recore_sdk_config.h")

# Now make sure that you the the build directory on your "Include" path when compiling
include_directories(${PROJECT_BINARY_DIR})


set(CORE_SRC	src/core/RE2DAlgorithm.cpp
				src/core/REAffineTransform.cpp
				src/core/REAngle.cpp
				src/core/REApplication.cpp
				src/core/REArray.cpp
				src/core/REArrayObject.cpp
				src/core/REAutoReleasePool.cpp
				src/core/REAutoReleasePoolPrivate.cpp
				src/core/REBase64.cpp
				src/core/REBuffer.cpp
				src/core/REBufferNoCopy.cpp
				src/core/REBufferObject.cpp
				src/core/RECRC32Generator.cpp
				src/core/REColor.cpp
				src/core/RECoreC.cpp
				src/core/REData.cpp
				src/core/REDate.cpp
				src/core/REDictionary.cpp
				src/core/REEdgeInsets.cpp
				src/core/REFile.cpp
				src/core/REFileManager.cpp
				src/core/REImage.cpp
				src/core/REImageBase.cpp
				src/core/REImageManager.cpp
				src/core/RELZMACompression.cpp
				src/core/RELocale.cpp
				src/core/RELog.cpp
				src/core/REMD5Generator.cpp
				src/core/REMainLoopUpdatable.cpp
				src/core/REMainLoopsObjectsStoragePrivate.cpp
				src/core/REMath.cpp
				src/core/REMatrix4.cpp
				src/core/REMutableImage.cpp
				src/core/REMutableString.cpp
				src/core/REMutex.cpp
				src/core/RENULL.cpp
				src/core/RENULLObject.cpp
				src/core/RENotificationManager.cpp
				src/core/RENumber.cpp
				src/core/RENumberObject.cpp
				src/core/REObject.cpp
				src/core/REPoint2.cpp
				src/core/REPoint3.cpp
				src/core/REQuaternion.cpp
				src/core/RERandomizer.cpp
				src/core/RERect.cpp
				src/core/REResourcesStorage.cpp
				src/core/RESQLiteDB.cpp
				src/core/RESQLiteDBResultSet.cpp
				src/core/RESerializable.cpp
				src/core/RESize.cpp
				src/core/REStaticString.cpp
				src/core/REString.cpp
				src/core/REStringBase.cpp
				src/core/REStringObject.cpp
				src/core/REStringUtilsPrivate.cpp
				src/core/RETetragon.cpp
				src/core/REThread.cpp
				src/core/RETime.cpp
				src/core/RETimer.cpp
				src/core/RETypedArray.cpp
				src/core/RETypedPtr.cpp
				src/core/REURL.cpp
				src/core/REURLConnection.cpp
				src/core/REURLRequest.cpp
				src/core/REURLResponse.cpp
				src/core/REUserCallBacks.cpp
				src/core/REVector2.cpp
				src/core/REVector3.cpp
				src/core/REWideString.cpp
				src/core/REXMLPropertyListReader.cpp
				src/core/REXMLPropertyListWriter.cpp
				src/core/REZLIBCompression.cpp
				src/core/REZipReader.cpp
				
				src/core/db/REDB.cpp
				src/core/db/REDBResultSet.cpp
				
				src/core/json/OKJSONParser.c
				src/core/json/REDictionaryJSONCallbacks.cpp
				
				src/core/unzip/ioapi.c
				src/core/unzip/unzip.c
				
				src/core/yuv2rgb/REYUV420toRGB565.cpp
				src/core/yuv2rgb/REYUV420toRGB888.cpp
				src/core/yuv2rgb/REYUV420toRGB8888.cpp
				src/core/yuv2rgb/REYUV422toRGB565.cpp
				src/core/yuv2rgb/REYUV422toRGB888.cpp
				src/core/yuv2rgb/REYUV422toRGB8888.cpp
				src/core/yuv2rgb/REYUV444toRGB565.cpp
				src/core/yuv2rgb/REYUV444toRGB888.cpp
				src/core/yuv2rgb/REYUV444toRGB8888.cpp
				src/core/yuv2rgb/REYUVtoRGB.cpp
				
				)


IF(__RE_OS_IPHONE__ OR __RE_OS_MACOSX__)
LIST(APPEND CORE_SRC src/core/RECoreObjectiveCSourceCode.mm)
ENDIF(__RE_OS_IPHONE__ OR __RE_OS_MACOSX__)


set(CORE_HDR 	include/recore.h
				recore_sdk_config.h)

add_definitions(-DHAVE_RECORE_SDK_CONFIG_H)
IF(__RE_OS_MACOSX__)
include_directories("src/addlibs1/macosx/include/")
ENDIF()

IF(__RE_OS_IPHONE__)
include_directories("src/addlibs1/ios/include/")
ENDIF()

add_library(recore ${CORE_SRC} ${CORE_HDR})

#	include_directories("headers/" "more_headers/")
#	find_package(ZLIB REQUIRED)
#	include_directories(${ZLIB_INCLUDE_DIR})



