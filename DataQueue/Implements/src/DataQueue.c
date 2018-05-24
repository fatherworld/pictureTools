#include "Config.h"

#include <stdlib.h>
#include <stdio.h>

#include "stdio_fl.h"
#include "string_fl.h"
#include "memory_fl.h"
#include "AlvaThread.h"

#define LOG_TAG "DataQueue"
#include "Log.h"
#include "Dump.h"
#include "ErrorTools.h"

#include "DataQueueInterface.h"
#include "DataQueue.h"

#undef FILE_NUM
#define FILE_NUM 0xD800

#define AlgorithmNum 5                               //算法个数

typedef enum _FrameStatus_ {
    FS_Ready = 0,
    FS_Busy  = 1
}FrameStatus;

typedef enum _FrameInitStatus_ {
    FIS_Free   = 0,
    FIS_Inited = 1
}FrameInitStatus;

typedef enum _ThreadMessage_ {
    TM_None = 0,
    TM_Stop = 1
}ThreadMessage;

typedef enum _FrameDealStatus_ {
    FDS_Free     = 0,
    FDS_Used     = 1,
    FDS_Perfed   = 2,
    FDS_Update   = 3,
    FDS_Perfing  = 4,
    FDS_Needperf = 5
}FrameDealStatus;

typedef enum _AlgorithmActive_ {
    AA_Freeze = 0,
    AA_Active
}AlgorithmActive;

typedef struct _FrameNode_       FrameNode;
typedef struct _FrameListResult_ FrameListResult;
typedef struct _FrameResult_     FrameResult;
typedef struct _FrameListInfo_   FrameListInfo;
typedef struct __AlgorithmInfo__ AlgorithmInfo;

//全局结果结构体
struct _FrameResult_ {
    void* frameResult;                               //结果指针
    int   resultSize;                                //结果大小
    AlgorithmType isSync;                            //是否是同步
    volatile FrameDealStatus statusIs;               //结果状态
};

//结果结构体
struct _FrameListResult_ {
    void* frameResult;                               //结果指针
    int   resultSize;                                //结果大小
    volatile FrameDealStatus isProcess;              //这个算法的处理状态
};                                                   
                                                     
//算法依赖信息                                       
struct __AlgorithmInfo__ {                           
    char  name[256];                                 //算法的名字
    char  dependName[256];                           //算法依赖的算法的名字
    int   dependIndex;                               //算法依赖的算法的索引
    void* dependResultPtr;                           //算法依赖的算法的结果指针
};

//链表节点
struct _FrameNode_ {
    FrameListResult gframeListResult[AlgorithmNum];
    unsigned char* frameData;                        //传入的图像数据
    float matrix[16];                                //纹理的变化矩阵
    int ImageDataSize;                               //传入的图像数据大小
    atomic_int_Alva frameListDealNum;                //节点被几个同步算法处理过
    volatile FrameDealStatus isDeal;                 //处理状态
};

//链表信息
struct _FrameListInfo_ {
    FrameNode* Camera_buffer;                        //相机空间
    FrameNode* Perf_buffer;                          //处理空间
    FrameNode* UI_buffer;                            //UI空间
    AlgorithmInfo algorithmInfo[AlgorithmNum];       //依赖信息
    int regIsFlag[AlgorithmNum];                     //算法注册索引
    atomic_int_Alva frameFlag;                       //数据锁标志位
    atomic_int_Alva regAlgorithmNum;                 //注册了几个算法
    atomic_int_Alva regSyncAlgorithmNum;             //同步算法注册了几个
    atomic_int_Alva freezeAlgorithmNum;              //冻结的算法数
};

static FrameListInfo gFrameListInfo;
static FrameResult   gFrameResult[AlgorithmNum];

static atomic_int_Alva AlgorithmStatus[AlgorithmNum];//算法状态
static atomic_int_Alva mFrameStatus   = FIS_Free;    //链表初始化标志
static atomic_int_Alva mThreadMessage = TM_None;     //主线程消息标志

//链表加锁函数
void lockFrameList() {
#undef FUNC_CODE
#define FUNC_CODE 0x0001

    long ready = FS_Ready;
    long busy  = FS_Busy;

    while (atomic_compare_exchange_Alva(&(gFrameListInfo.frameFlag), &ready, busy)) {
        mSleep_Alva(1);
    }
}

