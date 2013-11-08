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

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libfreetype
LOCAL_MODULE_FILENAME := libfreetype

LOCAL_SRC_FILES := freetype/src/autofit/autofit.c \
freetype/src/bdf/bdf.c \
freetype/src/cff/cff.c \
freetype/src/base/ftbase.c \
freetype/src/base/ftbitmap.c \
freetype/src/cache/ftcache.c \
freetype/src/base/ftfstype.c \
freetype/src/base/ftgasp.c \
freetype/src/base/ftglyph.c \
freetype/src/gzip/ftgzip.c \
freetype/src/base/ftinit.c \
freetype/src/lzw/ftlzw.c \
freetype/src/base/ftstroke.c \
freetype/src/base/ftsystem.c \
freetype/src/smooth/smooth.c \
freetype/src/base/ftbbox.c \
freetype/src/base/ftmm.c \
freetype/src/base/ftpfr.c \
freetype/src/base/ftsynth.c \
freetype/src/base/fttype1.c \
freetype/src/base/ftwinfnt.c \
freetype/src/base/ftxf86.c \
freetype/src/base/ftlcdfil.c \
freetype/src/base/ftgxval.c \
freetype/src/base/ftotval.c \
freetype/src/base/ftpatent.c \
freetype/src/pcf/pcf.c \
freetype/src/pfr/pfr.c \
freetype/src/psaux/psaux.c \
freetype/src/pshinter/pshinter.c \
freetype/src/psnames/psmodule.c \
freetype/src/raster/raster.c \
freetype/src/sfnt/sfnt.c \
freetype/src/truetype/truetype.c \
freetype/src/type1/type1.c \
freetype/src/cid/type1cid.c \
freetype/src/type42/type42.c \
freetype/src/winfonts/winfnt.c


LOCAL_C_INCLUDES := $(LOCAL_PATH)/freetype/include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/freetype/include


LOCAL_CFLAGS := -DANDROID -DANDROID_NDK -DFT2_BUILD_LIBRARY=1 -DFT_CONFIG_OPTION_SYSTEM_ZLIB=1 -DDARWIN_NO_CARBON -DNDEBUG
LOCAL_CXXFLAGS := $(LOCAL_CFLAGS)
LOCAL_CPPFLAGS := $(LOCAL_CFLAGS)


include $(BUILD_STATIC_LIBRARY)

