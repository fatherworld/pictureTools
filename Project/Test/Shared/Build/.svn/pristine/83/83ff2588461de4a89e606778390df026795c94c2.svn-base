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