//链表解锁函数
void unlockFrameList() {
#undef FUNC_CODE
#define FUNC_CODE 0x0002

    atomic_store_Alva(&(gFrameListInfo.frameFlag), FS_Ready);
}

EXPORT_API int FrameInit(int ImageSize) {
#undef FUNC_CODE
#define FUNC_CODE 0x0003

    long status = FIS_Inited;

    if (!atomic_compare_exchange_Alva(&(mFrameStatus), &status, status)) {
        if (gFrameListInfo.Camera_buffer->ImageDataSize == ImageSize) {
            LOGE("Already Inited!\n");
            return 0;
        }
        else {
            OWN_ERROR_RETURN(0x001, "Initialized, ImageSize is not equal with last initial.\n");
        }
    }
    LOGE("FrameInit start!\n");
    atomic_store_Alva(&(gFrameListInfo.frameFlag), FS_Ready);
    gFrameListInfo.Camera_buffer = (FrameNode*)calloc(1, sizeof(FrameNode));
    gFrameListInfo.Perf_buffer   = (FrameNode*)calloc(1, sizeof(FrameNode));
    gFrameListInfo.UI_buffer     = (FrameNode*)calloc(1, sizeof(FrameNode));
    gFrameListInfo.Camera_buffer->frameData = (unsigned char*)calloc(1, ImageSize);
    gFrameListInfo.Perf_buffer->frameData   = (unsigned char*)calloc(1, ImageSize);
    gFrameListInfo.UI_buffer->frameData     = (unsigned char*)calloc(1, ImageSize);
    memset(gFrameListInfo.Camera_buffer->matrix, 0, 16 * sizeof(float));
    memset(gFrameListInfo.Perf_buffer->matrix, 0, 16 * sizeof(float));
    memset(gFrameListInfo.UI_buffer->matrix, 0, 16 * sizeof(float));
    gFrameListInfo.Camera_buffer->ImageDataSize = ImageSize;
    gFrameListInfo.Perf_buffer->ImageDataSize   = ImageSize;
    gFrameListInfo.UI_buffer->ImageDataSize     = ImageSize;
    gFrameListInfo.Camera_buffer->frameListDealNum = 0;
    gFrameListInfo.Perf_buffer->frameListDealNum   = 0;
    gFrameListInfo.UI_buffer->frameListDealNum     = 0;
    gFrameListInfo.Camera_buffer->isDeal = FDS_Free;
    gFrameListInfo.Perf_buffer->isDeal   = FDS_Free;
    gFrameListInfo.UI_buffer->isDeal     = FDS_Free;
    if ((NULL == gFrameListInfo.Camera_buffer) || (NULL == gFrameListInfo.Camera_buffer->frameData)
        ||(NULL == gFrameListInfo.Perf_buffer) || (NULL == gFrameListInfo.Perf_buffer->frameData)
        ||(NULL == gFrameListInfo.UI_buffer)   || (NULL == gFrameListInfo.UI_buffer->frameData)) {
        OWN_ERROR_RETURN(0x002, "FrameInit frameData is NULL");
    }
    int i = 0;
    for (i = 0; i < AlgorithmNum; i++) {
        gFrameListInfo.algorithmInfo[i].dependIndex = -1;
        memset(gFrameListInfo.algorithmInfo[i].dependName, 0, 256 * sizeof(char));
        memset(gFrameListInfo.algorithmInfo[i].name, 0, 256 * sizeof(char));
        gFrameListInfo.algorithmInfo[i].dependResultPtr = NULL;
        atomic_store_Alva(&(AlgorithmStatus[i]), AA_Active);
    }
    memset(gFrameListInfo.regIsFlag, -1, AlgorithmNum * sizeof(int));
    gFrameListInfo.regAlgorithmNum = 0;
    gFrameListInfo.regSyncAlgorithmNum = 0;
    gFrameListInfo.freezeAlgorithmNum = 0;
    atomic_store_Alva(&(mThreadMessage), TM_None);
    atomic_store_Alva(&(mFrameStatus), FIS_Inited);
    LOGE("FrameInit end!\n");
    return 0;
}

void FL_mainThreadQuit() {
#undef FUNC_CODE
#define FUNC_CODE 0x0004

    atomic_store_Alva(&(mThreadMessage), TM_Stop);
}

