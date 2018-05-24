#include "Config.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "AlvaPrediction.h"

#include "ConstSpeed.h"

#undef FILE_NUM
#define FILE_NUM 0xB900

typedef struct _CONST_SPEED_PREDICTION_DATA_ {
    float* lastValue[2];
}CONST_SPEED_PREDICTION_DATA;

static
void
predInitMemory(Prediction* prediction) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

    CONST_SPEED_PREDICTION_DATA* ptr = (CONST_SPEED_PREDICTION_DATA*)calloc(1, sizeof(CONST_SPEED_PREDICTION_DATA));

    ptr->lastValue[0] = (float*)calloc(prediction->numGroup, sizeof(float));
    ptr->lastValue[1] = (float*)calloc(prediction->numGroup, sizeof(float));

    prediction->data = (void*)ptr;
}

static
void
constSpeed(Prediction* prediction, float* data) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    CONST_SPEED_PREDICTION_DATA* ptr = (CONST_SPEED_PREDICTION_DATA*)prediction->data;
    int i = 0;

    if (prediction->frameIndex < 1) {
        memcpy(ptr->lastValue[1], data, prediction->numGroup * sizeof(float));
        prediction->frameIndex++;
    }
    else {
        memcpy(ptr->lastValue[0], ptr->lastValue[1], prediction->numGroup * sizeof(float));
        memcpy(ptr->lastValue[1], data, prediction->numGroup * sizeof(float));
        for (i = 0; i < prediction->numGroup; i++) {
            data[i] = 2 * ptr->lastValue[1][i] - ptr->lastValue[0][i];
        }
    }
}

static
void
predUnit(Prediction* prediction) {

#undef FUNC_CODE
#define FUNC_CODE 0x03

    if (NULL != prediction) {
        CONST_SPEED_PREDICTION_DATA* ptr = (CONST_SPEED_PREDICTION_DATA*)prediction->data;
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
#define FUNC_CODE 0x04

    prediction->frameIndex = 0;
}

void constSpeed_init(Prediction* prediction, int dataNum, float userData) {

#undef FUNC_CODE
#define FUNC_CODE 0x05

    prediction->type = Prediction_ConstSpeed;
    prediction->numGroup = dataNum;
    prediction->frameIndex = 0;

    prediction->prediction = constSpeed;
    prediction->unit = predUnit;
    prediction->reset = predReset;

    predInitMemory(prediction);
}
