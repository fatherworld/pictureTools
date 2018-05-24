#ifndef _ALVA_TRUNCATION_FILTER_H_
#define _ALVA_TRUNCATION_FILTER_H_

#ifdef AlvaPublic
#include "Filter_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    void truncation_init(Filter* filter, int dataNum, float* userData);

#ifdef __cplusplus
}
#endif

#endif