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

LOCAL_MODULE := librecoresdk
LOCAL_MODULE_FILENAME := librecoresdk

LOCAL_SRC_FILES := ../../../src/core/RELog.cpp \
../../../src/core/REDate.cpp \
../../../src/core/REThread.cpp \
../../../src/core/RE2DAlgorithm.cpp \
../../../src/core/REAffineTransform.cpp \
../../../src/core/REAngle.cpp \
../../../src/core/REApplication.cpp \
../../../src/core/REArray.cpp \
../../../src/core/REArrayObject.cpp \
../../../src/core/REAutoReleasePool.cpp \
../../../src/core/REAutoReleasePoolPrivate.cpp \
../../../src/core/REBase64.cpp \
../../../src/core/REBuffer.cpp \
../../../src/core/REBufferNoCopy.cpp \
../../../src/core/REBufferObject.cpp \
../../../src/core/RECRC32Generator.cpp \
../../../src/core/REColor.cpp \
../../../src/core/RECoreC.cpp \
../../../src/core/REData.cpp \
../../../src/core/REDictionary.cpp \
../../../src/core/REEdgeInsets.cpp \
../../../src/core/REFile.cpp \
../../../src/core/REFileManager.cpp \
../../../src/core/REImage.cpp \
../../../src/core/REImageBase.cpp \
../../../src/core/REImageManager.cpp \
../../../src/core/RELZMACompression.cpp \
../../../src/core/RELocale.cpp \
../../../src/core/REMD5Generator.cpp \
../../../src/core/REMainLoopUpdatable.cpp \
../../../src/core/REMainLoopsObjectsStoragePrivate.cpp \
../../../src/core/REMath.cpp \
../../../src/core/REMatrix4.cpp \
../../../src/core/REMutableImage.cpp \
../../../src/core/REMutableString.cpp \
../../../src/core/REMutex.cpp \
../../../src/core/RENULL.cpp \
../../../src/core/RENULLObject.cpp \
../../../src/core/RENotificationManager.cpp \
../../../src/core/RENumber.cpp \
../../../src/core/RENumberObject.cpp \
../../../src/core/REObject.cpp \
../../../src/core/REPoint2.cpp \
../../../src/core/REPoint3.cpp \
../../../src/core/REQuaternion.cpp \
../../../src/core/RERandomizer.cpp \
../../../src/core/RERect.cpp \
../../../src/core/REResourcesStorage.cpp \
../../../src/core/RESQLiteDB.cpp \
../../../src/core/RESQLiteDBResultSet.cpp \
../../../src/core/RESerializable.cpp \
../../../src/core/RESize.cpp \
../../../src/core/REStaticString.cpp \
../../../src/core/REString.cpp \
../../../src/core/REStringBase.cpp \
../../../src/core/REStringObject.cpp \
../../../src/core/REStringUtilsPrivate.cpp \
../../../src/core/RETetragon.cpp \
../../../src/core/RETime.cpp \
../../../src/core/RETimer.cpp \
../../../src/core/RETypedArray.cpp \
../../../src/core/RETypedPtr.cpp \
../../../src/core/REURL.cpp \
../../../src/core/REURLConnection.cpp \
../../../src/core/REURLRequest.cpp \
../../../src/core/REURLResponse.cpp \
../../../src/core/REUserCallBacks.cpp \
../../../src/core/REVector2.cpp \
../../../src/core/REVector3.cpp \
../../../src/core/REWideString.cpp \
../../../src/core/REXMLPropertyListReader.cpp \
../../../src/core/REXMLPropertyListWriter.cpp \
../../../src/core/REZLIBCompression.cpp \
../../../src/core/REZipReader.cpp \
../../../src/core/db/REDB.cpp \
../../../src/core/db/REDBResultSet.cpp \
../../../src/core/json/OKJSONParser.c \
../../../src/core/json/REDictionaryJSONCallbacks.cpp \
../../../src/core/unzip/ioapi.c \
../../../src/core/unzip/unzip.c \
../../../src/core/yuv2rgb/REYUV420toRGB565.cpp \
../../../src/core/yuv2rgb/REYUV420toRGB888.cpp \
../../../src/core/yuv2rgb/REYUV420toRGB8888.cpp \
../../../src/core/yuv2rgb/REYUV422toRGB565.cpp \
../../../src/core/yuv2rgb/REYUV422toRGB888.cpp \
../../../src/core/yuv2rgb/REYUV422toRGB8888.cpp \
../../../src/core/yuv2rgb/REYUV444toRGB565.cpp \
../../../src/core/yuv2rgb/REYUV444toRGB888.cpp \
../../../src/core/yuv2rgb/REYUV444toRGB8888.cpp \
../../../src/core/yuv2rgb/REYUVtoRGB.cpp 
			

INCLUDE_ROOT := $(LOCAL_PATH)/../../../src/addlibs1/android

LOCAL_C_INCLUDES := $(INCLUDE_ROOT)/art_lgpl
LOCAL_C_INCLUDES += $(INCLUDE_ROOT)/jpeg/jpeg
LOCAL_C_INCLUDES += $(INCLUDE_ROOT)/lzma/lzma/C
LOCAL_C_INCLUDES += $(INCLUDE_ROOT)/png/libpng
LOCAL_C_INCLUDES += $(INCLUDE_ROOT)/tinyxml2/tinyxml2
LOCAL_C_INCLUDES += $(INCLUDE_ROOT)/webp/src


LOCAL_CFLAGS := -DANDROID -DANDROID_NDK -D__RE_OS_ANDROID__=1 -DHAVE_RECORE_SDK_CONFIG_H=1
LOCAL_CXXFLAGS := $(LOCAL_CFLAGS)
LOCAL_CPPFLAGS := $(LOCAL_CFLAGS)

LOCAL_CPP_FEATURES := rtti

LOCAL_LDLIBS := -llog -lz


LOCAL_STATIC_LIBRARIES := libart_lgpl 
LOCAL_STATIC_LIBRARIES += libjpeg 
LOCAL_STATIC_LIBRARIES += liblzma 
LOCAL_STATIC_LIBRARIES += libpng 
LOCAL_STATIC_LIBRARIES += libtinyxml2 
LOCAL_STATIC_LIBRARIES += libwebp
LOCAL_STATIC_LIBRARIES += libcpufeatures 


include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)

$(call import-add-path,$(LOCAL_PATH)/../../../src/addlibs1/android)
$(call import-module,art_lgpl)
$(call import-module,jpeg)
$(call import-module,lzma)
$(call import-module,png)
$(call import-module,tinyxml2)
$(call import-module,webp)





