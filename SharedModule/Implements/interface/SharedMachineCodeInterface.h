#ifndef _SHARED_MACHINE_CODE_INTERFACE_H_
#define _SHARED_MACHINE_CODE_INTERFACE_H_

#ifdef __cplusplus
extern "C" 
{
#endif
    /*
     * cGetMachineCode
     * 功能：获取机器码
     * @param code: 机器码，用于获取授权，用户分配内存，最小 41 bytes
     */
    EXPORT_API int cGetMachineCode(unsigned char* code);
#ifdef __cplusplus
}
#endif

#endif
