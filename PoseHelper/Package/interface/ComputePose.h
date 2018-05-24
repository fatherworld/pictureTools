#ifndef _ComputePose_H_
#define _ComputePose_H_

#ifdef AlvaPublic
#include "PoseHelper_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * setCameraInfo
     * �����������
     * @FOV: ����ӽǣ��Ƕȣ�Ĭ�ϣ�45��
     * @param markWidth: mark��Ĭ�ϣ�0
     * @param markHeight: mark�ߣ�Ĭ�ϣ�0
     * @param nearPlaneDistance: ���ü�ƽ����룬Ĭ�ϣ�10
     * @param farPlaneDistance: Զ�ü�ƽ����룬Ĭ�ϣ�2000
     * @param distanceCamera2Mark: ģ�;���Camera���룬Ĭ�ϣ�75
     */
    void
    setCameraInfo(float FOV, float imageWidth, float imageHeight, float nearPlaneDistance/*= 10.0f*/, float farPlaneDistance/*= 2000.0f*/, float distanceCamera2Mark/*= 75.0f*/);

    /*
     * getVirtualMarkSize
     * ��ȡ����ģ�ʹ�С
     * �˺���������setCameraInfo֮�����
     * @param markWidth: mark��
     * @param markHeight: mark��
     * @param width: ����ģ�Ϳ�
     * @param height: ����ģ�͸�
     */
    void
    getVirtualMarkSize(int markWidth, int markHeight, float* width, float* height);

    /*
     * ComputeRTS
     * Compute pose of module. Must call setCameraInfo before call this method.
     * @Note: Left-Hand System
     * @param num: in, num of points
     * @param srcfeatures: in, coords of mark points
     * @param markHeight : in, mark width
     * @param markHeight : in, mark height
     * @param dstfeatures: in, coords of points found on preview image
     * @param oviewmatrix: out, RTS
     *                          RTS = q1 q2 q3 q4 t1 t2 t3 s1 s2 s3
     */
    int
    ComputeRTS(int num, float* srcfeatures, float* power, int markWidth, int markHeight, float* dstfeatures, float* RTS);

    /*
     * ComputeRTS_Camera
     * Compute pose of camera. Must call setCameraInfo before call this method.
     * @Note: Left-Hand System
     * @param num: in, num of points
     * @param srcfeatures: in, coords of mark points
     * @param markHeight : in, mark width
     * @param markHeight : in, mark height
     * @param dstfeatures: in, coords of points found on preview image
     * @param oviewmatrix: out, RTS
     *                          RTS = q1 q2 q3 q4 t1 t2 t3 s1 s2 s3
     */
    int
    ComputeRTS_Camera(int num, float* srcfeatures, float* power, int markWidth, int markHeight, float* dstfeatures, float* RTS);

    /*
     * ComputeRTS_Weak
     * Compute pose of module. Must call setCameraInfo before call this method.
     * @Note: Left-Hand System
     * @param num: in, num of points
     * @param srcfeatures: in, coords of mark points
     * @param power: in, power of points
     * @param H0: in, initial Homography matrix
     * @param markWidth : in, mark width
     * @param markHeight : in, mark height
     * @param dstfeatures: in, coords of points found on preview image
     * @param oviewmatrix: out, RTS
     *                          RTS = q1 q2 q3 q4 t1 t2 t3 s1 s2 s3
     */
    int
    ComputeRTS_Weak(int num, float* srcfeatures, float* power, float* H0, int markWidth, int markHeight, float* dstfeatures, float* rts);

    /*
     * normalH2RTS
     * compute RTS from H matrix(normalized)
     * @param H  : in, H matrix(normalized)
     * @param rts: out, RTS
     *                  RTS = q1 q2 q3 q4 t1 t2 t3 s1 s2 s3
     */
    int
    normalH2RTS(float* H, float* rts);

    /*
     * commonH2RTS
     * compute RTS from H matrix(common, not normalized)
     * @param H  : in, H matrix(common, not normalized)
     * @param rts: out, RTS
     *                  RTS = q1 q2 q3 q4 t1 t2 t3 s1 s2 s3
     */
    int
    commonH2RTS(float* H, float* rts);

    /*
     * orthonormalizeH
     * orthonormalize H
     * @param H  : in, H matrix
     * @param rts: out, H matrix(orthonormalized)
     */
    int
    orthonormalizeH(float* H, float* Hout);

    /*
     * ComputeModuleMatrix
     * compute module matrix. Must call setCameraInfo before call this method.
     * @Note: Left-Hand System
     * @param num: in, num of points
     * @param srcfeatures: in, coords of mark points
     * @param markHeight : in, mark width
     * @param markHeight : in, mark height
     * @param dstfeatures: in, coords of points found on preview image
     * @param oviewmatrix: out, viewMatrix, this is norm viewmatrix
     *                                       a11 a12 a13 a14
     *                          viewMatrix = a21 a22 a23 a24
     *                                       a31 a32 a33 a34
     *                                       a41 a42 a43 a44
    */
    int
    ComputeViewMatrix(int num, float* srcfeatures, float* power, int markWidth, int markHeight, float* dstfeatures, float* viewMatrix);

    /*
     * ViewMatrix2RTS
     * convert view matrix to RTS. Must call setCameraInfo before call this method.
     * @param viewmatrix: in, view matrix. this is real view matrix
     *                                 R11 R12 R13 t1
     *                    viewmatrix = R21 R22 R23 t2
     *                                 R31 R32 R33 t3
     * @param RTS       : out, RTS
     *                    RTS = q1 q2 q3 q4 t1 t2 t3 s1 s2 s3
     */
    void
    ViewMatrix2RTS(float* viewmatrix, float* RTS);

    /*
    * ViewMatrix2RTS_Norm
    * convert view matrix to RTS. Must call setCameraInfo before call this method.
    * @param viewmatrix: in, view matrix, this is norm view matrix
    *                                 R11 R12 R13 t1
    *                    viewmatrix = R21 R22 R23 t2
    *                                 R31 R32 R33 t3
    * @param RTS       : out, RTS
    *                    RTS = -q1 q2 -q3 q4 t1*distanceCamera2Mark -t2*distanceCamera2Mark t3*distanceCamera2Mark s1 s2 s3
    */
    void
    ViewMatrix2RTS_Norm(float* viewmatrix, float* RTS);

    /*
    * ViewMatrix2RTS_Convert
    * convert view matrix to RTS. Must call setCameraInfo before call this method.
    * @param viewmatrix: in, view matrix. this is real view matrix. the coord system is different with RTS.
    *                                 R11 R12 R13 t1
    *                    viewmatrix = R21 R22 R23 t2
    *                                 R31 R32 R33 t3
    * @param RTS       : out, RTS
    *                    RTS = -q1 q2 -q3 q4 t1 -t2 t3 s1 s2 s3
    */
    void
    ViewMatrix2RTS_Convert(float* viewmatrix, float* RTS);

    /*
     * RTS2ViewMatrix
     * convert RTS to view matrix. Do not need to call setCameraInfo before call this method.
     * @param RTS       : in, RTS
     *                    RTS = q1 q2 q3 q4 t1 t2 t3 s1 s2 s3
     * @param viewmatrix: out, view matrix of RTS = (-q1 -q2 -q3 q4 t1 t2 t3 s1 s2 s3)
     *                                 R11 R12 R13 t1
     *                    viewmatrix = R21 R22 R23 t2
     *                                 R31 R32 R33 t3
     */
    void
    RTS2ViewMatrix(float* RTS, float* viewmatrix);

