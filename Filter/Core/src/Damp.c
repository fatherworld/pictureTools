#include "Config.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "AlvaFilter.h"

#include "Damp.h"

#undef FILE_NUM
#define FILE_NUM 0xE300

typedef struct _THRESHOLD_FILTER_DATA_ {
    float* lastValue[2];
    float* alpha;
    float* beta;
}Threshold_Filter_Data;

static 
void 
initMemory(Filter* filter) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

    Threshold_Filter_Data* ptr = (Threshold_Filter_Data*)calloc(1, sizeof(Threshold_Filter_Data));

    ptr->alpha = (float*)calloc(filter->numGroup, sizeof(float));
    ptr->beta = (float*)calloc(filter->numGroup, sizeof(float));
    ptr->lastValue[0] = (float*)calloc(filter->numGroup, sizeof(float));
    ptr->lastValue[1] = (float*)calloc(filter->numGroup, sizeof(float));

    filter->data = (void*)ptr;
}

static 
void 
damp(Filter* filter, float* data) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    Threshold_Filter_Data* ptr = (Threshold_Filter_Data*)filter->data;
    int i = 0;

    if (filter->frameIndex < 2) {
        memcpy(ptr->lastValue[filter->frameIndex], data, filter->numGroup * sizeof(float));
        filter->frameIndex++;
    }
    else {
        for (i = 0; i < filter->numGroup; i++) {
            data[i] = ptr->alpha[i] * data[i] 
                    + ptr->lastValue[1][i] * (2.0f - ptr->alpha[i] - ptr->beta[i]) 
                    + ptr->lastValue[0][i] * (ptr->beta[i] - 1.0f);
        }
        memcpy(ptr->lastValue[0], ptr->lastValue[1], filter->numGroup * sizeof(float));
        memcpy(ptr->lastValue[1], data, filter->numGroup * sizeof(float));
    }
}

static
void 
unit(Filter* filter) {

#undef FUNC_CODE
#define FUNC_CODE 0x03

    if (NULL != filter) {
        Threshold_Filter_Data* ptr = (Threshold_Filter_Data*)filter->data;
        if (NULL != ptr) {
            if (NULL != ptr->lastValue[0]) {
                free(ptr->lastValue[0]);
            }
            if (NULL != ptr->lastValue[1]) {
                free(ptr->lastValue[1]);
            }
            if (NULL != ptr->alpha) {
                free(ptr->alpha);
            }
            if (NULL != ptr->beta) {
                free(ptr->beta);
            }
            free(ptr);
        }
        filter->type = Filter_None;
        filter->numGroup = 0;
    }
}

static
void
reset(Filter* filter) {

#undef FUNC_CODE
#define FUNC_CODE 0x04

    filter->frameIndex = 0;
}

void damp_init(Filter* filter, int dataNum, float* userData) {

#undef FUNC_CODE
#define FUNC_CODE 0x05

    int i = 0;

    filter->type = Filter_Damp;
    filter->numGroup = dataNum;
    filter->frameIndex = 0;

    filter->filter = damp;
    filter->unit = unit;
    filter->reset = reset;

    initMemory(filter);

    Threshold_Filter_Data* ptr = (Threshold_Filter_Data*)filter->data;
    for (i = 0; i < filter->numGroup; i++) {
        ptr->alpha[i] = userData[0];
        ptr->beta[i] = 2 * (float)sqrt(ptr->alpha[i]) - ptr->alpha[i];
    }
}
