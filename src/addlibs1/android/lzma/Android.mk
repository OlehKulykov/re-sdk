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

LOCAL_MODULE := liblzma
LOCAL_MODULE_FILENAME := liblzma

LOCAL_SRC_FILES := lzma/C/Alloc.c \
lzma/C/LzFind.c \
lzma/C/LzmaDec.c \
lzma/C/LzmaEnc.c \
lzma/C/LzmaLib.c 


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/lzma/C

LOCAL_CFLAGS := -DANDROID -DANDROID_NDK -DINFINITE=1 -D__LITTLE_ENDIAN__=1 -DMY_CPU_X86=1 -D_7ZIP_ST=1
LOCAL_CXXFLAGS := $(LOCAL_CFLAGS)
LOCAL_CPPFLAGS := $(LOCAL_CFLAGS)

include $(BUILD_STATIC_LIBRARY)

