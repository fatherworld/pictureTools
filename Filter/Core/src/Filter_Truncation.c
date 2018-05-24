#include "Config.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "AlvaFilter.h"

#include "Filter_Truncation.h"

#undef FILE_NUM
#define FILE_NUM 0xE400

typedef struct _TRUNCATION_FILTER_DATA_ {
    float* threshold;
}Truncation_Filter_Data;

static
void
initMemory(Filter* filter) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

    Truncation_Filter_Data* ptr = (Truncation_Filter_Data*)malloc(sizeof(Truncation_Filter_Data));

    ptr->threshold = (float*)calloc(filter->numGroup, sizeof(float));

    filter->data = (void*)ptr;
}

static
void
truncation(Filter* filter, float* data) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    Truncation_Filter_Data* ptr = (Truncation_Filter_Data*)filter->data;
    int i = 0;

    for (i = 0; i < filter->numGroup; i++) {
        data[i] = ((int)(data[i] * ptr->threshold[i])) / ptr->threshold[i];
    }
}

static
void
unit(Filter* filter) {

#undef FUNC_CODE
#define FUNC_CODE 0x03

    if (NULL != filter) {
        Truncation_Filter_Data* ptr = (Truncation_Filter_Data*)filter->data;
        if (NULL != ptr) {
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

}

void truncation_init(Filter* filter, int dataNum, float* userData) {

#undef FUNC_CODE
#define FUNC_CODE 0x05

    int i = 0;

    filter->type = Filter_Truncation;
    filter->numGroup = dataNum;

    filter->filter = truncation;
    filter->unit = unit;
    filter->reset = reset;

    initMemory(filter);

    Truncation_Filter_Data* ptr = (Truncation_Filter_Data*)filter->data;
    for (i = 0; i < filter->numGroup; i++) {
        ptr->threshold[i] = userData[0];
    }
}
