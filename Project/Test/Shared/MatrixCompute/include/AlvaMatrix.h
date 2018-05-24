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

	//����ṹ��
	typedef struct _AlvaMatrix_{
		int rows;   //��������
		int cols;   //��������
		float* matrix;
	}AlvaMatrix;

	/**
	 * @brief ������� input_martix_left * input_martix_right = output_martix
	 * @param input_martix_left: ��߾���
	 * @param input_martix_right: �ұ߾���
	 * @param output_martix: �������
	 * @return: ��
	 */
	void MulMatrix(AlvaMatrix* input_martix_left, AlvaMatrix* input_martix_right, AlvaMatrix* output_martix);

    /**
	 * @brief 4 X 4 ������� M_L * M_R = M_Res
	 * @param M_L: ��߾���
	 * @param M_R: �ұ߾���
	 * @param M_Res: �������
	 * @return: 0
	 */
	int asM4xM4(float M_L[16], float M_R[16], float M_Res[16]);
#define M4xM4(M_L, M_R, M_Res) asM4xM4(M_L->matrix, M_R->matrix, M_Res->matrix)

    /**
    * @brief 3 X 3 ������� M_L * M_R = M_Res
    * @param M_L: ��߾���
    * @param M_R: �ұ߾���
    * @param M_Res: �������
    * @return: 0
    */
    int asM3xM3(float M_L[9], float M_R[9], float M_Res[9]);
#define M3xM3(M_L, M_R, M_Res) asM3xM3(M_L->matrix, M_R->matrix, M_Res->matrix)

    /**
    * @brief 2 X 2 ������� M_L * M_R = M_Res
    * @param M_L: ��߾���
    * @param M_R: �ұ߾���
    * @param M_Res: �������
    * @return: 0
    */
    int asM2xM2(float M_L[4], float M_R[4], float M_Res[4]);
#define M2xM2(M_L, M_R, M_Res) asM2xM2(M_L->matrix, M_R->matrix, M_Res->matrix)

	/**
	 * @brief 4 X 4 �����V[4] �������
	 * @param M_L: ��߾���
	 * @param V: �ұ�����
	 * @param M_Res: �������
	 * @return: 0
	 */
	int asM4xV4(float M_L[16], float V[4], float M_Res[4]);
#define M4xV4(M_L, M_R, M_Res) asM4xV4(M_L->matrix, M_R->matrix, M_Res->matrix)

	/**
	 * @brief 3 X 3 �����V[3] �������
	 * @param M_L: ��߾���
	 * @param V: �ұ�����
	 * @param M_Res: �������
	 * @return: 0
	 */
	int asM3xV3(float M_L[9], float V[3], float M_Res[3]);
#define M3xV3(M_L, M_R, M_Res) asM3xV3(M_L->matrix, M_R->matrix, M_Res->matrix)

	/**
	 * @brief 2 X 2 �����V[2] �������
	 * @param M_L: ��߾���
	 * @param V: �ұ�����
	 * @param M_Res: �������
	 * @return: 0
	 */
	int asM2xV2(float M_L[4], float V[2], float M_Res[2]);
#define M2xV2(M_L, M_R, M_Res) asM2xV2(M_L->matrix, M_R->matrix, M_Res->matrix)

	/**
	 * @brief V[4]��һ������
	 * @param V: ����
	 * @param VNor: ��һ�����
	 * @return: 0
	 */
	int asV4Normal(float V[4], float VNor[4]);
#define V4Normal(V, VNor) asV4Normal(V->matrix, VNor->matrix)

	/**
	 * @brief V[3]��һ������
	 * @param V: ����
	 * @param VNor: ��һ�����
	 * @return: 0
	 */
	int asV3Normal(float V[3], float VNor[3]);
#define V3Normal(V, VNor) asV3Normal(V->matrix, VNor->matrix)

	/**
	 * @brief V[2]��һ������
	 * @param V: ����
	 * @param VNor: ��һ�����
	 * @return: 0
	 */
	int asV2Normal(float V[2], float VNor[2]);
#define V2Normal(V, VNor) asV2Normal(V->matrix, VNor->matrix)

#ifndef AlvaPublic
    /**
     * @brief ��ʽ����ʾ����
     * @param matrix: ����
     * @return: 0
     */
    #define printf_m(matrix) printf_as_m(matrix->martix, matrix->rows, matrix->cols)

    /**
     * @brief �������ʽ����ʾ
     * @param matrix: ����
     * @return: 0
     */
    void printf_as_m(float* matrix, int row, int col);
#endif

    /**
     * @brief �������������ڴ��Ƿ����
     * @param matrix1: ����
     * @param matrix2: ����
     * @param row: ��������
     * @param col: ��������
     * @return: 0: �����, 1: ���
     */
    int isAsMatrixsEqual(float* matrix1, float* matrix2, int row, int col, float err);
#define isMatrixsEqual(matrix1, matrix2, row, col, err) isAsMatrixsEqual(matrix1->matrix, matrix2->matrix, matrix1->rows, matrix1->cols, err)

    /**
     * @brief ��������, inv_matrix = matrix ^ -1
     * @param matrix: ����
     * @param inv_matrix: �����
     * @return: 1: ����ɹ�, 0: ����ʧ��
     */
    int InvMatrix(AlvaMatrix* matrix, AlvaMatrix* inv_matrix);

    /**
     * @brief ��������, inv_matrix = matrix ^ -1
     * @param matrix: ����
     * @param inv_matrix: �����
     * @return: 1: ����ɹ�, 0: ����ʧ��
     */
    int asM4Inv(float* matrix, float* inv_matrix);
#define M4Inv(matrix, inv_matrix) asM4Inv(matrix->matrix, inv_matrix->matrix)
    /**
     * @brief ��������, inv_matrix = matrix ^ -1
     * @param matrix: ����
     * @param inv_matrix: �����
     * @return: 1: ����ɹ�, 0: ����ʧ��
     */
    int asM3Inv(float* matrix, float* inv_matrix);
#define M3Inv(matrix, inv_matrix) asM3Inv(matrix->matrix, inv_matrix->matrix)

    /**
     * @brief ��������, inv_matrix = matrix ^ -1
     * @param matrix: ����
     * @param inv_matrix: �����
     * @return: 1: ����ɹ�, 0: ����ʧ��
     */
    int asM2Inv(float* matrix, float* inv_matrix);
#define M2Inv(matrix, inv_matrix) asM2Inv(matrix->matrix, inv_matrix->matrix)

    /**
    * @brief ���������ʽ
    * @param matrix: ����
    * @return: ����ʽ
    */
    float DetMatrix(AlvaMatrix* matrix);

    /**
    * @brief ���������ʽ
    * @param matrix: ����
    * @return: ����ʽ
    */
    float asM4Det(float* matrix);
#define M4Det(matrix) asM4Det(matrix->matrix)

    /**
    * @brief ���������ʽ
    * @param matrix: ����
    * @return: ����ʽ
    */
    float asM3Det(float* matrix);
#define M3Det(matrix) asM3Det(matrix->matrix)

    /**
    * @brief ���������ʽ
    * @param matrix: ����
    * @return: ����ʽ
    */
    float asM2Det(float* matrix);
#define M2Det(matrix) asM2Det(matrix->matrix)
#ifdef __cplusplus
}
#endif
#endif