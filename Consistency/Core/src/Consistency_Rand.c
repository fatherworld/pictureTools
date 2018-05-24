#include "Config.h"

#include "stdio_fl.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "memory_fl.h"

#define LOG_TAG "Consistency_Rand"
#include "Log.h"
#include "ErrorTools.h"

#include "Consistency_Rand.h"

#undef FILE_NUM
#define FILE_NUM 0x9900
/*
  * AllocConsistency_Rand_Data
  * Ϊһ���Լ�⺯�����������ڴ�
  * @data: һ���Լ���õ��Ľṹ�����ָ��
  * @maxPointNum: ��������ĵ�Ը���
  * @return: ��
  **/
void AllocConsistency_Rand_Data(pConsistency_Rand_Info* data, int maxPointNum) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

    pConsistency_Rand_Info temp = (pConsistency_Rand_Info)calloc(sizeof(Consistency_Rand_Info), 1);

    temp->mConsLoc              = NULL;
    temp->mConsNum              = 0;
    temp->mH[0]                 = 0;

    temp->pRandTime             = 300;
    temp->pMaxPointNum          = maxPointNum;
    temp->pCoarseThreshold      = 5.0f * 5.0f;
    temp->pFineThreshold        = 2.0f * 2.0f;

    temp->tConsensus1           = (int*)malloc(sizeof(int) * maxPointNum);
    temp->tConsensus2           = (int*)malloc(sizeof(int) * maxPointNum);
    temp->tSampled              = (int*)malloc(sizeof(int) * maxPointNum);
    temp->tLabeled1             = (int*)malloc(sizeof(int) * maxPointNum);
    temp->tLabeled2             = (int*)malloc(sizeof(int) * maxPointNum);
    temp->tConsTimes            = (int*)malloc(sizeof(int) * maxPointNum);
    AllocComputeH_Direct_Data(&(temp->tComputeHData), maxPointNum);
    *data                       = temp;
}

/*
 * FreeConsistency_Rand_Data
 * �ͷ�һ���Լ�⺯���������ڴ�
 * @data: һ���Լ���õ��Ľṹ�����ָ��
 * @return: ��
 **/
void FreeConsistency_Rand_Data(pConsistency_Rand_Info* data) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    if (NULL != data) {
        if (NULL != *data) {
            if (NULL != (*data)->tConsensus1) {
                free((*data)->tConsensus1);
            }
            if (NULL != (*data)->tConsensus2) {
                free((*data)->tConsensus2);
            }
            if (NULL != (*data)->tSampled) {
                free((*data)->tSampled);
            }
            if (NULL != (*data)->tLabeled1) {
                free((*data)->tLabeled1);
            }
            if (NULL != (*data)->tLabeled2) {
                free((*data)->tLabeled2);
            }
            if (NULL != (*data)->tConsTimes) {
                free((*data)->tConsTimes);
            }
            FreeComputeH_Direct_Data(&((*data)->tComputeHData));
        }
        *data = NULL;
    }
}

/*
 * setRandTime
 * ����һ���Լ����������
 * @data: һ���Լ���õ��Ľṹ��ָ��
 * @time: ���õ��������
 * @return: ��
 **/
void setRandTime(pConsistency_Rand_Info data, int time) {

#undef FUNC_CODE
#define FUNC_CODE 0x03

    data->pRandTime = time;
}

/*
* setCoarseThreshold
* ���ô�ѡ��ֵ��Ĭ��Ϊ5
*/
void setCoarseThreshold(pConsistency_Rand_Info data, float threshold) {

#undef FUNC_CODE
#define FUNC_CODE 0x04

    data->pCoarseThreshold = threshold * threshold;
}

/*
* setFineThreshold
* ���ô�ѡ��ֵ��Ĭ��Ϊ2
*/
void setFineThreshold(pConsistency_Rand_Info data, float threshold) {

#undef FUNC_CODE
#define FUNC_CODE 0x05

    data->pFineThreshold = threshold * threshold;
}

/*
 * Rand_Gen_Four_Diff_Nums
 * ����n���ڵ��ĸ����ظ������������,����ŵ�sample��
 * @n: ����������ķ�Χ  < n
 * @sample: �����������ڴ�ռ�
 * @sampled: ���������ʹ�õ����м��ڴ�
 * @return: ��
 **/
