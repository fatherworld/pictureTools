#ifndef _ALVA_CONSTSPEEDH_PREDICTION_H_
#define _ALVA_CONSTSPEEDH_PREDICTION_H_

#ifdef AlvaPublic
#include "Prediction_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    void constSpeedH_init(Prediction* prediction, int dataNum, float userData);

#ifdef __cplusplus
}
#endif

#endif