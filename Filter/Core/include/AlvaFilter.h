#ifndef _ALVA_FILTER_H_
#define _ALVA_FILTER_H_

#ifdef AlvaPublic
#include "Filter_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum _FILTER_TYPE_ {
        Filter_None             = -1,
        Filter_Threthold            , //ÓòÖµÂË²¨
        Filter_Damp                 , //×èÄáÂË²¨
        Filter_Truncation           , //½Ø¶ÏÂË²¨
        Filter_ConditionDamp        , //´øÓòÖµµÄ×èÄáÂË²¨
    }Filter_Type;

    typedef struct _FILTER_DATA_ {
        Filter_Type type;
        int numGroup;
        void* data;
        int frameIndex;
        void(*init) (struct _FILTER_DATA_*, int, float*);
        void(*filter) (struct _FILTER_DATA_*, float*);
        void(*reset) (struct _FILTER_DATA_*);
        void(*unit) (struct _FILTER_DATA_* filter);
    }Filter;

    void AlvaInitFilter(Filter* filter, Filter_Type type, int dataNum, float* userData);

    void AlvaFilter(Filter* filter, float* data);

    void AlvaResetFilter(Filter* filter);

    void AlvaUnitFilter(Filter* filter);

#ifdef __cplusplus
}
#endif

#endif