#include "Config.h"

#include "AlvaFilter.h"

#include "Damp.h"
#include "Threshold.h"
#include "Filter_Truncation.h"
#include "Filter_ConditionDamp.h"

#undef FILE_NUM
#define FILE_NUM 0xE100

void AlvaInitFilter(Filter* filter, Filter_Type type, int dataNum, float* userData) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

    switch (type)
    {
        case Filter_Threthold:
            filter->init = threshold_init;
            filter->init(filter, dataNum, userData);
            break;
        case Filter_Damp:
            filter->init = damp_init;
            filter->init(filter, dataNum, userData);
            break;
        case Filter_Truncation:
            filter->init = truncation_init;
            filter->init(filter, dataNum, userData);
            break;
        case Filter_ConditionDamp:
            filter->init = condition_damp_init;
            filter->init(filter, dataNum, userData);
            break;
        case Filter_None:
        default:
            break;
    }
}

void AlvaFilter(Filter* filter, float* data) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    filter->filter(filter, data);
}

void AlvaResetFilter(Filter* filter) {

#undef FUNC_CODE
#define FUNC_CODE 0x03

    filter->reset(filter);
    filter->frameIndex = 0;
}

void AlvaUnitFilter(Filter* filter) {

#undef FUNC_CODE
#define FUNC_CODE 0x04

    filter->unit(filter);
}