static void Rand_Gen_Four_Diff_Nums(int n, int* sample, int* sampled){

#undef FUNC_CODE
#define FUNC_CODE 0x06

    int i, x;
    for (i = 0; i < 4; i++) {//��n��������ȡ��m������ģ�ͽ���
        do{
            x = rand() % n;//0-(n-1) ֮�������
        } while (1 == sampled[x]);//����ȡ���������sampledֵΪ1������ѡȡ����ֹ�Ѿ�ȡ���ĵ㱻�ظ�ʰȡ��������ֹͣ��������Ϊ�����е�һ����

        sample[i] = x;
        sampled[x] = 1;//�õ��feature_data��Ա��sampled��ֵ��Ϊ1
    }
    for (i = 0; i < 4; i++) {
        sampled[sample[i]] = 0;
    }
}

/*
 * Find_consensus
 * ��HΪ�任��ϵ,�õ�������С��һ������ֵ�ĵ��
 * @_consensus: ��ŵ�����С��һ����ֵ�ĵ�����
 * @feat0: ��һ���x,y,x,y,x,y...
 * @feat1: �ڶ����x,y,x,y,x,y...
 * @H_transform: H����
 * @num: ��Ը���
 * @err_tol: �����ֵ
 ***/
static int Find_consensus(int* _consensus, float* feat0, float* feat1, float H_transform[9], int num, float err_threshold){

#undef FUNC_CODE
#define FUNC_CODE 0x07

    float err;
    float temp;
    int i, in = 0;
    float x0, y0, x1, y1, x2, y2;

    for (i = 0; i < num; i++){
        x0 = feat0[i * 2 + 0];
        y0 = feat0[i * 2 + 1];

        x1 = feat1[i * 2 + 0];
        y1 = feat1[i * 2 + 1];

        temp = H_transform[6] * x0 + H_transform[7] * y0 + 1;
        x2 = ((H_transform[0] * x0 + H_transform[1] * y0 + H_transform[2]) / temp);
        y2 = ((H_transform[3] * x0 + H_transform[4] * y0 + H_transform[5]) / temp);

        err = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
        if (err <= err_threshold)//���任���С���ݴ�ȣ��������һ�¼�
            _consensus[in++] = i;
    }
    return 	in;
}

static int MLESAC_consensus(int* _consensus, float* score, float* feat0, float* feat1, float H_transform[9], int num, float err_threshold) {

#undef FUNC_CODE
#define FUNC_CODE 0x08

    float err;
    float temp;
    int i, in = 0;
    float x0, y0, x1, y1, x2, y2;
    float tscore = 0.0f;

    for (i = 0; i < num; i++) {
        x0 = feat0[i * 2 + 0];
        y0 = feat0[i * 2 + 1];

        x1 = feat1[i * 2 + 0];
        y1 = feat1[i * 2 + 1];

        temp = H_transform[6] * x0 + H_transform[7] * y0 + 1;
        x2 = ((H_transform[0] * x0 + H_transform[1] * y0 + H_transform[2]) / temp);
        y2 = ((H_transform[3] * x0 + H_transform[4] * y0 + H_transform[5]) / temp);

        err = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
        if (err <= err_threshold) {
            _consensus[in++] = i;
            tscore += err;
        }
        else {
            tscore += err_threshold;
        }
    }
    *score = tscore;
    return 	in;
}

static int Find_consensus_disperse(int* _consensus, int*matchLoc, float* feat0, float* feat1, float H_transform[9], int num, int maxNum, float err_threshold) {

#undef FUNC_CODE
#define FUNC_CODE 0x08

    float err;
    float temp;
    int i, in = 0;
    int idx, idy;
    float x0, y0, x1, y1, x2, y2;

    for (i = 0; i < num; i++) {
        idx = matchLoc[i];
        idy = matchLoc[maxNum + i];

        x0 = feat0[idx];
        y0 = feat0[maxNum + idx];

        x1 = feat1[idy];
        y1 = feat1[maxNum + idy];

        temp = H_transform[6] * x0 + H_transform[7] * y0 + 1;
        x2 = ((H_transform[0] * x0 + H_transform[1] * y0 + H_transform[2]) / temp);
        y2 = ((H_transform[3] * x0 + H_transform[4] * y0 + H_transform[5]) / temp);

        err = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
        if (err <= err_threshold)//���任���С���ݴ�ȣ��������һ�¼�
            _consensus[in++] = i;
    }
    return 	in;
}

