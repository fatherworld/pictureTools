#include "Config.h"

#include "stdio_fl.h"
#include <stdlib.h>
#include <math.h>
#include "memory_fl.h"

#define LOG_TAG "Main"
#include "Log.h"
#include "ErrorTools.h"

#include "Consistency_Rand.h"
#if 0
int main() {
    float* srcPoints = (float*)calloc(1, sizeof(float) * 2 * 10);
    float* dstPoints = (float*)calloc(1, sizeof(float) * 2 * 10);
    int i = 0;
    int consNum = 0;

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

    memcpy_s(dstPoints, sizeof(float) * 2 * 10, srcPoints, sizeof(float) * 2 * 10);
    pConsistency_Rand_Info data = NULL;
    AllocConsistency_Rand_Data(&data, 10);

    setRandTime(data, 300);

    int matchLoc[20] = {0};
    for(int i = 0; i < 10; i++) {
        matchLoc[i * 2 + 0] = i;
        matchLoc[i * 2 + 1] = i;
    }
    Consistency_Rand_Check_disperse(srcPoints, dstPoints, matchLoc, 10, data);
    //Consistency_Rand_Check_disperse_Iteration(srcPoints, dstPoints, matchLoc, 10, data);

    return getchar();
}
#endif

int main() {

    float modelXY[64] = {
        1242.000000f,86.000000f ,
        285.000000f ,157.000000f ,
        264.000000f ,807.000000f ,
        1140.000000f,622.000000f,
        1239.000000f,197.000000f,
        285.000000f ,157.000000f ,
        257.000000f ,618.000000f ,
        1020.000000f,593.000000f,
        1063.000000f,174.000000f,
        258.000000f ,282.000000f ,
        357.000000f ,679.000000f ,
        1126.000000f,342.000000f,
        411.000000f ,178.000000f ,
        489.000000f ,670.000000f ,
        948.000000f ,658.000000f ,
        937.000000f ,184.000000f ,
        249.000000f ,400.000000f ,
        460.000000f ,573.000000f ,
        1122.000000f,406.000000f,
        604.000000f ,74.000000f  ,
        555.000000f ,642.000000f ,
        798.000000f ,687.000000f ,
        807.000000f ,149.000000f ,
        476.000000f ,231.000000f ,
        560.000000f ,494.000000f ,
        922.000000f ,469.000000f ,
        911.000000f ,267.000000f ,
        457.000000f ,361.000000f ,
        695.000000f ,571.000000f ,
        774.000000f ,465.000000f ,
        726.000000f ,288.000000f ,
        583.000000f ,264.000000f};
    float featCoords[64] = {
        1090.000000f,165.000000f,
        626.000000f, 144.000000f,
        514.000000f, 316.000000f,
        1019.000000f, 428.000000f,
        1087.000000f, 167.000000f,
        626.000000f, 144.000000f,
        547.000000f, 305.000000f,
        921.000000f, 374.000000f,
        976.000000f, 214.000000f,
        597.000000f, 191.000000f,
        581.000000f, 338.000000f,
        960.000000f, 232.000000f,
        677.000000f, 164.000000f,
        646.000000f, 348.000000f,
        877.000000f, 397.000000f,
        915.000000f, 208.000000f,
        578.000000f, 225.000000f,
        647.000000f, 308.000000f,
        1055.000000f, 255.000000f,
        808.000000f, 106.000000f,
        679.000000f, 344.000000f,
        797.000000f, 394.000000f,
        855.000000f, 184.000000f,
        694.000000f, 186.000000f,
        702.000000f, 289.000000f,
        881.000000f, 315.000000f,
        909.000000f, 209.000000f,
        673.000000f, 233.000000f,
        758.000000f, 331.000000f,
        809.000000f, 299.000000f,
        800.000000f, 232.000000f,
        741.000000f, 207.000000f};

    pConsistency_Rand_Info consistency;


    AllocConsistency_Rand_Data(&consistency, 2000);

    setRandTime(consistency, 300);
    setFineThreshold(consistency, 2.0f);
    Consistency_Rand_Check_RotationConsistency(modelXY, featCoords, 32, consistency);

    int*   loc = consistency->mConsLoc;
    float* srcPoints = (float*)calloc(1, sizeof(float) * consistency->mConsNum * 2);
    float* dstPoints = (float*)calloc(1, sizeof(float) * consistency->mConsNum * 2);

    FILE* fp = NULL;
    fopen_fl(&fp, "consisCoords.txt", "w+");

    for(int offset = 0; offset < consistency->mConsNum; offset++) {
        srcPoints[offset * 2 + 0] = modelXY[loc[offset] * 2 + 0];
        srcPoints[offset * 2 + 1] = modelXY[loc[offset] * 2 + 1];

        dstPoints[offset * 2 + 0] = featCoords[loc[offset] * 2 + 0];
        dstPoints[offset * 2 + 1] = featCoords[loc[offset] * 2 + 1];

        fprintf_fl(fp, "%f,%f,%f,%f\n", srcPoints[offset * 2 + 0], srcPoints[offset * 2 + 1], dstPoints[offset * 2 + 0], dstPoints[offset * 2 + 1]);
    }
    fclose(fp);
    free(srcPoints);
    free(dstPoints);
    FreeConsistency_Rand_Data(&consistency);
    return 0;
}