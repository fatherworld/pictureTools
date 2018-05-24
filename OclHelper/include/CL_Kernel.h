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
     * @InitKenrelList的kernelNum最大支持的数量
     */
#define MaxKernelNum 100

     /*
      * getKernelList
      * 功能：获取kernel列表，顺序由kernelName顺序定义
      * @return：kernel列表
      */
    cl_kernel* getKernelList();

    /*
     * InitKernelList
     * 功能：初始化kernel列表
     * @param key：占位
     * @param type: 占位
     * @param kernelNum：kernelName、kernelSrc、kernelLength中kernel数量
     * @param kernelName：kernelSrc中kernel名字列表，kernel列表也将按此顺序创建
     * @param kernelSrc：kernel源码数组
     * @param kernelLength：各kernel源码的长度
     * @param buildOpt：编译选项
     * @return：0：成功，其他：失败
     */
    int InitKernelList(char* key, int type, int kernelNum, const char* kernelName[], const char* kernelSrc[], const size_t kernelLength[], char* buildOpt);

    /*
     * UnitKernelList
     * 功能：释放Kernel列表资源
     * @return：0：成功，其他：失败
     */
    int UnitKernelList();

#ifdef __cplusplus
}
#endif

#endif
