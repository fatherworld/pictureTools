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
     * @brief 链表初始化
     * @param ImageSize: 每一帧数据大小,用于UpdateImage等函数的参数申请空间大小
     * @return: int型
     *         -<em> <0 </em> fail，错误码
     *         -<em>  0 </em> succeed
     */
    jint
    JNIFUNC(FrameInit)(JNIEnv * env, jobject obj, jint ImageSize) {

        int res = 0;
        res = FrameInit(ImageSize);

        return res;
    }

    /**
     * @brief 链表反初始化,释放初始化申请的空间
     * @return: int型
     *         -<em> <0 </em> fail，错误码
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
     * 将最新的源数据发送给识别库，执行一次识别过程
     * 此函数适用于输入图像为内存
     * @data: 输入数据指针
     * @matrix: 纹理的变化矩阵
     * @return: int型
     *         -<em> <0 </em> fail，错误码
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
     * 获取backGround YUV数据
     * @param data: ptr of YUV
     * @return: int型
     *         -<em> <0 </em> fail，错误码
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
