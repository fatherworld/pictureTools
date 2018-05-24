#ifndef _EULER_ROTATION_MATRIX_H_
#define _EULER_ROTATION_MATRIX_H_

#ifdef AlvaPublic
#include "AngleHelper_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum _ROTATE_ORDER_ {
        ORDER_Start = -1,
        ORDER_XYZ, //RzRyRx
        ORDER_XZY, //RyRzRx
        ORDER_YXZ, //RzRxRy
        ORDER_YZX, //RxRzRy
        ORDER_ZXY, //RyRxRz
        ORDER_ZYX, //RxRyRz
        ORDER_NUM,
    }ROTATE_ORDER;

#ifndef M_PI
#define M_PI 3.1415927f
#endif
    /*
     * Radian2Angle
     * 弧度转角度
     */
    #define Radian2Angle(R) ((R) * 180.0f / M_PI)

     /*
     * Angle2Radian
     * 角度转弧度
     */
    #define Angle2Radian(A) ((A) * M_PI / 180.0f)

    int
    RotationMatrix2Euler(float* matrix, float* eular, ROTATE_ORDER order);

    int
    Euler2RotationMatrix(float* eular, float* matrix, ROTATE_ORDER order);

    /** 
     * @brief 将四元数转为欧拉角
     * @param quart    四元数，输入，qx, qy, qz, qw
     * @param eular    欧拉角，输出，x, y, z(弧度)
     */
    void
    FromQuarternion(const float* quart,float* eular);

#ifdef __cplusplus
}
#endif

#endif