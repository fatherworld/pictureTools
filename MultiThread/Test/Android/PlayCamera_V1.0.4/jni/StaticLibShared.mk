#
#
# 此文件使用方法
# 在Android.mk文件中通过以下方法调用
#
# #Shared_PATH为所有共享文件根路径（注意路径必须为/）
# #Shared_INCLUDE为所有共享文件根路径（注意路径必须为/）
# #Shared_Check_INCLUDE为所有共享文件根路径（注意路径必须为\）
# #通过include引用此文件
# #LOCAL_SRC_FILES添加到最终工程编译
# #LOCAL_C_INCLUDES添加工程编译引用路径
#
# #注意：共享只需要引用工程中用到的包即可，不用将这里提到的所有包全部引用为共享
#
# Shared_PATH             := ../../../../Shared
# Shared_INCLUDE          := $(LOCAL_PATH)/$(Shared_PATH)
# Shared_Check_INCLUDE    := $(LOCAL_PATH)\..\..\..\..\Shared
# include $(LOCAL_PATH)/$(Shared_PATH)/Build/Android/StaticLib.mk
#
# LOCAL_SRC_FILES	      += $(AlvaProjectPredefine_Module)
# LOCAL_C_INCLUDES        += $(AlvaProjectPredefine_INCLUDE)
#
#

ifeq ($(HOST_OS),windows)
CUST_TEST_CMD                    := dir
endif
ifeq ($(HOST_OS),linux)
CUST_TEST_CMD                    := test -f
endif

AlvaProjectPredefine_Module       :=
AlvaProjectPredefine_INCLUDE      :=

GC_INCLUDE                        := $(Shared_INCLUDE)/GlobalConfig
GC_PATH                           := $(Shared_PATH)/GlobalConfig
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\GlobalConfig
HAVE_TEST_CUST_GC                 := $(shell $(CUST_TEST_CMD) $(M_CHECK_Shared_INCLUDE)\)
ifneq ($(HAVE_TEST_CUST_GC),)
	AlvaProjectPredefine_Module       += 
	AlvaProjectPredefine_INCLUDE      += $(GC_INCLUDE)
endif


STD_MULTI_PLATFORM_INCLUDE        := $(Shared_INCLUDE)/std-multi-platform
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\std-multi-platform
HAVE_TEST_CUST_STD_MULTI_PLATFORM := $(shell $(CUST_TEST_CMD) $(M_CHECK_Shared_INCLUDE)\)
ifneq ($(HAVE_TEST_CUST_STD_MULTI_PLATFORM),)
	AlvaProjectPredefine_Module       += 
	AlvaProjectPredefine_INCLUDE      += $(STD_MULTI_PLATFORM_INCLUDE)
endif


DT_INCLUDE                        := $(Shared_INCLUDE)/DebugTools/include
DT_PATH                           := $(Shared_PATH)/DebugTools/src
DT_C_FILES                        := #Dump.c \
                                     AlvaTimer.c
DT_SRC_FILES                      := $(addprefix $(DT_PATH)/, $(DT_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\DebugTools\include
HAVE_TEST_CUST_DT                 := $(shell $(CUST_TEST_CMD) $(M_CHECK_Shared_INCLUDE)\)
ifneq ($(HAVE_TEST_CUST_DT),)
	AlvaProjectPredefine_Module       += $(DT_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(DT_INCLUDE)
endif




JNI_HELPER_INCLUDE                := $(Shared_INCLUDE)/JniHelper/include
JNI_HELPER_PATH                   := $(Shared_PATH)/JniHelper/src
JNI_HELPER_C_FILES                := JniHolder.c \
                                     AndroidHelper.c \
                                     JniLoad.cpp
JNI_HELPER_SRC_FILES              := $(addprefix $(JNI_HELPER_PATH)/, $(JNI_HELPER_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\JniHelper\include
HAVE_TEST_CUST_JNI_HELPER         := $(shell $(CUST_TEST_CMD) $(M_CHECK_Shared_INCLUDE)\)
ifneq ($(HAVE_TEST_CUST_JNI_HELPER),)
	AlvaProjectPredefine_Module       += $(JNI_HELPER_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(JNI_HELPER_INCLUDE)
endif