#define Max_Point_Nums 4000
    /*
     * ComputeH
     * @param basePoints:
     * @param matchPoints:
     * @param num:
     * @param H:
     *            H11 H12 H13
     *        H = H21 H22 H23
     *            H31 H32 H33
     * @param focus:
     * @param width:
     * @param height:
     */
    void ComputeH(float* basePoints, float* power, float* matchPoints, int num, float H[], float focus, int width, int height);

    /**
     * @brief ��ģ�͡��ӽ�תΪģ���ӽǾ���
     * @param viewRT    in , �ӽ�RT, qx, qy, qz, qw, tx, ty, tz
     * @param modelRT   in , ģ��RT, qx, qy, qz, qw, tx, ty, tz
     * @param vm_matrix out, ģ���ӽǾ���,
     *                       vm11, vm12, vm13, vm14
     *                       vm21, vm22, vm23, vm24
     *                       vm31, vm32, vm33, vm34
     *                       vm41, vm42, vm43, vm44
     *
     * @return
     */
    void RT2VMMatrix(float* viewRT, float* modelRT, float* vm_matrix);

    /**
     * @brief ��3D��ͶӰ��ͼ���У���UnityͶӰһ�£�
     * @param points3D     in , 3D�����꣬x1y1z1x2y2z2...
     * @param num          in , points3D�е���
     * @param viewRT       in , �ӽ�RT, qx, qy, qz, qw, tx, ty, tz
     * @param modelRT      in , ģ��RT, qx, qy, qz, qw, tx, ty, tz
     * @param points3D     out, ͶӰ��2D�����꣬x1y1x2y2...
     *
     * @return
     */
    void Project3DPoints(float* points3D, int num, float* viewRT, float* modelRT, float* out_points2D);

    /**
     * @brief ��3D��ͶӰ��ͼ���У���UnityͶӰһ�£�
     * @param points3D     in , 3D�����꣬x1y1z1x2y2z2...
     * @param num          in , points3D�е���
     * @param vm_matrix    in , ģ���ӽǾ���,
     *                          vm11, vm12, vm13, vm14
     *                          vm21, vm22, vm23, vm24
     *                          vm31, vm32, vm33, vm34
     *                          vm41, vm42, vm43, vm44
     * @param points3D     out, ͶӰ��2D�����꣬x1y1x2y2...
     *
     * @return
     */
    void Project3DPoints_VM(float* points3D, int num, float* vm_matrix, float* out_points2D);
#ifdef __cplusplus
}
#endif

#endif