static int MLESAC_consensus_disperse(int* _consensus, float* score, int*matchLoc, float* feat0, float* feat1, float H_transform[9], int num, int maxNum, float err_threshold) {

#undef FUNC_CODE
#define FUNC_CODE 0x08

    float err, tscore = 0.0f;
    float temp;
    int i, in = 0;
    int idx, idy;
    float x0, y0, x1, y1, x2, y2;

    for (i = 0; i < num; i++) {
        idx = matchLoc[i];
        idy = matchLoc[maxNum + i];

        x0 = feat0[idx];
        y0 = feat0[maxNum + idx];

        x1 = feat1[idy];
        y1 = feat1[maxNum + idy];

        temp = H_transform[6] * x0 + H_transform[7] * y0 + 1;
        x2 = ((H_transform[0] * x0 + H_transform[1] * y0 + H_transform[2]) / temp);
        y2 = ((H_transform[3] * x0 + H_transform[4] * y0 + H_transform[5]) / temp);

        err = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
        if (err <= err_threshold) {
            _consensus[in++] = i;
            tscore += err;
        }
        else {
            tscore += err_threshold;
        }
    }
    *score = tscore;
    return 	in;
}

static int CountRealMatch(int num, float* feat0, float* feat1, int* _consensus, int* labeled1, int* labeled2) {

#undef FUNC_CODE
#define FUNC_CODE 0x09

    int i, j;
    int m, n;
    float n1 = 0, n2 = 0;
    int match1, match2;
    match1 = match2 = num;
    for (i = 0; i < num; i++) {
        labeled1[i] = labeled2[i] = 0;
    }

    for (i = 0; i<num; i++) {
        m = _consensus[i];
        if (!labeled1[i]) {
            n1 = feat0[m * 2 + 0];// vMatches[i].queryIdx;
            labeled1[i] = 1;
        }
        if (!labeled2[i]) {
            n2 = feat1[m * 2 + 0];// vMatches[i].trainIdx;
            labeled2[i] = 1;
        }
        for (j = i + 1; j < num; j++) {
            n = _consensus[j];
            if (n1 == feat0[n * 2 + 0] && !labeled1[j]) {
                labeled1[j] = 1;
                match1--;
            }
            if (n2 == feat1[n * 2 + 0] && !labeled2[j]) {
                labeled2[j] = 1;
                match2--;
            }
        }
    }
    return match1 < match2 ? match1 : match2;
}

static int CountRealMatch_disperse(int num, float* feat0, float* feat1, int* matchLoc, int* _consensus, int* labeled1, int* labeled2, int maxNum) {

#undef FUNC_CODE
#define FUNC_CODE 0x0A

    int i, j;
    int m, n;
    int idxi, idxj;
    float n1 = 0, n2 = 0;

    int realMatch = 0;
    for (i = 0; i < num; i++) {
        labeled1[i] = labeled2[i] = 0;
    }

    for (i = 0; i < num; i++) {
        m = _consensus[i];
        if (!labeled1[i]) {
            idxi = matchLoc[m];
            n1 = feat0[idxi];// vMatches[i].queryIdx;
            labeled1[i] = 1;
        }
        if (!labeled2[i]) {
            idxi = matchLoc[maxNum + m];
            n2 = feat1[idxi];// vMatches[i].trainIdx;
            labeled2[i] = 1;
        }
        if (labeled1[i] == 1 && labeled2[i] == 1) {
            _consensus[realMatch] = _consensus[i];
            realMatch++;
            for (j = i + 1; j < num; j++) {
                n = _consensus[j];
                idxj = matchLoc[n];
                if (n1 == feat0[idxj] && !labeled1[j]) {
                    labeled1[j] = 2;
                }
                idxj = matchLoc[maxNum + n];
                if (n2 == feat1[idxj] && !labeled2[j]) {
                    labeled2[j] = 2;
                }
            }
        }
    }

    return realMatch;
}

static int CheckRotationConsistency(float* srcPoints, float* dstPoints, int* sample) {

#undef FUNC_CODE
#define FUNC_CODE 0x0B

    int i = 0;
    float x[4], y[4], x_[4], y_[4];
    float result = 0.f, result_ = 0.f;

    for (i = 0; i < 4; i++) {
        int n = 0;
        if (sample == NULL)  n = i;
        else  n = sample[i];

        x[i] = srcPoints[n * 2 + 0];
        y[i] = srcPoints[n * 2 + 1];

        x_[i] = dstPoints[n * 2 + 0];
        y_[i] = dstPoints[n * 2 + 1];
    }

    result = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
    result_ = (x_[1] - x_[0]) * (y_[2] - y_[0]) - (x_[2] - x_[0]) * (y_[1] - y_[0]);

    if (result * result_ <= 0)
        return 0;

    result = (x[2] - x[1]) * (y[3] - y[1]) - (x[3] - x[1]) * (y[2] - y[1]);
    result_ = (x_[2] - x_[1]) * (y_[3] - y_[1]) - (x_[3] - x_[1]) * (y_[2] - y_[1]);

    if (result * result_ <= 0)
        return 0;

    result = (x[3] - x[1]) * (y[0] - y[2]) - (x[0] - x[2]) * (y[3] - y[2]);
    result_ = (x_[3] - x_[1]) * (y_[0] - y_[2]) - (x_[0] - x_[2]) * (y_[3] - y_[2]);

    if (result * result_ <= 0)
        return 0;

    result = (x[0] - x[3]) * (y[1] - y[3]) - (x[1] - x[3]) * (y[0] - y[3]);
    result_ = (x_[0] - x_[3]) * (y_[1] - y_[3]) - (x_[1] - x_[3]) * (y_[0] - y_[3]);

    if (result * result_ <= 0)
        return 0;

    return 1;
}

