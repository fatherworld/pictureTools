#ifndef _QuarternionRotationMatrix_H_
#define _QuarternionRotationMatrix_H_

#ifdef AlvaPublic
#include "AngleHelper_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    void Quarternion2RotationMatrix(float q1, float q2, float q3, float q4, float* rm);
    void RotationMatrix2Quarternion(float* rm, float*q, float* qbase);

    /** 
     * @brief 将欧拉角转为四元数
     * @param eular    欧拉角，输入，x, y, z(弧度)
     * @param quart    四元数，输出，qx, qy, qz, qw
     *
     * @return 
     */
    void FromEular(const float* eular, float* quart);
#ifdef __cplusplus
}
#endif

#endif