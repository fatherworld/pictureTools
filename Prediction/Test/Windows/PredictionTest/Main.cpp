#include "Config.h"

#include "stdio_fl.h"
#include "string_fl.h"

#define LOG_TAG "Main"
#include "Log.h"
#include "Dump.h"

#include "AndroidHelper.h"

#include "AlvaPrediction.h"

int readH(char* ptr, float* H) {
    char* p;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            p = strstr(ptr, ",");
            *p = '\0';
            sscanf_fl(ptr, "%f", &(H[i * 3 + j]));
            ptr = p + 1;
        }
        p = strstr(ptr, "\n");
        *p = '\0';

        sscanf_fl(ptr, "%f", &(H[i * 3 + 2]));
        ptr = p + 1;
    }

    LOGE("H:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            LOGE("%9.7f ", H[i * 3 + j]);
        }
        LOGE("\n");
    }
    return 0;
}

int main(int argc, char** argv) {
    char* firstValue = NULL;
    char* secondValue = NULL;
    char* thirdValue = NULL;

    int err = 0;
    if (argc == 1) {
        firstValue = (char*)malloc(2048);
        LOGE("please input first value name: \n");
        gets_fl(firstValue, 2048);
        secondValue = (char*)malloc(2048);
        LOGE("please input second value name: \n");
        gets_fl(secondValue, 2048);
        thirdValue = (char*)malloc(2048);
        LOGE("please input third value name: \n");
        gets_fl(thirdValue, 2048);
    }
    else if (argc == 2) {
        firstValue = (char*)malloc(2048);
        strcpy_fl(firstValue, 2048, argv[1]);
        secondValue = (char*)malloc(2048);
        LOGE("please input second value name: \n");
        gets_fl(secondValue, 2048);
        thirdValue = (char*)malloc(2048);
        LOGE("please input third value name: \n");
        gets_fl(thirdValue, 2048);
    }
    else if (argc == 3) {
        firstValue = (char*)malloc(2048);
        strcpy_fl(firstValue, 2048, argv[1]);
        secondValue = (char*)malloc(2048);
        strcpy_fl(secondValue, 2048, argv[2]);
        thirdValue = (char*)malloc(2048);
        LOGE("please input third value name: \n");
        gets_fl(thirdValue, 2048);
    }
    else if (argc == 4) {
        firstValue = (char*)malloc(2048);
        strcpy_fl(firstValue, 2048, argv[1]);
        secondValue = (char*)malloc(2048);
        strcpy_fl(secondValue, 2048, argv[2]);
        thirdValue = (char*)malloc(2048);
        strcpy_fl(thirdValue, 2048, argv[3]);
    }
    else {
        LOGE("parameter error, use: PredictionTest.exe first.txt second.txt third.txt\n");
        return -1;
    }

    char name[1024];
    int i = 0;
    void* file = NULL;

    system("del /Q .\\Dump\\*");

    float H1[9], H2[9], H3[9];
    char* packageData = NULL;
    int length = 0;

    readData(firstValue, Type_File, &packageData, &length);
    readH(packageData, H1);
    free(packageData);
    readData(secondValue, Type_File, &packageData, &length);
    readH(packageData, H2);
    free(packageData);
    readData(thirdValue, Type_File, &packageData, &length);
    readH(packageData, H3);
    free(packageData);
    
    Prediction mPred_ConstSpeed;
    AlvaInitPrediction(&mPred_ConstSpeed, Prediction_ConstSpeed, 9, 0.0f);
    AlvaPrediction(&mPred_ConstSpeed, H2);
    AlvaPrediction(&mPred_ConstSpeed, H3);

    sprintf_fl(name, 1024, "Dump/H_ConstSpeed.txt");
    file = start_filedump_temp(name, DUMP_CLEAR);
    for (i = 0; i < 9; i++) {
        log2file_temp(file, "%8.6f, ", H3[i]);
        if (i % 3 == 2)
            log2file_temp(file, "%s", "\n");
    }
    end_filedump_temp(file);

    readData(firstValue, Type_File, &packageData, &length);
    readH(packageData, H1);
    free(packageData);
    readData(secondValue, Type_File, &packageData, &length);
    readH(packageData, H2);
    free(packageData);
    readData(thirdValue, Type_File, &packageData, &length);
    readH(packageData, H3);
    free(packageData);

    Prediction mPred_Acce;
    AlvaInitPrediction(&mPred_Acce, Prediction_ConstAcceleration, 9, 0.0f);
    AlvaPrediction(&mPred_Acce, H1);
    AlvaPrediction(&mPred_Acce, H2);
    AlvaPrediction(&mPred_Acce, H3);

    sprintf_fl(name, 1024, "Dump/H_ConstAcceleration.txt");
    file = start_filedump_temp(name, DUMP_CLEAR);
    for (i = 0; i < 9; i++) {
        log2file_temp(file, "%8.6f, ", H3[i]);
        if (i % 3 == 2)
            log2file_temp(file, "%s", "\n");
    }
    end_filedump_temp(file);

    return 0;
}