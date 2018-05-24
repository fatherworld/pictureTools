#ifndef _FRAMELIST_JNI_H_
#define _FRAMELIST_JNI_H_

#include "Config.h"

#include <jni.h>
#include <android/log.h>
#include <malloc.h>
#include <string.h>

#include "Format.h"
#include "AndroidHelper.h"

#define  LOG_TAG    "DataQueue_Jni"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define JNIFUNC_NOTUSE2(CLZ, FUN) JNIEXPORT JNICALL Java_##CLZ##_##FUN
#define JNIFUNC_NOTUSE1(CLZ, FUN) JNIFUNC_NOTUSE2(CLZ, FUN)
#define JNIFUNC(FUNCNAME) JNIFUNC_NOTUSE1(CLASSNAME, FUNCNAME)

#include "DataQueueInterface.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief �����ʼ��
     * @param ImageSize: ÿһ֡���ݴ�С,����UpdateImage�Ⱥ����Ĳ�������ռ��С
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    jint
    JNIFUNC(FrameInit)(JNIEnv * env, jobject obj, jint ImageSize) {

        int res = 0;
        res = FrameInit(ImageSize);

        return res;
    }

    /**
     * @brief ������ʼ��,�ͷų�ʼ������Ŀռ�
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    jint
    JNIFUNC(FrameUnit)(JNIEnv * env, jobject obj) {

        int res = 0;
        res = FrameUnit();

        return res;
    }

    /*
     * UpdateImage
     * �����µ�Դ���ݷ��͸�ʶ��⣬ִ��һ��ʶ�����
     * �˺�������������ͼ��Ϊ�ڴ�
     * @data: ��������ָ��
     * @matrix: ����ı仯����
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    jint
    JNIFUNC(UpdateImage)(JNIEnv * env, jobject obj, jbyteArray data, jfloatArray matrix) {

        unsigned char* cdata = NULL;
        float* cmatrix = NULL;
        int res = 0;

        if (data)
            cdata = (unsigned char *)env->GetByteArrayElements(data, 0);
        if (matrix)
            cmatrix = (float *)env->GetFloatArrayElements(matrix, 0);

        if (cdata && cmatrix) {
            res = UpdateImage(cdata, cmatrix);
        }

        if (cdata)
            env->ReleaseByteArrayElements(data, (jbyte*)cdata, JNI_ABORT);
        if (cmatrix)
            env->ReleaseFloatArrayElements(matrix, (jfloat*)cmatrix, JNI_ABORT);
        return res;
    }

    /*
     * getBackground
     * ��ȡbackGround YUV����
     * @param data: ptr of YUV
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    jint
    JNIFUNC(getBackground)(JNIEnv * env, jobject obj, jbyteArray data) {

        unsigned char* cdata = NULL;
        int res = 0;

        if (data)
            cdata = (unsigned char *)env->GetByteArrayElements(data, 0);

        res = getBackground(cdata);

        if (cdata)
            env->ReleaseByteArrayElements(data, (jbyte*)cdata, JNI_ABORT);
        return res;
    }

#ifdef __cplusplus
}
#endif

#endif