static int CheckRotationConsistency_disperse(float* srcPoints, float* dstPoints, int* matchLoc, int* sample, int maxPointNum) {

#undef FUNC_CODE
#define FUNC_CODE 0x0C

    int k = 0, i = 0;
    int srcIndex = 0, dstIndex = 0;
    float x[4], y[4], x_[4], y_[4];
    float result = 0.f, result_ = 0.f;
    for (i = 0; i < 4; i++)
    {
        k = sample[i];
        srcIndex = matchLoc[k];
        dstIndex = matchLoc[maxPointNum + k];
        x[i] = srcPoints[srcIndex];
        y[i] = srcPoints[srcIndex + maxPointNum];
        x_[i] = dstPoints[dstIndex];
        y_[i] = dstPoints[dstIndex + maxPointNum];
    }

    result = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
    result_ = (x_[1] - x_[0]) * (y_[2] - y_[0]) - (x_[2] - x_[0]) * (y_[1] - y_[0]);

    if (result * result_ <= 0)
        return 0;

    result = (x[2] - x[1]) * (y[3] - y[1]) - (x[3] - x[1]) * (y[2] - y[1]);
    result_ = (x_[2] - x_[1]) * (y_[3] - y_[1]) - (x_[3] - x_[1]) * (y_[2] - y_[1]);

    if (result * result_ <= 0)
        return 0;

    result = (x[3] - x[1]) * (y[0] - y[2]) - (x[0] - x[2]) * (y[3] - y[2]);
    result_ = (x_[3] - x_[1]) * (y_[0] - y_[2]) - (x_[0] - x_[2]) * (y_[3] - y_[2]);

    if (result * result_ <= 0)
        return 0;

    result = (x[0] - x[3]) * (y[1] - y[3]) - (x[1] - x[3]) * (y[0] - y[3]);
    result_ = (x_[0] - x_[3]) * (y_[1] - y_[3]) - (x_[1] - x_[3]) * (y_[0] - y_[3]);

    if (result * result_ <= 0)
        return 0;

    return 1;
}

int Consistency_Rand_Check(float* srcPoints, float* dstPoints, int num, pConsistency_Rand_Info pPoseAnalysisInfo) {

#undef FUNC_CODE
#define FUNC_CODE 0x0D

    int in = 0;
    int in_max = 0;
    int* _consensus = pPoseAnalysisInfo->tConsensus1;
    int* best_consensus = NULL;
    int m, n, lastNum = 0, k;
    int threshold = pPoseAnalysisInfo->pRandTime >> 1;

    float coarse = pPoseAnalysisInfo->pCoarseThreshold;
    float fine = pPoseAnalysisInfo->pFineThreshold;

    if (num < 6) {
        pPoseAnalysisInfo->mConsNum = 0;
        return ERROR_CODE(0x001 | ERROR_OWNER);
    }
    else if (num > pPoseAnalysisInfo->pMaxPointNum) {//���ƥ������������100��  ��������λ100
        num = pPoseAnalysisInfo->pMaxPointNum;
    }

    memset(pPoseAnalysisInfo->tConsTimes, 0, sizeof(int) * pPoseAnalysisInfo->pMaxPointNum);

    srand((unsigned int)time(NULL));
    for (n = 0; n < pPoseAnalysisInfo->pRandTime; n++) {
        for (m = 0; m < 20; m++) {
            Rand_Gen_Four_Diff_Nums(num, pPoseAnalysisInfo->tSample, pPoseAnalysisInfo->tSampled);

            setComputeH_Direct_Data(srcPoints, dstPoints, pPoseAnalysisInfo->tSample, 4, pPoseAnalysisInfo->tComputeHData);
            ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);

            in = Find_consensus(_consensus, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, coarse);//���������㼯���任�����������������ǰһ�¼�consensus������һ�¼���Ԫ�ظ�����in
            if (in > in_max) {
                best_consensus = _consensus;
                in_max = in;

                if (_consensus == pPoseAnalysisInfo->tConsensus1)
                    _consensus = pPoseAnalysisInfo->tConsensus2;
                else
                    _consensus = pPoseAnalysisInfo->tConsensus1;
            }
        }

        lastNum = 0;
        for (m = 0; m < 10; m++) {
            setComputeH_Direct_Data(srcPoints, dstPoints, best_consensus, in_max, pPoseAnalysisInfo->tComputeHData);
            ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);

            in = Find_consensus(best_consensus, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, fine);

            if (in == lastNum)
                break;
            lastNum = in;
        }

        for (m = 0; m < in; m++) {
            k = best_consensus[m];
            pPoseAnalysisInfo->tConsTimes[k] ++;
        }
    }

    if (in_max <= 5) {
        pPoseAnalysisInfo->mConsNum = 0;
        return 0;
    }

    in_max = 0;
    for (m = 0; m < num; m++) {
        if (pPoseAnalysisInfo->tConsTimes[m] > threshold) {
            best_consensus[in_max] = m;
            in_max++;
        }
    }

    if (in_max > 5) {
        setComputeH_Direct_Data(srcPoints, dstPoints, best_consensus, in_max, pPoseAnalysisInfo->tComputeHData);
        ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
        pPoseAnalysisInfo->mConsLoc = best_consensus;
        pPoseAnalysisInfo->mConsNum = in_max;
    }
    else {
        pPoseAnalysisInfo->mConsNum = 0;
    }

    return 0;
}

