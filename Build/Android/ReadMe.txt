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
