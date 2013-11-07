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

LOCAL_MODULE := libtinyxml2
LOCAL_MODULE_FILENAME := libtinyxml2

LOCAL_SRC_FILES := tinyxml2/tinyxml2.cpp


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/tinyxml2


LOCAL_CFLAGS := -DANDROID -DANDROID_NDK
LOCAL_CXXFLAGS := $(LOCAL_CFLAGS)
LOCAL_CPPFLAGS := $(LOCAL_CFLAGS)


include $(BUILD_STATIC_LIBRARY)