int Consistency_Rand_Check_RotationConsistency(float* srcPoints, float* dstPoints, int num, pConsistency_Rand_Info pPoseAnalysisInfo) {

#undef FUNC_CODE
#define FUNC_CODE 0x0E

    int in = 0;
    int in_max = 0;
    int* _consensus = pPoseAnalysisInfo->tConsensus1;
    int* best_consensus = NULL;
    int n;

    float coarse = pPoseAnalysisInfo->pCoarseThreshold;
    float fine = pPoseAnalysisInfo->pFineThreshold;

    int fourPointIsOk = 0;
    if (num < 6) {
        pPoseAnalysisInfo->mConsNum = 0;
        return ERROR_CODE(0x001 | ERROR_OWNER);
    }
    else if (num > pPoseAnalysisInfo->pMaxPointNum) {
        num = pPoseAnalysisInfo->pMaxPointNum;
    }

    srand((unsigned int)time(NULL));
    for (n = 0; n < pPoseAnalysisInfo->pRandTime; n++) {
        Rand_Gen_Four_Diff_Nums(num, pPoseAnalysisInfo->tSample, pPoseAnalysisInfo->tSampled);

        fourPointIsOk = CheckRotationConsistency(srcPoints, dstPoints, pPoseAnalysisInfo->tSample);
        if (fourPointIsOk == 0)
            continue;

        setComputeH_Direct_Data(srcPoints, dstPoints, pPoseAnalysisInfo->tSample, 4, pPoseAnalysisInfo->tComputeHData);
        ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);

        in = Find_consensus(_consensus, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, coarse);
        if (in > in_max) {
            best_consensus = _consensus;
            in_max = in;
            if (_consensus == pPoseAnalysisInfo->tConsensus1)
                _consensus = pPoseAnalysisInfo->tConsensus2;
            else
                _consensus = pPoseAnalysisInfo->tConsensus1;
        }
    }

    if (in_max <= 5) {
        pPoseAnalysisInfo->mConsNum = 0;
        return 0;
    }

    while (1) {
        setComputeH_Direct_Data(srcPoints, dstPoints, best_consensus, in_max, pPoseAnalysisInfo->tComputeHData);
        ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
        in = Find_consensus(best_consensus, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, fine);
        if(in <= in_max) {
            in_max = in;
            break;
        }
        else {
            in_max = in;
        }
    }

    if (in_max > 5) {
        setComputeH_Direct_Data(srcPoints, dstPoints, best_consensus, in_max, pPoseAnalysisInfo->tComputeHData);
        ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
        pPoseAnalysisInfo->mConsLoc = best_consensus;
        pPoseAnalysisInfo->mConsNum = in_max;
    }
    else {
        pPoseAnalysisInfo->mConsNum = 0;
    }

    return 0;
}

