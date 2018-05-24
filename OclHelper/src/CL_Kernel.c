#include <malloc.h>
#include <stdio.h>

#include "Config.h"
#include "Format.h"

#define LOG_TAG "CL_Kernel"
#include "Log.h"
#include "ErrorTools.h"

#include "SafeCL.h"

#include "CL_Env.h"

#include "CL_Kernel.h"

#undef FILE_NUM
#define FILE_NUM 0xB200

static cl_kernel kernelList[MaxKernelNum] = {0};

cl_kernel* getKernelList(){

#undef FUNC_CODE
#define FUNC_CODE 0x01

    return kernelList;
}

int InitKernelList(char* key, int type, int kernelNum, const char* kernelName[], const char* kernelSrc[], const size_t kernelLength[], char* buildOpt){

#undef FUNC_CODE
#define FUNC_CODE 0x02

    cl_int err = 0;
    int i = 0;
    cl_uint j = 0;
    cl_build_status status = 0;
    
    char* buildLog = NULL;
    size_t logSize = 0;
    
    cl_device_type dt = 0;
    
    cl_env* env = NULL;
    cl_program m_program = NULL;

    if(kernelNum > MaxKernelNum){
       OWN_ERROR_RETURN(0x001, "too many kernels to initialize.");
    }

    env = getCLEnv();
    
    m_program = flCreateProgramWithSource(env->ctx, kernelNum, kernelSrc, NULL, &err);
    
    CHECK_OPENCL_ERROR(err, "clCreateProgramWithSource");
    
    err = flBuildProgram(m_program, env->numOfDevice, env->device, buildOpt, NULL, NULL);
    
    if(CL_SUCCESS != err){
        for(j = 0; j < env->numOfDevice; j ++){
            flGetProgramBuildInfo(m_program, env->device[j], CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &status, NULL);
            if(CL_SUCCESS != status){
                LOGI("Device %d info:\n", j);
                flGetDeviceInfo(env->device[j], CL_DEVICE_TYPE, sizeof(cl_device_type), &dt, NULL);
                LOGI("    Type: %s\n", dt == CL_DEVICE_TYPE_CPU ? "CL_DEVICE_TYPE_CPU" : 
                    (dt == CL_DEVICE_TYPE_GPU ? "CL_DEVICE_TYPE_GPU" : 
                    (dt == CL_DEVICE_TYPE_ACCELERATOR ? "CL_DEVICE_TYPE_ACCELERATOR" : 
                    (dt == CL_DEVICE_TYPE_DEFAULT ? "CL_DEVICE_TYPE_DEFAULT" : "CL_DEVICE_TYPE_CUSTOM"))));
                flGetDeviceInfo(env->device[j], CL_DEVICE_VENDOR, 0, NULL, &logSize);
                buildLog = (char*)malloc(logSize + 1);
                flGetDeviceInfo(env->device[j], CL_DEVICE_VENDOR, logSize + 1, buildLog, NULL);
                LOGI("    Vendor: %s\n", buildLog);
                if(NULL != buildLog)
                    free(buildLog);
                flGetDeviceInfo(env->device[j], CL_DEVICE_NAME,0, NULL, &logSize);
                buildLog = (char*)malloc(logSize + 1);
                flGetDeviceInfo(env->device[j], CL_DEVICE_NAME, logSize + 1, buildLog, NULL);
                LOGI("    Name: %s\n", buildLog);
                if(NULL != buildLog)
                    free(buildLog);
                flGetDeviceInfo(env->device[j], CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &logSize);
                buildLog = (char*)malloc(logSize + 1);
                flGetDeviceInfo(env->device[j], CL_DEVICE_OPENCL_C_VERSION, logSize + 1, buildLog, NULL);
                LOGI("    OpenCL C Version: %s\n", buildLog);
                if(NULL != buildLog)
                    free(buildLog);

#if DebugInfo_Switch == Debug_Alva
                flGetProgramBuildInfo(m_program, env->device[j], CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
                buildLog = (char*)malloc(logSize + 1);
                flGetProgramBuildInfo(m_program, env->device[j], CL_PROGRAM_BUILD_LOG, logSize + 1, buildLog, NULL);
                LOGE("build error on device %d:\n", j);
                LOGE("build Log: %s\n", buildLog);
                if(NULL != buildLog)
                    free(buildLog);
#endif
            }
        }
        
        CHECK_OPENCL_ERROR(err, "clBuildProgram");
    }
    
    for(i = 0; i < kernelNum; i ++){
        kernelList[i] = flCreateKernel(m_program, kernelName[i], &err);
        CHECK_OPENCL_ERROR(err, kernelName[i]);
    }
    
    flReleaseProgram(m_program);
    
    return 0;
}

int UnitKernelList(){

#undef FUNC_CODE
#define FUNC_CODE 0x03

    int i = 0;
    for(i = 0; i < MaxKernelNum; i ++){
        flReleaseKernel(kernelList[i]);
        kernelList[i] = 0;
    }
    return 0;
}