EXPORT_API int FrameUnit() {
#undef FUNC_CODE
#define FUNC_CODE 0x0005

    LOGE("FrameUnit start!\n");
    long status = FIS_Inited;

    if (atomic_compare_exchange_Alva(&(mFrameStatus), &status, status)) {
        LOGE("uninitialized\n");
        return 0;
    }
    FL_mainThreadQuit();
    int i = 0;

    for (i = 0; i < AlgorithmNum; i++) {
        while (1) {
            lockFrameList();
            if (-1 == gFrameListInfo.regIsFlag[i]) {
                unlockFrameList();
                break;
            }
            unlockFrameList();
            mSleep_Alva(1);
        }
    }
    lockFrameList();
    free(gFrameListInfo.Camera_buffer->frameData);
    gFrameListInfo.Camera_buffer->frameData = NULL;
    free(gFrameListInfo.Camera_buffer);
    gFrameListInfo.Camera_buffer = NULL;

    free(gFrameListInfo.Perf_buffer->frameData);
    gFrameListInfo.Perf_buffer->frameData = NULL;
    free(gFrameListInfo.Perf_buffer);
    gFrameListInfo.Perf_buffer = NULL;

    free(gFrameListInfo.UI_buffer->frameData);
    gFrameListInfo.UI_buffer->frameData = NULL;
    free(gFrameListInfo.UI_buffer);
    gFrameListInfo.UI_buffer = NULL;

    atomic_store_Alva(&(mFrameStatus), FIS_Free);
    unlockFrameList();
    LOGE("FrameUnit end!\n");
    return 0;
}

int FL_regAlgorithm(char algorithmName[256], int resultSize, AlgorithmType algorithmType, void* initResult) {
#undef FUNC_CODE
#define FUNC_CODE 0x0006

    LOGE("FL_regAlgorithm %s start!\n", algorithmName);
    long status = FIS_Inited;

    if (atomic_compare_exchange_Alva(&mFrameStatus, &status, status)) {
        OWN_ERROR_RETURN(0x001, "uninitialized");
    }
    status = TM_Stop;
    if (!atomic_compare_exchange_Alva(&(mThreadMessage), &status, status)) {
        OWN_ERROR_RETURN(0x002, "mainThreadQuit");
    }
    if (NULL == initResult) {
        OWN_ERROR_RETURN(0x003, "initResult is NULL");
    }
    lockFrameList();
    int i = 0;
    int algorithmIndex = -1;

    for (i = 0; i < AlgorithmNum; i++) {
        if (-1 == gFrameListInfo.regIsFlag[i]) {
            algorithmIndex = i;
            gFrameListInfo.regIsFlag[i] = i;
            break;
        }
    }
    if (-1 == algorithmIndex) {
        unlockFrameList();
        OWN_ERROR_RETURN(0x004, "Reg too many algorithm\n");
    }
    memcpy_fl(gFrameListInfo.algorithmInfo[algorithmIndex].name, 256 * sizeof(char), algorithmName, 256 * sizeof(char));
    gFrameListInfo.algorithmInfo[algorithmIndex].dependResultPtr = NULL;
    gFrameListInfo.algorithmInfo[algorithmIndex].dependIndex = -1;
    memset(gFrameListInfo.algorithmInfo[algorithmIndex].dependName, 0, 256 * sizeof(char));
    if (AT_Sync == algorithmType) {
        gFrameListInfo.regSyncAlgorithmNum = gFrameListInfo.regSyncAlgorithmNum + 1;
    }
    gFrameListInfo.Camera_buffer->gframeListResult[algorithmIndex].frameResult = (void*)calloc(1, resultSize);
    gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].frameResult = (void*)calloc(1, resultSize);
    gFrameListInfo.UI_buffer->gframeListResult[algorithmIndex].frameResult = (void*)calloc(1, resultSize);
    if ((gFrameListInfo.Camera_buffer->gframeListResult[algorithmIndex].frameResult == NULL) || (gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].frameResult == NULL)
        || (gFrameListInfo.UI_buffer->gframeListResult[algorithmIndex].frameResult == NULL)) {
        unlockFrameList();
        OWN_ERROR_RETURN(0x005, "FL_regAlgorithm frameResult is NULL");
    }
    memset(gFrameListInfo.Camera_buffer->gframeListResult[algorithmIndex].frameResult, 0, resultSize);
    memset(gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].frameResult, 0, resultSize);
    memset(gFrameListInfo.UI_buffer->gframeListResult[algorithmIndex].frameResult, 0, resultSize);
    gFrameListInfo.Camera_buffer->gframeListResult[algorithmIndex].resultSize = resultSize;
    gFrameListInfo.Camera_buffer->gframeListResult[algorithmIndex].isProcess = FDS_Free;
    gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].resultSize = resultSize;
    gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].isProcess = FDS_Free;
    gFrameListInfo.UI_buffer->gframeListResult[algorithmIndex].resultSize = resultSize;
    gFrameListInfo.UI_buffer->gframeListResult[algorithmIndex].isProcess = FDS_Free;

    gFrameListInfo.regAlgorithmNum = gFrameListInfo.regAlgorithmNum + 1;

    gFrameResult[algorithmIndex].resultSize = resultSize;
    gFrameResult[algorithmIndex].frameResult = (void*)calloc(1, resultSize);
    if (gFrameResult[algorithmIndex].frameResult == NULL) {
        unlockFrameList();
        OWN_ERROR_RETURN(0x006, "FL_regAlgorithm frameResult is NULL");
    }
    memcpy_fl(gFrameResult[algorithmIndex].frameResult, resultSize, initResult, resultSize);
    gFrameResult[algorithmIndex].isSync = algorithmType;
    AlgorithmStatus[algorithmIndex] = 1;
    unlockFrameList();
    LOGE("FL_regAlgorithm %s end!\n", algorithmName);
    return algorithmIndex;
}