int Consistency_MLESAC(float* srcPoints, float* dstPoints, int num, pConsistency_Rand_Info pPoseAnalysisInfo) {

#undef FUNC_CODE
#define FUNC_CODE 0x11

    int* _consensus = pPoseAnalysisInfo->tConsensus1;
    int* best_consensus = NULL;
    int n;

    float tScore = 0.0f, minScore = 1000000.0f;
    int tNum = 0, minNum = 0;

    float fine = pPoseAnalysisInfo->pFineThreshold;
    float coarse = pPoseAnalysisInfo->pCoarseThreshold;

    int fourPointIsOk = 0;
    if (num < 6) {
        pPoseAnalysisInfo->mConsNum = 0;
        return ERROR_CODE(0x001 | ERROR_OWNER);
    }
    else if (num > pPoseAnalysisInfo->pMaxPointNum) {
        num = pPoseAnalysisInfo->pMaxPointNum;
    }

    srand((unsigned int)time(NULL));
    for (n = 0; n < pPoseAnalysisInfo->pRandTime; n++) {
        Rand_Gen_Four_Diff_Nums(num, pPoseAnalysisInfo->tSample, pPoseAnalysisInfo->tSampled);

        fourPointIsOk = CheckRotationConsistency(srcPoints, dstPoints, pPoseAnalysisInfo->tSample);
        if (fourPointIsOk == 0)
            continue;

        setComputeH_Direct_Data(srcPoints, dstPoints, pPoseAnalysisInfo->tSample, 4, pPoseAnalysisInfo->tComputeHData);
        ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);

        tNum = MLESAC_consensus(_consensus, &tScore, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, coarse);
        if (tScore < minScore) {
            best_consensus = _consensus;
            minNum = tNum;
            minScore = tScore;
            if (_consensus == pPoseAnalysisInfo->tConsensus1)
                _consensus = pPoseAnalysisInfo->tConsensus2;
            else
                _consensus = pPoseAnalysisInfo->tConsensus1;
        }
    }

    if (minNum > 5) {
        setComputeH_Direct_Data(srcPoints, dstPoints, best_consensus, minNum, pPoseAnalysisInfo->tComputeHData);
        ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
        minNum = MLESAC_consensus(_consensus, &tScore, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, fine);
        best_consensus = _consensus;
        if (_consensus == pPoseAnalysisInfo->tConsensus1)
            _consensus = pPoseAnalysisInfo->tConsensus2;
        else
            _consensus = pPoseAnalysisInfo->tConsensus1;

        for (n = 0; n < 2; n++) {
            if (minNum > 5) {
                setComputeH_Direct_Data(srcPoints, dstPoints, best_consensus, minNum, pPoseAnalysisInfo->tComputeHData);
                ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
                minNum = MLESAC_consensus(_consensus, &tScore, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, fine);
                best_consensus = _consensus;
                if (_consensus == pPoseAnalysisInfo->tConsensus1)
                    _consensus = pPoseAnalysisInfo->tConsensus2;
                else
                    _consensus = pPoseAnalysisInfo->tConsensus1;
            }
        }

        if (minNum > 5) {
            setComputeH_Direct_Data(srcPoints, dstPoints, best_consensus, minNum, pPoseAnalysisInfo->tComputeHData);
            ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
            pPoseAnalysisInfo->mConsLoc = best_consensus;
            pPoseAnalysisInfo->mConsNum = minNum;
        }
        else {
            pPoseAnalysisInfo->mConsNum = 0;
        }
    }
    else {
        pPoseAnalysisInfo->mConsNum = 0;
    }

    return 0;
}

