#ifndef _ALVAMATRIX_H_
#define _ALVAMATRIX_H_

/**
 * @file      AlvaMatrix.h
 */

#ifdef AlvaPublic
#include "MatrixCompute_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	//矩阵结构体
	typedef struct _AlvaMatrix_{
		int rows;   //矩阵行数
		int cols;   //矩阵列数
		float* matrix;
	}AlvaMatrix;

	/**
	 * @brief 矩阵相乘 input_martix_left * input_martix_right = output_martix
	 * @param input_martix_left: 左边矩阵
	 * @param input_martix_right: 右边矩阵
	 * @param output_martix: 结果矩阵
	 * @return: 空
	 */
	void MulMatrix(AlvaMatrix* input_martix_left, AlvaMatrix* input_martix_right, AlvaMatrix* output_martix);

    /**
	 * @brief 4 X 4 矩阵相乘 M_L * M_R = M_Res
	 * @param M_L: 左边矩阵
	 * @param M_R: 右边矩阵
	 * @param M_Res: 结果矩阵
	 * @return: 0
	 */
	int asM4xM4(float M_L[16], float M_R[16], float M_Res[16]);
#define M4xM4(M_L, M_R, M_Res) asM4xM4(M_L->matrix, M_R->matrix, M_Res->matrix)

    /**
    * @brief 3 X 3 矩阵相乘 M_L * M_R = M_Res
    * @param M_L: 左边矩阵
    * @param M_R: 右边矩阵
    * @param M_Res: 结果矩阵
    * @return: 0
    */
    int asM3xM3(float M_L[9], float M_R[9], float M_Res[9]);
#define M3xM3(M_L, M_R, M_Res) asM3xM3(M_L->matrix, M_R->matrix, M_Res->matrix)

    /**
    * @brief 2 X 2 矩阵相乘 M_L * M_R = M_Res
    * @param M_L: 左边矩阵
    * @param M_R: 右边矩阵
    * @param M_Res: 结果矩阵
    * @return: 0
    */
    int asM2xM2(float M_L[4], float M_R[4], float M_Res[4]);
#define M2xM2(M_L, M_R, M_Res) asM2xM2(M_L->matrix, M_R->matrix, M_Res->matrix)

	/**
	 * @brief 4 X 4 矩阵和V[4] 向量相乘
	 * @param M_L: 左边矩阵
	 * @param V: 右边向量
	 * @param M_Res: 结果向量
	 * @return: 0
	 */
	int asM4xV4(float M_L[16], float V[4], float M_Res[4]);
#define M4xV4(M_L, M_R, M_Res) asM4xV4(M_L->matrix, M_R->matrix, M_Res->matrix)

	/**
	 * @brief 3 X 3 矩阵和V[3] 向量相乘
	 * @param M_L: 左边矩阵
	 * @param V: 右边向量
	 * @param M_Res: 结果向量
	 * @return: 0
	 */
	int asM3xV3(float M_L[9], float V[3], float M_Res[3]);
#define M3xV3(M_L, M_R, M_Res) asM3xV3(M_L->matrix, M_R->matrix, M_Res->matrix)

	/**
	 * @brief 2 X 2 矩阵和V[2] 向量相乘
	 * @param M_L: 左边矩阵
	 * @param V: 右边向量
	 * @param M_Res: 结果向量
	 * @return: 0
	 */
	int asM2xV2(float M_L[4], float V[2], float M_Res[2]);
#define M2xV2(M_L, M_R, M_Res) asM2xV2(M_L->matrix, M_R->matrix, M_Res->matrix)

	/**
	 * @brief V[4]归一化向量
	 * @param V: 向量
	 * @param VNor: 归一化结果
	 * @return: 0
	 */
	int asV4Normal(float V[4], float VNor[4]);
#define V4Normal(V, VNor) asV4Normal(V->matrix, VNor->matrix)

	/**
	 * @brief V[3]归一化向量
	 * @param V: 向量
	 * @param VNor: 归一化结果
	 * @return: 0
	 */
	int asV3Normal(float V[3], float VNor[3]);
