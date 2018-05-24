#ifndef _ALVA_CL_KERNEL_H_
#define _ALVA_CL_KERNEL_H_

#ifdef AlvaPublic
#include "OclHelper_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * MaxKernelNum
     * @InitKenrelList��kernelNum���֧�ֵ�����
     */
#define MaxKernelNum 100

     /*
      * getKernelList
      * ���ܣ���ȡkernel�б�˳����kernelName˳����
      * @return��kernel�б�
      */
    cl_kernel* getKernelList();

    /*
     * InitKernelList
     * ���ܣ���ʼ��kernel�б�
     * @param key��ռλ
     * @param type: ռλ
     * @param kernelNum��kernelName��kernelSrc��kernelLength��kernel����
     * @param kernelName��kernelSrc��kernel�����б�kernel�б�Ҳ������˳�򴴽�
     * @param kernelSrc��kernelԴ������
     * @param kernelLength����kernelԴ��ĳ���
     * @param buildOpt������ѡ��
     * @return��0���ɹ���������ʧ��
     */
    int InitKernelList(char* key, int type, int kernelNum, const char* kernelName[], const char* kernelSrc[], const size_t kernelLength[], char* buildOpt);

    /*
     * UnitKernelList
     * ���ܣ��ͷ�Kernel�б���Դ
     * @return��0���ɹ���������ʧ��
     */
    int UnitKernelList();

#ifdef __cplusplus
}
#endif

#endif
