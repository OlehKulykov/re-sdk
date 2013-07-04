
#!/bin/bash

clear

rm -f *.o
rm -f *.c
rm -f *.cpp
rm -f REConfigHardcoded.h

echo -e "\n// This header file generated automatically by conf.sh script on: "$(date)" \n" >> REConfigHardcoded.h

COMPILER=gcc

LAST_CHECK_HEADER_RESULT=0

IS_HAVE_INTTYPES=0
IS_HAVE_SYS_TYPES=0
IS_HAVE_STD_BOOL=0
IS_HAVE_STD_INT=0
IS_HAVE_LIMITS=0
IS_HAVE_FLOAT=0
IS_HAVE_MATH=0

check_header(){

LAST_CHECK_HEADER_RESULT=0

rm -f check_system_header.c
rm -f check_system_header.o

local SYSTEM_HEADER=$1
local UPER_SYSTEM_HEADER="$(tr "[a-z]./-" "[A-Z]___" <<< "$SYSTEM_HEADER")"

echo $UPER_SYSTEM_HEADER

echo -e "\n#include <$SYSTEM_HEADER>\n\nint main()\n{\nreturn 1;\n}\n\n" > check_system_header.c
$COMPILER -c check_system_header.c -o check_system_header.o
if [ -f check_system_header.o ]
then
	LAST_CHECK_HEADER_RESULT=1
    echo -e "<$SYSTEM_HEADER>: yes\n"
    echo -e "\n// #include <$SYSTEM_HEADER>\n#define __RE_HAVE_SYSTEM_"$UPER_SYSTEM_HEADER"__ \n"  >> REConfigHardcoded.h
else
    echo -e "<$SYSTEM_HEADER>: no\n"
    echo -e "\n// NO #include <$SYSTEM_HEADER>\n#undef __RE_HAVE_SYSTEM_"$UPER_SYSTEM_HEADER"__ \n"  >> REConfigHardcoded.h
fi
rm -f check_system_header.c
	rm -f check_system_header.o
}

check_re_types(){

rm -f check_re_types.c
rm -f check_re_types.o

local TYPES_HEADERS=""

if [ $IS_HAVE_INTTYPES -eq 1 ] 
then
TYPES_HEADERS+="#include <inttypes.h> \n"
else 
echo " "
fi

if [ $IS_HAVE_SYS_TYPES -eq 1 ] 
then
TYPES_HEADERS+="#include <sys/types.h> \n"
else 
echo " "
fi

if [ $IS_HAVE_STD_INT -eq 1 ] 
then
TYPES_HEADERS+="#include <stdint.h> \n"
else 
echo " "
fi

if [ $IS_HAVE_LIMITS -eq 1 ] 
then
TYPES_HEADERS+="#include <limits.h> \n"
else 
echo " "
fi

if [ $IS_HAVE_FLOAT -eq 1 ] 
then
TYPES_HEADERS+="#include <float.h> \n"
else 
echo " "
fi

if [ $IS_HAVE_MATH -eq 1 ] 
then
TYPES_HEADERS+="#include <math.h> \n"
else 
echo " "
fi

if [ $IS_HAVE_STD_BOOL -eq 1 ] 
then
TYPES_HEADERS+="#include <stdbool.h> \n"
else 
echo " "
fi

echo -e $TYPES_HEADERS >> check_re_types.c

local TYPES_DEFINES=""

TYPES_DEFINES+="typedef uint8_t REUByte; \n"
TYPES_DEFINES+="typedef uint16_t REUInt16; \n"
TYPES_DEFINES+="typedef uint32_t REUInt32; \n"
TYPES_DEFINES+="typedef uint64_t REUInt64; \n"
TYPES_DEFINES+="typedef int8_t REByte; \n"
TYPES_DEFINES+="typedef int16_t REInt16; \n"
TYPES_DEFINES+="typedef int32_t REInt32; \n"
TYPES_DEFINES+="typedef int64_t REInt64; \n"
TYPES_DEFINES+="typedef float_t REFloat32; \n"
TYPES_DEFINES+="typedef double_t REFloat64; \n"
TYPES_DEFINES+="typedef double_t RETimeInterval; \n"
TYPES_DEFINES+="typedef uintptr_t REUIdentifier; \n"
TYPES_DEFINES+="typedef bool REBOOL; \n"

echo -e $TYPES_DEFINES >> check_re_types.c

echo -e "\nint main()\n{\nreturn 1;\n}\n\n" >> check_re_types.c

$COMPILER -c check_re_types.c -o check_re_types.o
if [ -f check_re_types.o ]
then
	echo -e "// System headers" >> REConfigHardcoded.h
	echo -e $TYPES_HEADERS >> REConfigHardcoded.h
	echo -e $TYPES_DEFINES >> REConfigHardcoded.h
	echo -e "#define __RETYPES_DEFINED___ \n" >> REConfigHardcoded.h
else
echo " "
fi

rm -f check_re_types.c
rm -f check_re_types.o

}