int Consistency_Rand_Check_disperse(float* srcPoints, float* dstPoints, int* matchLoc, int num, pConsistency_Rand_Info pPoseAnalysisInfo) {

#undef FUNC_CODE
#define FUNC_CODE 0x0F

    int in = 0;
    int in_max = 0;
    int* _consensus = pPoseAnalysisInfo->tConsensus1;
    int* best_consensus = NULL;
    int m, n, lastNum = 0, k;
    int threshold = pPoseAnalysisInfo->pRandTime >> 1;

    float coarse = pPoseAnalysisInfo->pCoarseThreshold;
    float fine = pPoseAnalysisInfo->pFineThreshold;

    if (num < 6) {
        pPoseAnalysisInfo->mConsNum = 0;
        return ERROR_CODE(0x001 | ERROR_OWNER);
    }
    else if (num > pPoseAnalysisInfo->pMaxPointNum) {//���ƥ������������100��  ��������λ100
        num = pPoseAnalysisInfo->pMaxPointNum;
    }

    memset(pPoseAnalysisInfo->tConsTimes, 0, sizeof(int) * pPoseAnalysisInfo->pMaxPointNum);

    srand((unsigned int)time(NULL));
    for (n = 0; n < pPoseAnalysisInfo->pRandTime; n++) {
        for (m = 0; m < 20; m++) {
            Rand_Gen_Four_Diff_Nums(num, pPoseAnalysisInfo->tSample, pPoseAnalysisInfo->tSampled);

            setComputeH_Direct_Data_disperse(srcPoints, dstPoints, matchLoc, pPoseAnalysisInfo->tSample, 4, pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->pMaxPointNum);
            ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);

            in = Find_consensus_disperse(_consensus, matchLoc, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, pPoseAnalysisInfo->pMaxPointNum, coarse);
            if (in > in_max){
                best_consensus = _consensus;
                in_max = in;

                if (_consensus == pPoseAnalysisInfo->tConsensus1)
                    _consensus = pPoseAnalysisInfo->tConsensus2;
                else
                    _consensus = pPoseAnalysisInfo->tConsensus1;
            }
        }

        lastNum = 0;
        for (m = 0; m < 10; m++) {
            setComputeH_Direct_Data_disperse(srcPoints, dstPoints, matchLoc, best_consensus, in_max, pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->pMaxPointNum);
            ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);

            in = Find_consensus_disperse(best_consensus, matchLoc, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, pPoseAnalysisInfo->pMaxPointNum, fine);

            if (in == lastNum)
                break;
            lastNum = in;
        }

        for (m = 0; m < in; m++) {
            k = best_consensus[m];
            pPoseAnalysisInfo->tConsTimes[k] ++;
        }
    }

    if (in_max <= 5) {
        pPoseAnalysisInfo->mConsNum = 0;
        return 0;
    }

    /***�öδ��������������*********/
    in_max = 0;
    for (m = 0; m < num; m++) {
        if (pPoseAnalysisInfo->tConsTimes[m] > threshold) {
            best_consensus[in_max] = m;
            in_max++;
        }
    }
    /*********************************/
    if (in_max > 5) {
        setComputeH_Direct_Data_disperse(srcPoints, dstPoints, matchLoc, best_consensus, in_max, pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->pMaxPointNum);
        ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
        pPoseAnalysisInfo->mConsLoc = best_consensus;
        pPoseAnalysisInfo->mConsNum = in_max;
    }
    else {
        pPoseAnalysisInfo->mConsNum = 0;
    }
    return 0;
}

int Consistency_Rand_Check_RotationConsistency_disperse(float* srcPoints, float* dstPoints, int* matchLoc, int num, pConsistency_Rand_Info pPoseAnalysisInfo) {

#undef FUNC_CODE
#define FUNC_CODE 0x10

    int in = 0;
    int in_max = 0;
    int* _consensus = pPoseAnalysisInfo->tConsensus1;
    int* best_consensus = NULL;
    int n;
    int fourPointIsOk = 0;
#if FLOW_TRACK_SWITCH
    int realRandCount = 0;
#endif

    float coarse = pPoseAnalysisInfo->pCoarseThreshold;
    float fine = pPoseAnalysisInfo->pFineThreshold;

    if (num < 6) {
        pPoseAnalysisInfo->mConsNum = 0;
        return ERROR_CODE(0x001 | ERROR_OWNER);
    }
    else if (num > pPoseAnalysisInfo->pMaxPointNum) {//���ƥ������������100��  ��������λ100
        num = pPoseAnalysisInfo->pMaxPointNum;
    }

    srand((unsigned int)time(NULL));

    for (n = 0; n < pPoseAnalysisInfo->pRandTime; n++) {

        Rand_Gen_Four_Diff_Nums(num, pPoseAnalysisInfo->tSample, pPoseAnalysisInfo->tSampled);

        fourPointIsOk = CheckRotationConsistency_disperse(srcPoints, dstPoints, matchLoc, pPoseAnalysisInfo->tSample, pPoseAnalysisInfo->pMaxPointNum);
        if (fourPointIsOk == 0)
            continue;

#if FLOW_TRACK_SWITCH
        realRandCount ++;
#endif
        setComputeH_Direct_Data_disperse(srcPoints, dstPoints, matchLoc, pPoseAnalysisInfo->tSample, 4, pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->pMaxPointNum);
        ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
        in = Find_consensus_disperse(_consensus, matchLoc, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, pPoseAnalysisInfo->pMaxPointNum, coarse);
        if (in > in_max) {
            best_consensus = _consensus;
            in_max = in;
            if (_consensus == pPoseAnalysisInfo->tConsensus1)
                _consensus = pPoseAnalysisInfo->tConsensus2;
            else
                _consensus = pPoseAnalysisInfo->tConsensus1;
        }
    }
#if FLOW_TRACK_SWITCH
    LOGE("realRandCount = %d.\n", realRandCount);
#endif

    if (in_max <= 5) {
        pPoseAnalysisInfo->mConsNum = 0;
        return 0;
    }
    while (1) {
        setComputeH_Direct_Data_disperse(srcPoints, dstPoints, matchLoc, best_consensus, in_max, pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->pMaxPointNum);
        ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
        in = Find_consensus_disperse(best_consensus, matchLoc, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, pPoseAnalysisInfo->pMaxPointNum, fine);
        if (in <= in_max) {
            in_max = in;
            break;
        }
        else {
            in_max = in;
        }
    }

    if (in_max > 5) {
        setComputeH_Direct_Data_disperse(srcPoints, dstPoints, matchLoc, best_consensus, in_max, pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->pMaxPointNum);
        ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
        pPoseAnalysisInfo->mConsLoc = best_consensus;
        pPoseAnalysisInfo->mConsNum = in_max;
    }
    else {
        pPoseAnalysisInfo->mConsNum = 0;
    }
    return 0;
}

