#ifndef _ALVA__CONDITION_DAMP_FILTER_H_
#define _ALVA__CONDITION_DAMP_FILTER_H_

#ifdef AlvaPublic
#include "Filter_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    void condition_damp_init(Filter* filter, int dataNum, float* userData);

#ifdef __cplusplus
}
#endif

#endif