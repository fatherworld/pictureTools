#include "Config.h"

#include "AlvaPrediction.h"

#include "ConstSpeed.h"
#include "ConstAcceleration.h"
#include "ConstSpeedH.h"

#undef FILE_NUM
#define FILE_NUM 0xB800

void AlvaInitPrediction(Prediction* prediction, Prediction_Type type, int dataNum, float userData) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

    switch (type){
        case Prediction_ConstSpeed:
            prediction->init = constSpeed_init;
            prediction->init(prediction, dataNum, userData);
            break;
        case Prediction_ConstAcceleration:
            prediction->init = constAcceleration_init;
            prediction->init(prediction, dataNum, userData);
            break;
        case Prediction_ConstSpeedH:
            prediction->init = constSpeedH_init;
            prediction->init(prediction, dataNum, userData);
            break;
        case Prediction_None:
        default:
            prediction->type = Prediction_None;
            prediction->data = 0;
            prediction->numGroup = 0;
            prediction->frameIndex = 0;
            prediction->init = 0;
            prediction->prediction = 0;
            prediction->unit = 0;
            prediction->reset = 0;
            //every implementments must do following opt:
            //prediction->init = initfunction;
            //prediction->init(prediction, dataNum, userData);
            break;
    }
}

void AlvaPrediction(Prediction* prediction, float* data) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    if(prediction->prediction)
        prediction->prediction(prediction, data);
}

void AlvaResetPrediction(Prediction* prediction) {

#undef FUNC_CODE
#define FUNC_CODE 0x03

    if(prediction->reset)
        prediction->reset(prediction);
    prediction->frameIndex = 0;
}

void AlvaUnitPrediction(Prediction* prediction) {

#undef FUNC_CODE
#define FUNC_CODE 0x04

    if(prediction->unit)
        prediction->unit(prediction);
    prediction->init = 0;
}