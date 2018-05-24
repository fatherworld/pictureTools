#include "Config.h"

#include "Eigen.h"
#define LOG_TAG "AlvaMatrix"
#include "Log.h"

#include <math.h>
#include "AlvaMatrix.h"

using namespace Eigen;
using namespace std;

void MulMatrix(AlvaMatrix* input_martix_left, AlvaMatrix* input_martix_right, AlvaMatrix* output_martix) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

    float *M_R = NULL, *M_L = NULL, *M_Res = NULL;
    int i = 0, j = 0, k = 0, offL = 0, offR = 0, off = 0;
    int row   = input_martix_left->rows,
        col   = input_martix_right->cols,
        inner = input_martix_left->cols;

	output_martix->cols = input_martix_right->cols;
	output_martix->rows = input_martix_left->rows;

    M_R   = input_martix_right->matrix;
    M_L   = input_martix_left->matrix;
    M_Res = output_martix->matrix;

    for(i = 0; i < row; i++) {
        for(j = 0; j < col; j++) {
            float temp = 0.0f;
            offR = j;
            for(k = 0; k < inner; k++) {
                temp += M_L[offL + k] * M_R[offR];
                offR += col;
            }
            M_Res[off++] = temp;
        }
        offL += inner;
    }
}

int asM4xM4(float M_L[16], float M_R[16], float M_Res[16]){

#undef FUNC_CODE
#define FUNC_CODE 0x02

    int i = 0, j = 0, k = 0, offL = 0, offR = 0, off = 0;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            float temp = 0.0f;
            offR = j;
            for(k = 0; k < 4; k++) {
                temp += M_L[offL + k] * M_R[offR];
                offR += 4;
            }
            M_Res[off++] = temp;
        }
        offL += 4;
    }

	return 0;
}

int asM3xM3(float M_L[9] , float M_R[9] , float M_Res[9] ) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    int i = 0, j = 0, k = 0, offL = 0, offR = 0, off = 0;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            float temp = 0.0f;
            offR = j;
            for(k = 0; k < 3; k++) {
                temp += M_L[offL + k] * M_R[offR];
                offR += 3;
            }
            M_Res[off++] = temp;
        }
        offL += 3;
    }

    return 0;
}

int asM2xM2(float M_L[4] , float M_R[4] , float M_Res[4] ) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    int i = 0, j = 0, k = 0, offL = 0, offR = 0, off = 0;
    for(i = 0; i < 2; i++) {
        for(j = 0; j < 2; j++) {
            float temp = 0.0f;
            offR = j;
            for(k = 0; k < 2; k++) {
                temp += M_L[offL + k] * M_R[offR];
                offR += 2;
            }
            M_Res[off++] = temp;
        }
        offL += 2;
    }

    return 0;
}

int asM4xV4(float M_L[16], float V[4], float M_Res[4]){

#undef FUNC_CODE
#define FUNC_CODE 0x03

    int i, j, off = 0;

    for(i = 0; i < 4; i++) {
        float temp = 0.0f;
        for(j = 0; j < 4; j++) {
            temp += M_L[off + j] * V[j];
        }
        M_Res[i] = temp;
        off += 4;
    }

	return 0;
}

int asM3xV3(float M_L[9], float V[3], float M_Res[3]){

#undef FUNC_CODE
#define FUNC_CODE 0x04

    int i, j, off = 0;

    for(i = 0; i < 3; i++) {
        float temp = 0.0f;
        for(j = 0; j < 3; j++) {
            temp += M_L[off + j] * V[j];
        }
        M_Res[i] = temp;
        off += 3;
    }

    return 0;
}

int asM2xV2(float M_L[4], float V[2], float M_Res[2]){

#undef FUNC_CODE
#define FUNC_CODE 0x05

    float v0 = V[0], v1 = V[1];

    M_Res[0] = M_L[0] * v0 + M_L[1] * v1;
    M_Res[1] = M_L[2] * v0 + M_L[3] * v1;

    return 0;
}

int asV4Normal(float V[4], float VNor[4]){

#undef FUNC_CODE
#define FUNC_CODE 0x06

	float temp = V[0] * V[0] + V[1] * V[1] + V[2] * V[2] + V[3] * V[3];
	temp = 1.0f / sqrtf(temp);
	VNor[0] = V[0] * temp;
	VNor[1] = V[1] * temp;
	VNor[2] = V[2] * temp;
	VNor[3] = V[3] * temp;

	return 0;
}

int asV3Normal(float V[3], float VNor[3]){

#undef FUNC_CODE
#define FUNC_CODE 0x07

	float temp = V[0] * V[0] + V[1] * V[1] + V[2] * V[2];
	temp = 1.0f / sqrtf(temp);
	VNor[0] = V[0] * temp;
	VNor[1] = V[1] * temp;
	VNor[2] = V[2] * temp;

	return 0;
}

int asV2Normal(float V[2], float VNor[2]){

#undef FUNC_CODE
#define FUNC_CODE 0x08

	float temp = V[0] * V[0] + V[1] * V[1];
	temp = 1.0f / sqrtf(temp);
	VNor[0] = V[0] * temp;
	VNor[1] = V[1] * temp;

	return 0;
}

#ifndef AlvaPublic
void printf_as_m(float* matrix, int rows, int cols) {

#undef FUNC_CODE
#define FUNC_CODE 0x09

    int i, j;

    LOGE("Matrix [%d x %d]: \n", rows, cols);

    for(j = 0; j < rows; j++) {
        for(i = 0; i < cols; i++) {
            LOGN("%10.6f ", matrix[j * cols + i]);
        }
        LOGN("\n");
    }
}
#endif

