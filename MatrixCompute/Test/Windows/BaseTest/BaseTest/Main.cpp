#include "Config.h"

#include <math.h>

#define LOG_TAG "Main"
#include "Log.h"

#include "AlvaMatrix.h"

int main(int argc, char** argv) {

    //定义部分
    float M_2[4 ] = {1.0f, 2.0f,
                     2.0f, 3.0f};
    float M_3[9 ] = {1.0f, 2.0f, 8.0f,
                     4.0f, 1.0f, 6.0f,
                     7.0f, 3.0f, 5.0f};
    float M_4[16] = {1.0f, 2.0f, 3.0f, 4.0f,
                     4.0f, 8.0f, 6.0f, 2.0f,
                     7.0f, 8.0f, 9.0f, 6.0f,
                     1.0f, 6.0f, 3.0f, 3.0f};
    float M_5[25] = {1.0f, 2.0f, 3.0f, 1.0f,  2.0f,
                     4.0f, 8.0f, 6.0f, 3.0f, 10.0f,
                     7.0f, 5.0f, 9.0f, 1.0f,  4.0f,
                     2.0f, 3.0f, 4.0f, 6.0f, 10.0f,
                     5.0f, 7.0f, 8.0f, 1.0f,  8.0f};

    float V_2[2] = {1.0f, 2.0f};
    float V_3[3] = {1.0f, 2.0f, 3.0f};
    float V_4[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    float V_5[5] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

    //结果部分
    float M2_M2[ 4] = {5.0f,  8.0f,
                       8.0f, 13.0f};
    float M3_M3[ 9] = {65.0f, 28.0f, 60.0f,
                       50.0f, 27.0f, 68.0f,
                       54.0f, 32.0f, 99.0f};
    float M4_M4[16] = { 34.0f,  66.0f,  54.0f,  38.0f,
                        80.0f, 132.0f, 120.0f,  74.0f,
                       108.0f, 186.0f, 168.0f, 116.0f,
                        49.0f,  92.0f,  75.0f,  43.0f};
    float M5_M5[25] = { 42.0f,  50.0f,  62.0f, 18.0f,  60.0f,
                       134.0f, 181.0f, 206.0f, 62.0f, 222.0f,
                       112.0f, 130.0f, 168.0f, 41.0f, 142.0f,
                       104.0f, 136.0f, 164.0f, 61.0f, 190.0f,
                       131.0f, 165.0f, 197.0f, 48.0f, 186.0f};

    float M2_V2[2] = {5.0f, 8.0f};
    float M3_V3[3] = {29.0f, 24.0f, 28.0f};
    float M4_V4[4] = {30.0f, 46.0f, 74.0f, 34.0f};
    float M5_V5[5] = {28.0f, 100.0f, 68.0f, 94.0f, 87.0f};

    float M2_Inv[ 4] = {-3.0f,  2.0f,
                         2.0f, -1.0f};
    float M3_Inv[ 9] = {-0.1831f,  0.1972f,  0.0563f,
                         0.3099f, -0.7183f,  0.3662f,
                         0.0704f,  0.1549f, -0.0986f};
    float M4_Inv[16] = {-0.7778f, -0.6111f,  0.5556f,  0.3333f,
                        -0.2778f, -0.1111f,  0.0556f,  0.3333f,
                         0.9259f,  0.8704f, -0.5185f, -0.7778f,
                        -0.1111f, -0.4444f,  0.2222f,  0.3333f};
    float M5_Inv[25] = {-0.6770f,  0.1429f,  0.2795f,  0.0248f, -0.1801f,
                         0.2950f,  0.4286f,  0.0342f, -0.2081f, -0.3665f,
                         0.4845f, -0.2857f, -0.0807f,  0.0373f,  0.2298f,
                         0.3634f,  0.2857f,  0.1894f,  0.0280f, -0.5776f,
                        -0.3649f, -0.2143f, -0.1475f,  0.1258f,  0.4006f};

    float M2_Det = -1.0f;
    float M3_Det = 71.0f;
    float M4_Det = -108.0f;
    float M5_Det = -644.0f;

    float result[25] = {0.0f};

    int right = 0, error = 0, err = 0;

    AlvaMatrix mL, mR, mRes;

    {
        LOGE("*****************************************************\n");
        LOGE("Check Multiply(Matrix x Vector):\n");
        LOGE("*****************************************************\n");
        asM2xV2(M_2, V_2, result);
        printf_as_m(M_2, 2, 2);
        LOGE("Multiply\n");
        printf_as_m(V_2, 1, 2);
        printf_as_m(result, 1, 2);
        err = isAsMatrixsEqual(result, M2_V2, 1, 2, 0.001f);
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
        asM3xV3(M_3, V_3, result);
        printf_as_m(M_3, 3, 3);
        LOGE("Multiply\n");
        printf_as_m(V_3, 1, 3);
        printf_as_m(result, 1, 3);
        err = isAsMatrixsEqual(result, M3_V3, 1, 3, 0.001f);
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
        asM4xV4(M_4, V_4, result);
        printf_as_m(M_4, 4, 4);
        LOGE("Multiply\n");
        printf_as_m(V_4, 1, 4);
        printf_as_m(result, 1, 4);
        err = isAsMatrixsEqual(result, M4_V4, 1, 4, 0.001f);
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
          mL.cols = 5;   mL.rows = 5;   mL.matrix = M_5;
          mR.cols = 1;   mR.rows = 5;   mR.matrix = V_5;
        mRes.cols = 1; mRes.rows = 5; mRes.matrix = result;
        MulMatrix(&mL, &mR, &mRes);
        printf_as_m(M_5, 5, 5);
        LOGE("Multiply\n");
        printf_as_m(V_5, 1, 5);
        printf_as_m(result, 1, 5);
        err = isAsMatrixsEqual(result, M5_V5, 1, 5, 0.001f);
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
    }

    {
        LOGE("*****************************************************\n");
        LOGE("Check Multiply(Matrix x Matrix):\n");
        LOGE("*****************************************************\n");
        mL.cols = 2;   mL.rows = 2;   mL.matrix = M_2;
        mRes.cols = 2; mRes.rows = 2; mRes.matrix = result;
        MulMatrix(&mL, &mL, &mRes);
        printf_as_m(M_2, 2, 2);
        LOGE("Multiply\n");
        printf_as_m(M_2, 2, 2);
        printf_as_m(result, 2, 2);
        err = isAsMatrixsEqual(result, M2_M2, 2, 2, 0.001f);
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
        mL.cols = 3;   mL.rows = 3;   mL.matrix = M_3;
        mRes.cols = 3; mRes.rows = 3; mRes.matrix = result;
        MulMatrix(&mL, &mL, &mRes);
        printf_as_m(M_3, 3, 3);
        LOGE("Multiply\n");
        printf_as_m(M_3, 3, 3);
        printf_as_m(result, 3, 3);
        err = isAsMatrixsEqual(result, M3_M3, 3, 3, 0.001f);
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
        mL.cols = 4;   mL.rows = 4;   mL.matrix = M_4;
        mRes.cols = 4; mRes.rows = 4; mRes.matrix = result;
        MulMatrix(&mL, &mL, &mRes);
        printf_as_m(M_4, 4, 4);
        LOGE("Multiply\n");
        printf_as_m(M_4, 4, 4);
        printf_as_m(result, 4, 4);
        err = isAsMatrixsEqual(result, M4_M4, 4, 4, 0.001f);
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
        mL.cols = 5;   mL.rows = 5;   mL.matrix = M_5;
        mRes.cols = 5; mRes.rows = 5; mRes.matrix = result;
        MulMatrix(&mL, &mL, &mRes);
        printf_as_m(M_5, 5, 5);
        LOGE("Multiply\n");
        printf_as_m(M_5, 5, 5);
        printf_as_m(result, 5, 5);
        err = isAsMatrixsEqual(result, M5_M5, 5, 5, 0.001f);
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
    }

    {
        LOGE("*****************************************************\n");
        LOGE("Check Determinant(det(Matrix)):\n");
        LOGE("*****************************************************\n");
        result[0] = asM2Det(M_2);
        printf_as_m(M_2, 2, 2);
        LOGE("det\n");
        LOGE("%f\n", result[0]);
        err = fabs(M2_Det - result[0]) < 0.001f ? 1 : 0;
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
        result[0] = asM3Det(M_3);
        printf_as_m(M_3, 3, 3);
        LOGE("det\n");
        LOGE("%f\n", result[0]);
        err = fabs(M3_Det - result[0]) < 0.001f ? 1 : 0;
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
        result[0] = asM4Det(M_4);
        printf_as_m(M_4, 4, 4);
        LOGE("det\n");
        LOGE("%f\n", result[0]);
        err = fabs(M4_Det - result[0]) < 0.001f ? 1 : 0;
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
        mL.cols = 5;   mL.rows = 5;   mL.matrix = M_5;
        result[0] = DetMatrix(&mL);
        printf_as_m(M_5, 5, 5);
        LOGE("det\n");
        LOGE("%f\n", result[0]);
        err = fabs(M5_Det - result[0]) < 0.001f ? 1 : 0;
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
    }

    {
        LOGE("*****************************************************\n");
        LOGE("Check Inverse(inv(Matrix)):\n");
        LOGE("*****************************************************\n");
        asM2Inv(M_2, result);
        printf_as_m(M_2, 2, 2);
        LOGE("Inv\n");
        printf_as_m(result, 2, 2);
        err = isAsMatrixsEqual(result, M2_Inv, 2, 2, 0.001f);
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
        asM3Inv(M_3, result);
        printf_as_m(M_3, 3, 3);
        LOGE("Inv\n");
        printf_as_m(result, 3, 3);
        err = isAsMatrixsEqual(result, M3_Inv, 3, 3, 0.001f);
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
        asM4Inv(M_4, result);
        printf_as_m(M_4, 4, 4);
        LOGE("Inv\n");
        printf_as_m(result, 4, 4);
        err = isAsMatrixsEqual(result, M4_Inv, 4, 4, 0.001f);
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
        mL.cols = 5;   mL.rows = 5;   mL.matrix = M_5;
        mRes.cols = 5; mRes.rows = 5; mRes.matrix = result;
        InvMatrix(&mL, &mRes);
        printf_as_m(M_5, 5, 5);
        LOGE("Inv\n");
        printf_as_m(result, 5, 5);
        err = isAsMatrixsEqual(result, M5_Inv, 5, 5, 0.001f);
        right += err;
        error += !err;
        LOGE("Is %s\n", err ? "right" : "error");
        LOGE("-----------------------------------------------------\n");
    }

    LOGE("Summary: %d right, %d wrong\n", right, error);

    return getchar();
}