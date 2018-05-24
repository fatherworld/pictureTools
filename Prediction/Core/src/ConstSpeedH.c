#include "Config.h"

#include <math.h>
#include "stdlib_fl.h"
#include "string_fl.h"
#include "stdio_fl.h"
#include <stddef.h>

#define LOG_TAG "ConstSpeedH"
#include "Log.h"
#include "ErrorTools.h"

#include "AlvaPrediction.h"

#include "ConstSpeedH.h"

#undef FILE_NUM
#define FILE_NUM 0xBB00

typedef struct _CONST_SPEED_H_PREDICTION_DATA_ {
    float* lastValue[2];
    float H_1[9];
    float H_T[9];
}CONST_SPEED_H_PREDICTION_DATA;

static
int 
Hinverse_CSH(float* H, float* Hinverse) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

    float detH = H[0] * H[4] * H[8] +
                 H[3] * H[7] * H[2] +
                 H[6] * H[1] * H[5] -
                 H[6] * H[4] * H[2] -
                 H[7] * H[5] * H[0] -
                 H[8] * H[3] * H[1];

    float AdjH[9] = { 0.0f }; //°éËæ¾ØÕó

    int i;

    if (0 == detH)
        OWN_ERROR_RETURN(0x001, "detH is 0");

    AdjH[(1 - 1) * 3 + (1 - 1)] = /*pow(-1, 1 + 1)*/ 1 * (H[4] * H[8] - H[5] * H[7]);
    AdjH[(2 - 1) * 3 + (1 - 1)] = /*pow(-1, 2 + 1)*/-1 * (H[3] * H[8] - H[5] * H[6]);
    AdjH[(3 - 1) * 3 + (1 - 1)] = /*pow(-1, 3 + 1)*/ 1 * (H[3] * H[7] - H[4] * H[6]);

    AdjH[(1 - 1) * 3 + (2 - 1)] = /*pow(-1, 1 + 2)*/-1 * (H[1] * H[8] - H[2] * H[7]);
    AdjH[(2 - 1) * 3 + (2 - 1)] = /*pow(-1, 2 + 2)*/ 1 * (H[0] * H[8] - H[2] * H[6]);
    AdjH[(3 - 1) * 3 + (2 - 1)] = /*pow(-1, 3 + 2)*/-1 * (H[0] * H[7] - H[1] * H[6]);

    AdjH[(1 - 1) * 3 + (3 - 1)] = /*pow(-1, 1 + 3)*/ 1 * (H[1] * H[5] - H[2] * H[4]);
    AdjH[(2 - 1) * 3 + (3 - 1)] = /*pow(-1, 2 + 3)*/-1 * (H[0] * H[5] - H[2] * H[3]);
    AdjH[(3 - 1) * 3 + (3 - 1)] = /*pow(-1, 3 + 3)*/ 1 * (H[0] * H[4] - H[1] * H[3]);

    for (i = 0; i < 9; i++) {
        Hinverse[i] = AdjH[i] / detH;
    }

    return 0;
}

static 
void
Product_CSH(float* a, float* b, float* o) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    int r, c, i;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            for (i = 0; i < 3; i++) {
                o[r * 3 + c] += a[r * 3 + i] * b[i * 3 + c];
            }
        }
    }
}

static
void
predInitMemory(Prediction* prediction) {

#undef FUNC_CODE
#define FUNC_CODE 0x03

    CONST_SPEED_H_PREDICTION_DATA* ptr = (CONST_SPEED_H_PREDICTION_DATA*)calloc(1, sizeof(CONST_SPEED_H_PREDICTION_DATA));

    ptr->lastValue[0] = (float*)calloc(prediction->numGroup, sizeof(float));
    ptr->lastValue[1] = (float*)calloc(prediction->numGroup, sizeof(float));

    prediction->data = (void*)ptr;
}

static
void
constSpeedH(Prediction* prediction, float* data) {

#undef FUNC_CODE
#define FUNC_CODE 0x04

    CONST_SPEED_H_PREDICTION_DATA* ptr = (CONST_SPEED_H_PREDICTION_DATA*)prediction->data;

    if (prediction->frameIndex < 1) {
        memcpy(ptr->lastValue[1], data, prediction->numGroup * sizeof(float));
        prediction->frameIndex++;
    }
    else {
        memcpy(ptr->lastValue[0], ptr->lastValue[1], prediction->numGroup * sizeof(float));
        memcpy(ptr->lastValue[1], data, prediction->numGroup * sizeof(float));

        if (0 != Hinverse_CSH(ptr->lastValue[0], ptr->H_1)) {
            return;
        }

        Product_CSH(ptr->lastValue[1], ptr->H_1, ptr->H_T);
        Product_CSH(ptr->H_T, ptr->lastValue[1], data);
    }
}

static
void
predUnit(Prediction* prediction) {

#undef FUNC_CODE
#define FUNC_CODE 0x05

    if (NULL != prediction) {
        CONST_SPEED_H_PREDICTION_DATA* ptr = (CONST_SPEED_H_PREDICTION_DATA*)prediction->data;
        if (NULL != ptr) {
            if (NULL != ptr->lastValue[0]) {
                free(ptr->lastValue[0]);
            }
            if (NULL != ptr->lastValue[1]) {
                free(ptr->lastValue[1]);
            }
            free(ptr);
        }
        prediction->type = Prediction_None;
        prediction->data = NULL;
        prediction->numGroup = 0;
        prediction->frameIndex = 0;
        prediction->prediction = NULL;
        prediction->unit = NULL;
        prediction->reset = NULL;
    }
}

static
void
predReset(Prediction* prediction) {

#undef FUNC_CODE
#define FUNC_CODE 0x06

    prediction->frameIndex = 0;
}

void constSpeedH_init(Prediction* prediction, int dataNum, float userData) {

#undef FUNC_CODE
#define FUNC_CODE 0x07

    prediction->type = Prediction_ConstSpeedH;
    prediction->numGroup = dataNum;
    prediction->frameIndex = 0;

    prediction->prediction = constSpeedH;
    prediction->unit = predUnit;
    prediction->reset = predReset;

    predInitMemory(prediction);
}