int FL_unRegAlgorithm(int algorithmIndex) {
#undef FUNC_CODE
#define FUNC_CODE 0x0007

    LOGE("FL_unRegAlgorithm %d start!\n", algorithmIndex);
    long status = FIS_Inited;

    if (atomic_compare_exchange_Alva(&(mFrameStatus), &status, status)) {
        OWN_ERROR_RETURN(0x001, "uninitialized");
    }
    if ((algorithmIndex >= AlgorithmNum) || (algorithmIndex < 0)) {
        OWN_ERROR_RETURN(0x002, "FL_unRegAlgorithm algorithmIndex over the mark");
    }
    lockFrameList();
    int i = 0;

    if (-1 != gFrameListInfo.regIsFlag[algorithmIndex]) {
        free(gFrameResult[algorithmIndex].frameResult);
        gFrameResult[algorithmIndex].frameResult = NULL;
        gFrameListInfo.regAlgorithmNum--;
        if (AT_Sync == gFrameResult[algorithmIndex].isSync) {
            gFrameListInfo.regSyncAlgorithmNum--;
            if (FDS_Perfing == gFrameListInfo.Perf_buffer->isDeal) {
                if (FDS_Perfed == gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].isProcess) {
                    gFrameListInfo.Perf_buffer->frameListDealNum--;
                }
            }
            status = AA_Active;
            if (atomic_compare_exchange_Alva(&(AlgorithmStatus[algorithmIndex]), &status, status)) {
                gFrameListInfo.freezeAlgorithmNum--;
                atomic_store_Alva(&(AlgorithmStatus[algorithmIndex]), AA_Active);
            }
        }
        free(gFrameListInfo.Camera_buffer->gframeListResult[algorithmIndex].frameResult);
        gFrameListInfo.Camera_buffer->gframeListResult[algorithmIndex].frameResult = NULL;
        free(gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].frameResult);
        gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].frameResult = NULL;
        free(gFrameListInfo.UI_buffer->gframeListResult[algorithmIndex].frameResult);
        gFrameListInfo.UI_buffer->gframeListResult[algorithmIndex].frameResult = NULL;
        gFrameListInfo.regIsFlag[algorithmIndex] = -1;
        gFrameListInfo.algorithmInfo[algorithmIndex].dependIndex = -1;
        gFrameListInfo.algorithmInfo[algorithmIndex].dependResultPtr = NULL;
        memset(gFrameListInfo.algorithmInfo[algorithmIndex].dependName, 0, 256 * sizeof(char));
        memset(gFrameListInfo.algorithmInfo[algorithmIndex].name, 0, 256 * sizeof(char));
    }
    unlockFrameList();
    LOGE("FL_unRegAlgorithm %d end!\n", algorithmIndex);
    return 0;
}

