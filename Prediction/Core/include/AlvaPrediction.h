#ifndef _ALVA_Prediction_H_
#define _ALVA_Prediction_H_

#ifdef AlvaPublic
#include "Prediction_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum _Prediction_TYPE_ {
        Prediction_None            = -1,
        Prediction_ConstSpeed          ,
        Prediction_ConstAcceleration   ,
        Prediction_ConstSpeedH         ,
    }Prediction_Type;

    typedef struct _Prediction_DATA_ {
        Prediction_Type type;
        int numGroup;
        void* data;
        int frameIndex;
        void(*init) (struct _Prediction_DATA_*, int, float);
        void(*prediction) (struct _Prediction_DATA_*, float*);
        void(*reset) (struct _Prediction_DATA_*);
        void(*unit) (struct _Prediction_DATA_* filter);
    }Prediction;

    void AlvaInitPrediction(Prediction* prediction, Prediction_Type type, int dataNum, float userData);

    void AlvaPrediction(Prediction* prediction, float* data);

    void AlvaResetPrediction(Prediction* prediction);

    void AlvaUnitPrediction(Prediction* prediction);

#ifdef __cplusplus
}
#endif

#endif