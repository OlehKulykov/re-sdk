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

LOCAL_MODULE    := libart_lgpl
LOCAL_MODULE_FILENAME := libart_lgpl

LOCAL_SRC_FILES := libart_lgpl/art_affine.c \
libart_lgpl/art_alphagamma.c \
libart_lgpl/art_bpath.c \
libart_lgpl/art_gray_svp.c \
libart_lgpl/art_misc.c \
libart_lgpl/art_pixbuf.c \
libart_lgpl/art_rect.c \
libart_lgpl/art_rect_svp.c \
libart_lgpl/art_rect_uta.c \
libart_lgpl/art_render.c \
libart_lgpl/art_render_gradient.c \
libart_lgpl/art_render_mask.c \
libart_lgpl/art_render_svp.c \
libart_lgpl/art_rgb.c \
libart_lgpl/art_rgb_a_affine.c \
libart_lgpl/art_rgb_affine.c \
libart_lgpl/art_rgb_affine_private.c \
libart_lgpl/art_rgb_bitmap_affine.c \
libart_lgpl/art_rgb_pixbuf_affine.c \
libart_lgpl/art_rgb_rgba_affine.c \
libart_lgpl/art_rgb_svp.c \
libart_lgpl/art_rgba.c \
libart_lgpl/art_rgba_rgba_affine.c \
libart_lgpl/art_svp.c \
libart_lgpl/art_svp_intersect.c \
libart_lgpl/art_svp_ops.c \
libart_lgpl/art_svp_point.c \
libart_lgpl/art_svp_render_aa.c \
libart_lgpl/art_svp_vpath.c \
libart_lgpl/art_svp_vpath_stroke.c \
libart_lgpl/art_svp_wind.c \
libart_lgpl/art_uta.c \
libart_lgpl/art_uta_ops.c \
libart_lgpl/art_uta_rect.c \
libart_lgpl/art_uta_svp.c \
libart_lgpl/art_uta_vpath.c \
libart_lgpl/art_vpath.c \
libart_lgpl/art_vpath_bpath.c \
libart_lgpl/art_vpath_dash.c \
libart_lgpl/art_vpath_svp.c \
libart_lgpl/libart-features.c 


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/libart_lgpl


LOCAL_CFLAGS := -DANDROID -DANDROID_NDK -DLIBART_COMPILATION
LOCAL_CXXFLAGS := $(LOCAL_CFLAGS)
LOCAL_CPPFLAGS := $(LOCAL_CFLAGS)


include $(BUILD_STATIC_LIBRARY)