echo "Checking headers ..."

check_header "inttypes.h"
IS_HAVE_INTTYPES=$LAST_CHECK_HEADER_RESULT

check_header "sys/types.h"
IS_HAVE_SYS_TYPES=$LAST_CHECK_HEADER_RESULT

check_header "stdbool.h"
IS_HAVE_STD_BOOL=$LAST_CHECK_HEADER_RESULT

check_header "stdint.h"
IS_HAVE_STD_INT=$LAST_CHECK_HEADER_RESULT

check_header "limits.h"
IS_HAVE_LIMITS=$LAST_CHECK_HEADER_RESULT

check_header "float.h"
IS_HAVE_FLOAT=$LAST_CHECK_HEADER_RESULT


check_header "mach/mach_time.h"
check_header "sys/time.h"
check_header "time.h"
check_header "locale.h"
check_header "sys/stat.h"
check_header "fcntl.h"
check_header "pthread.h"
check_header "sys/resource.h"
check_header "direct.h"
check_header "wchar.h"
check_header "memory.h"

check_header "math.h"
IS_HAVE_MATH=$LAST_CHECK_HEADER_RESULT

check_header "malloc.h"
check_header "byteswap.h"

check_header "lua.h"
check_header "lualib.h"

check_header "errno.h"
check_header "setjmp.h"
check_header "unistd.h"

check_header "zlib.h"

check_header "bzlib.h"

check_header "png.h"

check_header "jpeglib.h"

check_header "openjpeg-1.5/openjpeg.h"
check_header "openjpeg-2.0/openjpeg.h"
check_header "openjpeg/openjpeg.h"
check_header "openjpeg.h"

check_header "webp/types.h"
check_header "webp/mux.h"
check_header "webp/decode.h"
check_header "webp/encode.h"

check_header "sqlite3.h"

check_header "libpq-fe.h"
check_header "postgresql/libpq-fe.h"

check_header "ft2build.h"
check_header "freetype/freetype.h"
check_header "freetype2/freetype/freetype.h"

check_header "lzma/lzma.h"
check_header "lzma.h"
check_header "LzmaLib.h"

check_header "vorbis/codec.h"
check_header "vorbis/vorbisfile.h"

check_header "theora/theoradec.h"

check_header "tremor/ivorbiscodec.h"
check_header "tremor/ivorbisfile.h"


check_header "curl/curl.h"
check_header "ogg/ogg.h"



check_re_types


echo -e "// SDKROOT="$SDKROOT >> REConfigHardcoded.h

echo -e "// GCC_ENABLE_CPP_RTTI="$GCC_ENABLE_CPP_RTTI >> REConfigHardcoded.h
echo -e "// GCC_ENABLE_CPP_EXCEPTIONS="$GCC_ENABLE_CPP_EXCEPTIONS >> REConfigHardcoded.h
echo -e "// GCC_PREPROCESSOR_DEFINITIONS="$GCC_PREPROCESSOR_DEFINITIONS >> REConfigHardcoded.h
echo -e "// GCC_PREPROCESSOR_DEFINITIONS_NOT_USED_IN_PRECOMPS="$GCC_PREPROCESSOR_DEFINITIONS_NOT_USED_IN_PRECOMPS >> REConfigHardcoded.h
echo -e "// GCC_VERSION="$GCC_VERSION >> REConfigHardcoded.h
echo -e "// GCC_VERSION_IDENTIFIER="$GCC_VERSION_IDENTIFIER >> REConfigHardcoded.h


echo -e "\n\n" >> REConfigHardcoded.h

