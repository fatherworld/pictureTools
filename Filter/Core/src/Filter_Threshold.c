#include "Config.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "AlvaFilter.h"

#include "Threshold.h"

#undef FILE_NUM
#define FILE_NUM 0xE200

typedef struct _THRESHOLD_FILTER_DATA_ {
    float* threshold;
    float* lastValue;
}Threshold_Filter_Data;

static 
void 
initMemory(Filter* filter) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

    Threshold_Filter_Data* ptr = (Threshold_Filter_Data*)malloc(sizeof(Threshold_Filter_Data));

    ptr->threshold = (float*)calloc(filter->numGroup, sizeof(float));
    ptr->lastValue = (float*)calloc(filter->numGroup, sizeof(float));
    
    filter->data = (void*)ptr;
}

static
void 
threshold(Filter* filter, float* data) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    Threshold_Filter_Data* ptr = (Threshold_Filter_Data*)filter->data;
    int i = 0;

    for (i = 0; i < filter->numGroup; i++) {
        if (fabs(data[i] - ptr->lastValue[i]) < ptr->threshold[i])
            data[i] = ptr->lastValue[i];
        else
            ptr->lastValue[i] = data[i];
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
            if (NULL != ptr->lastValue) {
                free(ptr->lastValue);
            }
            if (NULL != ptr->threshold) {
                free(ptr->threshold);
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

void threshold_init(Filter* filter, int dataNum, float* userData) {

#undef FUNC_CODE
#define FUNC_CODE 0x05

    int i = 0;

    filter->type = Filter_Threthold;
    filter->numGroup = dataNum;

    filter->filter = threshold;
    filter->unit = unit;
    filter->reset = reset;

    initMemory(filter);

    Threshold_Filter_Data* ptr = (Threshold_Filter_Data*)filter->data;
    for (i = 0; i < filter->numGroup; i++) {
        ptr->threshold[i] = userData[0];
    }
}