EXPORT_API int UpdateImage(unsigned char* frameData, float matrix[16]) {
#undef FUNC_CODE
#define FUNC_CODE 0x0008

    LOGE("UpdateImage start!\n");
    long status = FIS_Inited;
    int i = 0;

    if (atomic_compare_exchange_Alva(&(mFrameStatus), &status, status)) {
        OWN_ERROR_RETURN(0x003, "uninitialized");
    }
    if (NULL == frameData) {
        OWN_ERROR_RETURN(0x002, "UpdateImage frameData is NULL");
    }
    if (NULL == matrix) {
        OWN_ERROR_RETURN(0x004, "UpdateImage matrix is NULL");
    }
    status = TM_Stop;
    if (!atomic_compare_exchange_Alva(&(mThreadMessage), &status, status)) {
        OWN_ERROR_RETURN(0x001, "mainThreadQuit");
    }

    memcpy_fl(gFrameListInfo.Camera_buffer->frameData, gFrameListInfo.Camera_buffer->ImageDataSize, frameData, gFrameListInfo.Camera_buffer->ImageDataSize);
    memcpy_fl(gFrameListInfo.Camera_buffer->matrix, 16 * sizeof(float), matrix, 16 * sizeof(float));
    gFrameListInfo.Camera_buffer->frameListDealNum = 0;
    for (i = 0; i < AlgorithmNum; i++) {
        if (-1 != gFrameListInfo.regIsFlag[i]) {
            gFrameListInfo.Camera_buffer->gframeListResult[i].isProcess = FDS_Free;
        }
    }
    if (gFrameListInfo.regSyncAlgorithmNum > gFrameListInfo.freezeAlgorithmNum){
        gFrameListInfo.Camera_buffer->isDeal = FDS_Needperf;
    }
    else {
        gFrameListInfo.Camera_buffer->isDeal = FDS_Update;
    }
    lockFrameList();
    if ((FDS_Perfing != gFrameListInfo.Perf_buffer->isDeal) && (FDS_Perfed != gFrameListInfo.Perf_buffer->isDeal)) {
        FrameNode* tempNode = NULL;
        tempNode = gFrameListInfo.Camera_buffer;
        gFrameListInfo.Camera_buffer = gFrameListInfo.Perf_buffer;
        gFrameListInfo.Perf_buffer = tempNode;
    }
    unlockFrameList();
    LOGE("UpdateImage end!\n");
    return 0;
}

EXPORT_API int getBackground(unsigned char * frameData) {
#undef FUNC_CODE
#define FUNC_CODE 0x0009

    LOGE("getBackground start!\n");
    long status = FIS_Inited;

    if (atomic_compare_exchange_Alva(&(mFrameStatus), &status, status)) {
        OWN_ERROR_RETURN(0x003, "uninitialized");
    }
    if (NULL == frameData) {
        OWN_ERROR_RETURN(0x002, "getBackground frameData is NULL");
    }
    status = TM_Stop;
    if (!atomic_compare_exchange_Alva(&(mThreadMessage), &status, status)) {
        OWN_ERROR_RETURN(0x001, "mainThreadQuit");
    }
    lockFrameList();
    if ((FDS_Free == gFrameListInfo.UI_buffer->isDeal) || (FDS_Used == gFrameListInfo.UI_buffer->isDeal)) {
        if ((FDS_Free != gFrameListInfo.Perf_buffer->isDeal) && (FDS_Needperf != gFrameListInfo.Perf_buffer->isDeal)
            && (FDS_Perfing != gFrameListInfo.Perf_buffer->isDeal)) {
            gFrameListInfo.UI_buffer->isDeal = FDS_Free;
            FrameNode* tempNode = NULL;
            tempNode = gFrameListInfo.UI_buffer;
            gFrameListInfo.UI_buffer = gFrameListInfo.Perf_buffer;
            gFrameListInfo.Perf_buffer = tempNode;
        }
    }
    memcpy_fl(frameData, gFrameListInfo.UI_buffer->ImageDataSize, gFrameListInfo.UI_buffer->frameData, gFrameListInfo.UI_buffer->ImageDataSize);
    if (FDS_Used != gFrameListInfo.UI_buffer->isDeal) {
        int i = 0;
        for (i = 0; i < AlgorithmNum;i++) {
            if (-1 != gFrameListInfo.regIsFlag[i]) {
                if (AT_Sync == gFrameResult[i].isSync) {
                    status = AA_Active;
                    if (!atomic_compare_exchange_Alva(&(AlgorithmStatus[i]), &status, status)) {
                        memcpy_fl(gFrameResult[i].frameResult, gFrameResult[i].resultSize, gFrameListInfo.UI_buffer->gframeListResult[i].frameResult, gFrameListInfo.UI_buffer->gframeListResult[i].resultSize);
                    }
                }
            }
        }
    }
    gFrameListInfo.UI_buffer->isDeal = FDS_Used;
    unlockFrameList();
    LOGE("getBackground end!\n");
    return 0;
}

