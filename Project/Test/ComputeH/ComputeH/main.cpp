#include "Config.h"

#include "stdio_fl.h"
#include <stdlib.h>
#include <math.h>
#include "memory_fl.h"

#include <windows.h>

#include "string_fl.h"
#include <direct.h>
#include <io.h>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <time.h>
#include <string.h>

using namespace std;

#define LOG_TAG "ComputeH_Direct"
#include "Log.h"
#include "ErrorTools.h"

#include "ComputeH_Direct.h"

#undef FILE_NUM
#define FILE_NUM 0x0100

int main(int argc, char** argv){

#undef FUNC_CODE
#define FUNC_CODE 0x01

    int num = 0;

    float* srcPoints = (float*)calloc(1, sizeof(float) * 2 * 2000);
    float* dstPoints = (float*)calloc(1, sizeof(float) * 2 * 2000);

    char* fileFullName = NULL;

    //设置当前目录
    char sBuf[1024];
    char *ptr;
    if (GetModuleFileNameA(NULL, sBuf, sizeof(sBuf)))
    {
        ptr = strrchr(sBuf, '\\');
        if (ptr)
            *ptr = '\0';
        SetCurrentDirectoryA(sBuf);
    }

    int err = 0;
    if (argc == 1) {

        srcPoints[0] = 1.0f;
        srcPoints[1] = 2.5f;

        srcPoints[2] = 1.0f + 0.8f;
        srcPoints[3] = 2.5f + 0.5f;

        srcPoints[4] = 1.3f;
        srcPoints[5] = 2.2f;

        srcPoints[6] = 1.3f + 0.33f;
        srcPoints[7] = 2.2f + 0.76f;

        srcPoints[8] = 7.35f;
        srcPoints[9] = 9.66f;

        srcPoints[10] = 7.35f + 0.22f;
        srcPoints[11] = 9.66f + 98.3f;

        srcPoints[12] = 56.33f;
        srcPoints[13] = 89.3f;

        srcPoints[14] = 56.33f + 22.4f;
        srcPoints[15] = 89.3f + 56.7f;

        srcPoints[16] = 66.4f + 55.5f;
        srcPoints[17] = 77.0f + 99.1f;

        srcPoints[18] = 66.4f;
        srcPoints[19] = 77.0f;

        num = 10;

        memcpy_s(dstPoints, sizeof(float) * 2 * num, srcPoints, sizeof(float) * 2 * num);
    }
    else if (argc == 2) {
        fileFullName = (char*)malloc(strlen(argv[1]) + 1);
        strcpy_fl(fileFullName, strlen(argv[1]) + 1, argv[1]);

        fstream mapFile;

        char* buf = new char[1024];

        mapFile.open(fileFullName, ios::in);

        num = 0;
        do {
            mapFile.clear(ios::goodbit);
            mapFile.getline(buf, 1024);
            if (mapFile.fail())
                break;

            sscanf_s(buf, "%f, %f, %f, %f", &(srcPoints[num * 2 + 0]), &(srcPoints[num * 2 + 1]),
                                            &(dstPoints[num * 2 + 0]), &(dstPoints[num * 2 + 1]));
            num++;
        } while (!mapFile.fail());

        mapFile.close();
    }
    else {
        printf("parameter error, use: xxx.exe filename\n");
        return -1;
    }

    float H[9];

    pComputeH_Direct_Data mComputeHData;
    AllocComputeH_Direct_Data(&mComputeHData, 2000);
    setComputeH_Direct_Data(srcPoints, dstPoints, NULL, num, mComputeHData);
    ComputeH_Direct(mComputeHData, H);
    FreeComputeH_Direct_Data(&mComputeHData);

    LOGE("--------------H------------\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf_fl("%f ", H[i * 3 + j]);
        }
        printf_fl("\n");
    }

    free(srcPoints);
    free(dstPoints);
    return getchar();
}
