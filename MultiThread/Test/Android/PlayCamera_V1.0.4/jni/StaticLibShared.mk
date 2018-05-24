#
#
# ���ļ�ʹ�÷���
# ��Android.mk�ļ���ͨ�����·�������
#
# #Shared_PATHΪ���й����ļ���·����ע��·������Ϊ/��
# #Shared_INCLUDEΪ���й����ļ���·����ע��·������Ϊ/��
# #Shared_Check_INCLUDEΪ���й����ļ���·����ע��·������Ϊ\��
# #ͨ��include���ô��ļ�
# #LOCAL_SRC_FILES��ӵ����չ��̱���
# #LOCAL_C_INCLUDES��ӹ��̱�������·��
#
# #ע�⣺����ֻ��Ҫ���ù������õ��İ����ɣ����ý������ᵽ�����а�ȫ������Ϊ����
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