#define V3Normal(V, VNor) asV3Normal(V->matrix, VNor->matrix)

	/**
	 * @brief V[2]归一化向量
	 * @param V: 向量
	 * @param VNor: 归一化结果
	 * @return: 0
	 */
	int asV2Normal(float V[2], float VNor[2]);
#define V2Normal(V, VNor) asV2Normal(V->matrix, VNor->matrix)

#ifndef AlvaPublic
    /**
     * @brief 格式化显示矩阵
     * @param matrix: 矩阵
     * @return: 0
     */
    #define printf_m(matrix) printf_as_m(matrix->martix, matrix->rows, matrix->cols)

    /**
     * @brief 按矩阵格式化显示
     * @param matrix: 矩阵
     * @return: 0
     */
    void printf_as_m(float* matrix, int row, int col);
#endif

    /**
     * @brief 按矩阵检查两个内存是否相等
     * @param matrix1: 矩阵
     * @param matrix2: 矩阵
     * @param row: 矩阵行数
     * @param col: 矩阵列数
     * @return: 0: 不相等, 1: 相等
     */
    int isAsMatrixsEqual(float* matrix1, float* matrix2, int row, int col, float err);
#define isMatrixsEqual(matrix1, matrix2, row, col, err) isAsMatrixsEqual(matrix1->matrix, matrix2->matrix, matrix1->rows, matrix1->cols, err)

    /**
     * @brief 矩阵求逆, inv_matrix = matrix ^ -1
     * @param matrix: 矩阵
     * @param inv_matrix: 逆矩阵
     * @return: 1: 求逆成功, 0: 求逆失败
     */
    int InvMatrix(AlvaMatrix* matrix, AlvaMatrix* inv_matrix);

    /**
     * @brief 矩阵求逆, inv_matrix = matrix ^ -1
     * @param matrix: 矩阵
     * @param inv_matrix: 逆矩阵
     * @return: 1: 求逆成功, 0: 求逆失败
     */
    int asM4Inv(float* matrix, float* inv_matrix);
#define M4Inv(matrix, inv_matrix) asM4Inv(matrix->matrix, inv_matrix->matrix)
    /**
     * @brief 矩阵求逆, inv_matrix = matrix ^ -1
     * @param matrix: 矩阵
     * @param inv_matrix: 逆矩阵
     * @return: 1: 求逆成功, 0: 求逆失败
     */
    int asM3Inv(float* matrix, float* inv_matrix);
#define M3Inv(matrix, inv_matrix) asM3Inv(matrix->matrix, inv_matrix->matrix)

    /**
     * @brief 矩阵求逆, inv_matrix = matrix ^ -1
     * @param matrix: 矩阵
     * @param inv_matrix: 逆矩阵
     * @return: 1: 求逆成功, 0: 求逆失败
     */
    int asM2Inv(float* matrix, float* inv_matrix);
#define M2Inv(matrix, inv_matrix) asM2Inv(matrix->matrix, inv_matrix->matrix)

    /**
    * @brief 求矩阵行列式
    * @param matrix: 矩阵
    * @return: 行列式
    */
    float DetMatrix(AlvaMatrix* matrix);

    /**
    * @brief 求矩阵行列式
    * @param matrix: 矩阵
    * @return: 行列式
    */
    float asM4Det(float* matrix);
#define M4Det(matrix) asM4Det(matrix->matrix)

    /**
    * @brief 求矩阵行列式
    * @param matrix: 矩阵
    * @return: 行列式
    */
    float asM3Det(float* matrix);
#define M3Det(matrix) asM3Det(matrix->matrix)

    /**
    * @brief 求矩阵行列式
    * @param matrix: 矩阵
    * @return: 行列式
    */
    float asM2Det(float* matrix);
#define M2Det(matrix) asM2Det(matrix->matrix)
#ifdef __cplusplus
}
#endif
#endif