int FL_setResultData(int algorithmIndex, void* frameResultData) {
#undef FUNC_CODE
#define FUNC_CODE 0x000A

    LOGE("FL_setResultData %d start!\n",algorithmIndex);
    long status = FIS_Inited;

    if (atomic_compare_exchange_Alva(&(mFrameStatus), &status, status)) {
        OWN_ERROR_RETURN(0x004, "uninitialized");
    }
    if ((algorithmIndex >= AlgorithmNum) || (algorithmIndex < 0)) {
        OWN_ERROR_RETURN(0x002, "FL_setResultData algorithmIndex over the mark");
    }
    if (NULL == frameResultData) {
        OWN_ERROR_RETURN(0x003, "FL_setResultData frameResultData is NULL");
    }
    status = TM_Stop;
    if (!atomic_compare_exchange_Alva(&(mThreadMessage), &status, status)) {
        OWN_ERROR_RETURN(0x001, "mainThreadQuit");
    }
    status = AA_Active;
    if (atomic_compare_exchange_Alva(&(AlgorithmStatus[algorithmIndex]), &status, status)) {
        LOGE("algorithm inactive\n");
        return 0;
    }
    lockFrameList();
    if (-1 != gFrameListInfo.regIsFlag[algorithmIndex]) {
        if (AT_Sync == gFrameResult[algorithmIndex].isSync) {
            memcpy_fl(gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].frameResult, gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].resultSize,
                frameResultData, gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].resultSize);
            gFrameListInfo.Perf_buffer->frameListDealNum++;
            gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].isProcess = FDS_Perfed;
            if (gFrameListInfo.Perf_buffer->frameListDealNum == (gFrameListInfo.regSyncAlgorithmNum - gFrameListInfo.freezeAlgorithmNum)) {
                gFrameListInfo.Perf_buffer->isDeal = FDS_Perfed;
                gFrameListInfo.UI_buffer->isDeal = FDS_Free;
                FrameNode* tempNode = NULL;
                tempNode = gFrameListInfo.UI_buffer;
                gFrameListInfo.UI_buffer = gFrameListInfo.Perf_buffer;
                gFrameListInfo.Perf_buffer = tempNode;
            }
        }
        else {
            memcpy_fl(gFrameResult[algorithmIndex].frameResult, gFrameResult[algorithmIndex].resultSize, frameResultData, gFrameResult[algorithmIndex].resultSize);
        }
    }
    unlockFrameList();
    LOGE("FL_setResultData %d end!\n", algorithmIndex);
    return 0;
}

int FL_getData(int algorithmIndex, unsigned char* frameData, float matrix[16]) {
#undef FUNC_CODE
#define FUNC_CODE 0x000B

    LOGE("FL_getData %d start!\n", algorithmIndex);
    long status = FIS_Inited;

    if (atomic_compare_exchange_Alva(&(mFrameStatus), &status, status)) {
        OWN_ERROR_RETURN(0x004, "uninitialized");
    }
    if ((algorithmIndex >= AlgorithmNum) || (algorithmIndex < 0)) {
        OWN_ERROR_RETURN(0x002, "FL_getData algorithmIndex over the mark");
    }
    if (NULL == frameData) {
        OWN_ERROR_RETURN(0x003, "FL_getData frameData is NULL");
    }
    if (NULL == matrix) {
        OWN_ERROR_RETURN(0x005, "FL_getData matrix is NULL");
    }
    while (1) {
        lockFrameList();
        status = TM_Stop;
        if (!atomic_compare_exchange_Alva(&(mThreadMessage), &status, status)) {
            unlockFrameList();
            OWN_ERROR_RETURN(0x001, "mainThreadQuit");
        }
        if (-1 == gFrameListInfo.regIsFlag[algorithmIndex]) {
            unlockFrameList();
            OWN_ERROR_RETURN(0x006, "Algorithm united!");
        }
        status = AA_Active;
        if (atomic_compare_exchange_Alva(&(AlgorithmStatus[algorithmIndex]), &status, status)) {
            LOGE("algorithm inactive\n");
            unlockFrameList();
            return 0;
        }
        if ((FDS_Update == gFrameListInfo.Perf_buffer->isDeal) || (FDS_Needperf == gFrameListInfo.Perf_buffer->isDeal)
            || (FDS_Perfing == gFrameListInfo.Perf_buffer->isDeal)) {
            if (FDS_Perfed != gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].isProcess) {
                memcpy_fl(matrix, 16 * sizeof(float), gFrameListInfo.Perf_buffer->matrix, 16 * sizeof(float));
                memcpy_fl(frameData, gFrameListInfo.Perf_buffer->ImageDataSize, gFrameListInfo.Perf_buffer->frameData, gFrameListInfo.Perf_buffer->ImageDataSize);
                if (AT_Sync == gFrameResult[algorithmIndex].isSync) {
                    gFrameListInfo.Perf_buffer->isDeal = FDS_Perfing;
                }
                break;
            }
        }
        unlockFrameList();
        mSleep_Alva(2);
    }
    unlockFrameList();
    LOGE("FL_getData %d end!\n", algorithmIndex);
    return 0;
}

