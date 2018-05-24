#ifndef _ALVA_DAMP_FILTER_H_
#define _ALVA_DAMP_FILTER_H_

#ifdef AlvaPublic
#include "Filter_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    void damp_init(Filter* filter, int dataNum, float* userData);

#ifdef __cplusplus
}
#endif

#endif