int Consistency_MLESAC_disperse(float* srcPoints, float* dstPoints, int* matchLoc, int num, pConsistency_Rand_Info pPoseAnalysisInfo) {

#undef FUNC_CODE
#define FUNC_CODE 0x11

    int* _consensus = pPoseAnalysisInfo->tConsensus1;
    int* best_consensus = NULL;
    int n;

    float tScore = 0.0f, minScore = 1000000.0f;
    int tNum = 0, minNum = 0;

    float fine = pPoseAnalysisInfo->pFineThreshold;
    float coarse = pPoseAnalysisInfo->pCoarseThreshold;

    int fourPointIsOk = 0;
    if (num < 6) {
        pPoseAnalysisInfo->mConsNum = 0;
        return ERROR_CODE(0x001 | ERROR_OWNER);
    }
    else if (num > pPoseAnalysisInfo->pMaxPointNum) {
        num = pPoseAnalysisInfo->pMaxPointNum;
    }

    srand((unsigned int)time(NULL));
    for (n = 0; n < pPoseAnalysisInfo->pRandTime; n++) {
        Rand_Gen_Four_Diff_Nums(num, pPoseAnalysisInfo->tSample, pPoseAnalysisInfo->tSampled);

        fourPointIsOk = CheckRotationConsistency_disperse(srcPoints, dstPoints, matchLoc, pPoseAnalysisInfo->tSample, pPoseAnalysisInfo->pMaxPointNum);
        if (fourPointIsOk == 0)
            continue;

        setComputeH_Direct_Data_disperse(srcPoints, dstPoints, matchLoc, pPoseAnalysisInfo->tSample, 4, pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->pMaxPointNum);
        ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);

        tNum = MLESAC_consensus_disperse(_consensus, &tScore, matchLoc, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, pPoseAnalysisInfo->pMaxPointNum, coarse);
        if (tScore < minScore) {
            best_consensus = _consensus;
            minNum = tNum;
            minScore = tScore;
            if (_consensus == pPoseAnalysisInfo->tConsensus1)
                _consensus = pPoseAnalysisInfo->tConsensus2;
            else
                _consensus = pPoseAnalysisInfo->tConsensus1;
        }
    }

    if (minNum > 5) {
        setComputeH_Direct_Data_disperse(srcPoints, dstPoints, matchLoc, best_consensus, minNum, pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->pMaxPointNum);
        ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
        minNum = MLESAC_consensus_disperse(_consensus, &tScore, matchLoc, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, pPoseAnalysisInfo->pMaxPointNum, fine);
        best_consensus = _consensus;
        if (_consensus == pPoseAnalysisInfo->tConsensus1)
            _consensus = pPoseAnalysisInfo->tConsensus2;
        else
            _consensus = pPoseAnalysisInfo->tConsensus1;

        for (n = 0; n < 2; n++) {
            if (minNum > 5) {
                setComputeH_Direct_Data_disperse(srcPoints, dstPoints, matchLoc, best_consensus, minNum, pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->pMaxPointNum);
                ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
                minNum = MLESAC_consensus_disperse(_consensus, &tScore, matchLoc, srcPoints, dstPoints, pPoseAnalysisInfo->mH, num, pPoseAnalysisInfo->pMaxPointNum, fine);
                best_consensus = _consensus;
                if (_consensus == pPoseAnalysisInfo->tConsensus1)
                    _consensus = pPoseAnalysisInfo->tConsensus2;
                else
                    _consensus = pPoseAnalysisInfo->tConsensus1;
            }
        }

        if (minNum > 5) {
            setComputeH_Direct_Data_disperse(srcPoints, dstPoints, matchLoc, best_consensus, minNum, pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->pMaxPointNum);
            ComputeH_Direct(pPoseAnalysisInfo->tComputeHData, pPoseAnalysisInfo->mH);
            pPoseAnalysisInfo->mConsLoc = best_consensus;
            pPoseAnalysisInfo->mConsNum = minNum;
        }
        else {
            pPoseAnalysisInfo->mConsNum = 0;
        }
    }
    else {
        pPoseAnalysisInfo->mConsNum = 0;
    }

    return 0;
}