int FL_updateResult(int algorithmIndex, void* frameResultData) {
#undef FUNC_CODE
#define FUNC_CODE 0x000C

    LOGE("FL_updateResult %d start!\n", algorithmIndex);
    long status = FIS_Inited;

    if (atomic_compare_exchange_Alva(&(mFrameStatus), &status, status)) {
        OWN_ERROR_RETURN(0x004, "uninitialized");
    }
    if ((algorithmIndex >= AlgorithmNum) || (algorithmIndex < 0)) {
        OWN_ERROR_RETURN(0x002, "FL_updateResult algorithmIndex over the mark");
    }
    if (NULL == frameResultData) {
        OWN_ERROR_RETURN(0x003, "FL_updateResult frameResultData is NULL");
    }
    status = TM_Stop;
    if (!atomic_compare_exchange_Alva(&(mThreadMessage), &status, status)) {
        OWN_ERROR_RETURN(0x001, "mainThreadQuit");
    }
    lockFrameList();
    if (-1 != gFrameListInfo.regIsFlag[algorithmIndex]) {
        status = AA_Active;
        if (atomic_compare_exchange_Alva(&(AlgorithmStatus[algorithmIndex]), &status, status)) {
            LOGE("algorithm inactive\n");
            unlockFrameList();
            return 0;
        }
        memcpy_fl(frameResultData, gFrameResult[algorithmIndex].resultSize, gFrameResult[algorithmIndex].frameResult, gFrameResult[algorithmIndex].resultSize);
    }
    else {
        unlockFrameList();
        OWN_ERROR_RETURN(0x005, "Algorithm United!");
    }
    unlockFrameList();
    LOGE("FL_updateResult %d end!\n", algorithmIndex);
    return 0;
}

int FL_activeAlgorithm(int algorithmIndex) {
#undef FUNC_CODE
#define FUNC_CODE 0x000D

    LOGE("FL_activeAlgorithm %d start!\n", algorithmIndex);
    long status = FIS_Inited;

    if (atomic_compare_exchange_Alva(&(mFrameStatus), &status, status)) {
        OWN_ERROR_RETURN(0x003, "uninitialized");
    }
    if ((algorithmIndex >= AlgorithmNum) || (algorithmIndex < 0)) {
        OWN_ERROR_RETURN(0x001, "FL_activeAlgorithm algorithmIndex over the mark");
    }
    status = TM_Stop;
    if (!atomic_compare_exchange_Alva(&(mThreadMessage), &status, status)) {
        OWN_ERROR_RETURN(0x004, "mainThreadQuit");
    }
    lockFrameList();
    if (-1 == gFrameListInfo.regIsFlag[algorithmIndex]) {
        unlockFrameList();
        OWN_ERROR_RETURN(0x002, "FL_activeAlgorithm algorithmIndex is not register");
    }
    status = AA_Active;
    if (!atomic_compare_exchange_Alva(&(AlgorithmStatus[algorithmIndex]), &status, status)) {
        LOGE("Has been activated!\n");
    }
    else  {
        if (AT_Sync == gFrameResult[algorithmIndex].isSync) {
            gFrameListInfo.freezeAlgorithmNum--;
        }
        atomic_store_Alva(&(AlgorithmStatus[algorithmIndex]), AA_Active);
    }
    unlockFrameList();
    LOGE("FL_activeAlgorithm %d end!\n", algorithmIndex);
    return 0;
}

