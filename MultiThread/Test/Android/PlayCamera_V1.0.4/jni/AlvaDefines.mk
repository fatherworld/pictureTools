include $(LOCAL_PATH)/$(Shared_PATH)/GlobalConfig/Build/Android/Release.mk

LOCAL_CFLAGS    += -DCLASSNAME=com_alvasystems_arsdk_ARJni
LOCAL_CFLAGS    += -DIR
#NDK_APP_DST_DIR := IR/libs/$(TARGET_ARCH_ABI)