#ifndef _ALVA_CL_ENV_H_
#define _ALVA_CL_ENV_H_

#ifdef AlvaPublic
#include "OclHelper_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct CL_ENV_ {
        cl_platform_id* platform;
        cl_uint numOfPlatform;
        cl_device_id* device;
        cl_uint numOfDevice;
        cl_context ctx;
        cl_command_queue* cmd_queue;
		int flag;
        int inited;
    }cl_env;

    cl_env* getCLEnv();
    int initCLEnv(alvadt dataType);
    int unitCLEnv();

#ifdef __cplusplus
}
#endif

#endif