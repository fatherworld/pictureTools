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
LOCAL_PATH              := $(call my-dir)

include $(CLEAR_VARS)

Shared_PATH             := ./../../../Shared
Shared_INCLUDE          := $(LOCAL_PATH)/$(Shared_PATH)
Shared_Check_INCLUDE    := $(LOCAL_PATH)\..\..\..\Shared
include $(LOCAL_PATH)/StaticLibShared.mk


include $(LOCAL_PATH)/StaticLibWithJni.mk
Palm_Module_PATH    := ./
Palm_Module_INCLUDE := $(LOCAL_PATH)/$(Palm_Module_PATH)


Palm_Module_FILES         := $(addprefix $(Palm_Module_PATH)/, $(Palm_Module))
Palm_INCLUDE              := $(addprefix $(Palm_Module_INCLUDE)/, $(Palm_INCLUDE))


LOCAL_MODULE            := PalmPerfTest

LOCAL_CPP_EXTENSION     := .cc .cpp

LOCAL_SRC_FILES	        := $(Palm_Module_FILES)                                  \
                           $(AlvaProjectPredefine_Module)

LOCAL_C_INCLUDES        := $(Palm_INCLUDE)                                       \
                           $(AlvaProjectPredefine_INCLUDE)


LOCAL_LDLIBS            := -llog -lGLESv1_CM -lGLESv2 -lEGL -landroid -latomic -lc



$(warning  $(TARGET_PLATFORM))
$(warning  $(TARGET_ARCH))


include $(LOCAL_PATH)/AlvaDefines.mk

include $(BUILD_SHARED_LIBRARY)