int FL_freezeAlgorithm(int algorithmIndex) {
#undef FUNC_CODE
#define FUNC_CODE 0x000E

    LOGE("FL_freezeAlgorithm %d start!\n", algorithmIndex);
    long status = FIS_Inited;

    if (atomic_compare_exchange_Alva(&(mFrameStatus), &status, status)) {
        OWN_ERROR_RETURN(0x003, "uninitialized");
    }
    if ((algorithmIndex >= AlgorithmNum) || (algorithmIndex < 0)) {
        OWN_ERROR_RETURN(0x001, "FL_freezeAlgorithm algorithmIndex over the mark");
    }
    status = TM_Stop;
    if (!atomic_compare_exchange_Alva(&(mThreadMessage), &status, status)) {
        OWN_ERROR_RETURN(0x002, "mainThreadQuit");
    }
    lockFrameList();
    if (-1 == gFrameListInfo.regIsFlag[algorithmIndex]) {
        unlockFrameList();
        OWN_ERROR_RETURN(0x004, "FL_freezeAlgorithm algorithmIndex is not register");
    }
    status = AA_Active;
    if (!atomic_compare_exchange_Alva(&(AlgorithmStatus[algorithmIndex]), &status, status)) {
        atomic_store_Alva(&(AlgorithmStatus[algorithmIndex]), AA_Freeze);
        if (AT_Sync == gFrameResult[algorithmIndex].isSync) {
            if ((FDS_Perfing == gFrameListInfo.Perf_buffer->isDeal) && (FDS_Perfed == gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].isProcess)) {
                gFrameListInfo.Perf_buffer->frameListDealNum--;
            }
            gFrameListInfo.freezeAlgorithmNum++;
        }
    }
    else {
        LOGE("Have been frozen!\n");
    }
    unlockFrameList();
    LOGE("FL_freezeAlgorithm %d end!\n", algorithmIndex);
    return 0;
}

int FL_switchoverAlgorithm(int algorithmIndex, AlgorithmType algorithmType) {
#undef FUNC_CODE
#define FUNC_CODE 0x000F

    LOGE("FL_switchoverAlgorithm %d start!\n", algorithmIndex);
    long status = FIS_Inited;

    if (atomic_compare_exchange_Alva(&(mFrameStatus), &status, status)) {
        OWN_ERROR_RETURN(0x003, "uninitialized");
    }
    if ((algorithmIndex >= AlgorithmNum) || (algorithmIndex < 0)) {
        OWN_ERROR_RETURN(0x001, "FL_freezeAlgorithm algorithmIndex over the mark");
    }
    status = TM_Stop;
    if (!atomic_compare_exchange_Alva(&(mThreadMessage), &status, status)) {
        OWN_ERROR_RETURN(0x002, "mainThreadQuit");
    }
    lockFrameList();
    if (-1 == gFrameListInfo.regIsFlag[algorithmIndex]) {
        unlockFrameList();
        OWN_ERROR_RETURN(0x004, "FL_freezeAlgorithm algorithmIndex is not register");
    }
    if (algorithmType != gFrameResult[algorithmIndex].isSync){
        if (AT_Sync == gFrameResult[algorithmIndex].isSync){
            status = AA_Active;
            if (!atomic_compare_exchange_Alva(&(AlgorithmStatus[algorithmIndex]), &status, status)) {
                if (FDS_Perfed ==gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].isProcess){
                    gFrameListInfo.Perf_buffer->frameListDealNum--;
                }
            }
            else {
                gFrameListInfo.freezeAlgorithmNum--;
            }
            gFrameListInfo.regSyncAlgorithmNum--;
        }
        else {
            status = AA_Active;
            if (!atomic_compare_exchange_Alva(&(AlgorithmStatus[algorithmIndex]), &status, status)) {
                gFrameListInfo.Perf_buffer->gframeListResult[algorithmIndex].isProcess = FDS_Perfed;
                gFrameListInfo.Perf_buffer->frameListDealNum++;
            }
            else {
                gFrameListInfo.freezeAlgorithmNum++;
            }
            gFrameListInfo.regSyncAlgorithmNum++;
        }
        gFrameResult[algorithmIndex].isSync = algorithmType;
    }
    else {
        LOGE("FL_switchoverAlgorithm algorithmType is equal.\n");
    }
    unlockFrameList();
    LOGE("FL_switchoverAlgorithm %d end!\n", algorithmIndex);
    return 0;
}
