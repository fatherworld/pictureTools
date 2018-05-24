# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

Shared_PATH             := ./../../Shared
Shared_INCLUDE          := $(LOCAL_PATH)/$(Shared_PATH)
Shared_Check_INCLUDE    := $(LOCAL_PATH)\..\..\Shared
include $(LOCAL_PATH)/$(Shared_PATH)/Build/Android/StaticLib.mk

LOCAL_MODULE            := ComputeHTest
LOCAL_SRC_FILES         := main.c

LOCAL_CPP_EXTENSION     := .cc .cpp

LOCAL_SRC_FILES	        += ../../../Core/src/ComputeH_Direct.c
LOCAL_SRC_FILES	        += $(AlvaProjectPredefine_Module)

LOCAL_C_INCLUDES        := $(LOCAL_PATH)/../../../Core/include
LOCAL_C_INCLUDES        += $(AlvaProjectPredefine_INCLUDE)

LOCAL_STATIC_LIBRARIES  := android_native_app_glue
LOCAL_LDLIBS            := -llog -lGLESv1_CM -lGLESv2 -lEGL -landroid -latomic

include $(LOCAL_PATH)/AlvaDefines.mk

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)