int isAsMatrixsEqual(float* matrix1, float* matrix2, int rows, int cols, float err) {

#undef FUNC_CODE
#define FUNC_CODE 0x0A

    int i, num = rows * cols;

    int equal = 1;

    for(i = 0; i < num; i++) {
        if(fabs(matrix1[i] - matrix2[i]) > err) {
            equal = 0;
            break;
        }
    }
    return equal;
}

int InvMatrix(AlvaMatrix* matrix, AlvaMatrix* inv_matrix) {

#undef FUNC_CODE
#define FUNC_CODE 0x0B

    int i = 0, j = 0;

    inv_matrix->rows = matrix->rows;
    inv_matrix->cols = matrix->cols;

    MatrixXf tM(matrix->rows, matrix->cols);
    for(j = 0; j < matrix->rows; j++) {
        for(i = 0; i < matrix->cols; i++) {
            tM(j, i) = matrix->matrix[j * matrix->cols + i];
        }
    }

    MatrixXf tIM = tM.inverse();
    for(j = 0; j < matrix->rows; j++) {
        for(i = 0; i < matrix->cols; i++) {
            inv_matrix->matrix[j * inv_matrix->cols + i] = tIM(j, i);
        }
    }

    return 1;
}

int asM4Inv(float* matrix, float* inv_matrix) {

#undef FUNC_CODE
#define FUNC_CODE 0x0C

    int i = 0, j = 0;

    MatrixXf tM(4, 4);
    for(j = 0; j < 4; j++) {
        for(i = 0; i < 4; i++) {
            tM(j, i) = matrix[j * 4 + i];
        }
    }

    MatrixXf tIM = tM.inverse();
    for(j = 0; j < 4; j++) {
        for(i = 0; i < 4; i++) {
            inv_matrix[j * 4 + i] = tIM(j, i);
        }
    }

    return 1;
}

int asM3Inv(float* matrix, float* inv_matrix) {

#undef FUNC_CODE
#define FUNC_CODE 0x0D

    float dDet = asM3Det(matrix);
    if(dDet == 0.0) {
        return 0;
    }
    float dInverseDet = 1.0f / dDet;
    inv_matrix[0] =  (matrix[4] * matrix[8] - matrix[5] * matrix[7]) * dInverseDet;
    inv_matrix[1] = -(matrix[1] * matrix[8] - matrix[2] * matrix[7]) * dInverseDet;
    inv_matrix[2] =  (matrix[1] * matrix[5] - matrix[2] * matrix[4]) * dInverseDet;

    inv_matrix[3] = -(matrix[3] * matrix[8] - matrix[5] * matrix[6]) * dInverseDet;
    inv_matrix[4] =  (matrix[0] * matrix[8] - matrix[2] * matrix[6]) * dInverseDet;
    inv_matrix[5] = -(matrix[0] * matrix[5] - matrix[2] * matrix[3]) * dInverseDet;

    inv_matrix[6] =  (matrix[3] * matrix[7] - matrix[4] * matrix[6]) * dInverseDet;
    inv_matrix[7] = -(matrix[0] * matrix[7] - matrix[1] * matrix[6]) * dInverseDet;
    inv_matrix[8] =  (matrix[0] * matrix[4] - matrix[1] * matrix[3]) * dInverseDet;

    return 1;
}

int asM2Inv(float* matrix, float* inv_matrix) {

#undef FUNC_CODE
#define FUNC_CODE 0x0E

    float dDet = asM2Det(matrix);
    if(dDet == 0.0) {
        return 0;
    }
    float dInverseDet = 1.0f / dDet;
    inv_matrix[0] =  matrix[3] * dInverseDet;
    inv_matrix[1] = -matrix[1] * dInverseDet;

    inv_matrix[2] = -matrix[2] * dInverseDet;
    inv_matrix[3] =  matrix[0] * dInverseDet;

    return 1;
}

float DetMatrix(AlvaMatrix* matrix) {

#undef FUNC_CODE
#define FUNC_CODE 0x0F

    int i = 0, j = 0;

    MatrixXf tM(matrix->rows, matrix->cols);
    for(j = 0; j < matrix->rows; j++) {
        for(i = 0; i < matrix->cols; i++) {
            tM(j, i) = matrix->matrix[j * matrix->cols + i];
        }
    }

    return tM.determinant();
}

float asM4Det(float* matrix) {

#undef FUNC_CODE
#define FUNC_CODE 0x10

    int i = 0, j = 0;

    MatrixXf tM(4, 4);
    for(j = 0; j < 4; j++) {
        for(i = 0; i < 4; i++) {
            tM(j, i) = matrix[j * 4 + i];
        }
    }

    return tM.determinant();
}

float asM3Det(float* matrix) {

#undef FUNC_CODE
#define FUNC_CODE 0x11
    return
        matrix[0] * (matrix[4] * matrix[8] - matrix[5] * matrix[7]) +
        matrix[1] * (matrix[5] * matrix[6] - matrix[3] * matrix[8]) +
        matrix[2] * (matrix[3] * matrix[7] - matrix[4] * matrix[6]);
}

float asM2Det(float* matrix) {

#undef FUNC_CODE
#define FUNC_CODE 0x12
    return
        matrix[0] * matrix[3] - matrix[1] * matrix[2];
}