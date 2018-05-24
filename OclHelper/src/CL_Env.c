#include "Config.h"

#include <stdlib.h>
#include "string_fl.h"
#include "stdio_fl.h"

#include "Format.h"

#if OS_ALVA == OS_Android
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#define LOG_TAG "CL_Env"
#include "Log.h"
#include "ErrorTools.h"

#include "Module.h"
#include "SafeCL_GL.h"

#include "CL_Env.h"

#undef FILE_NUM
#define FILE_NUM 0xB100

#if OS_ALVA == OS_Windows
#pragma warning( disable : 4996 )
#endif
static cl_env mEnv = {0};

cl_env* getCLEnv(){

#undef FUNC_CODE
#define FUNC_CODE 0x01

    if(mEnv.inited == 1)
        return &mEnv;
    else
        return NULL;
}

static int initPlatform(){

#undef FUNC_CODE
#define FUNC_CODE 0x02

    cl_int err = 0;
    err = flGetPlatformIDs(0, NULL, &(mEnv.numOfPlatform));
    CHECK_OPENCL_ERROR(err, "clGetPlatformIDs");

    mEnv.platform = (cl_platform_id*)malloc(sizeof(cl_platform_id) * mEnv.numOfPlatform);
    if(NULL == mEnv.platform){
        LOGE("malloc err, size: %zd\n", sizeof(cl_platform_id) * mEnv.numOfPlatform);
        OWN_ERROR_RETURN(0x001, "malloc error");
    }
    
    err = flGetPlatformIDs(mEnv.numOfPlatform, mEnv.platform, NULL);
    CHECK_OPENCL_ERROR(err, "clGetPlatformIDs");
    
    return 0;
}

static int initDevices(){

#undef FUNC_CODE
#define FUNC_CODE 0x03

    cl_int err = 0;
    cl_uint i = 0;
    cl_platform_id temp;
    size_t extensions_len;
    char extensions[2048];
	char* cl_version = "OpenCL 1.1";
    
    cl_ulong localSize;
    
    for(i = 0; i < mEnv.numOfPlatform; i ++){
        err = flGetDeviceIDs(mEnv.platform[i], CL_DEVICE_TYPE_GPU, 0, NULL, &mEnv.numOfDevice);
        
        if (CL_DEVICE_NOT_FOUND == err) {
            break;
        }

        CHECK_OPENCL_ERROR(err, "clGetDeviceIDs");

        if(0 != mEnv.numOfDevice){
            mEnv.device = (cl_device_id*)malloc(mEnv.numOfDevice * sizeof(cl_device_id));
            if(NULL == mEnv.device){
                LOGE("malloc err, size: %zd\n", mEnv.numOfDevice * sizeof(cl_device_id));
                OWN_ERROR_RETURN(0x001, "malloc error");
            }
            err = flGetDeviceIDs(mEnv.platform[i], CL_DEVICE_TYPE_GPU, mEnv.numOfDevice, mEnv.device, 0);
            CHECK_OPENCL_ERROR(err, "clGetDeviceIDs");
            
            temp = mEnv.platform[0];
            mEnv.platform[0] = mEnv.platform[i];
            mEnv.platform[i] = temp;
            
            break;
        }
    }
    
    if (0 != mEnv.numOfDevice) {
        err = flGetDeviceInfo(mEnv.device[0],
                              CL_DEVICE_EXTENSIONS,
                              0,
                              NULL,
                              &extensions_len);
        CHECK_OPENCL_ERROR(err, "clGetDeviceInfo CL_DEVICE_EXTENSIONS length");

        err = flGetDeviceInfo(mEnv.device[0],
                              CL_DEVICE_EXTENSIONS,
                              extensions_len,
                              extensions,
                              NULL);
        CHECK_OPENCL_ERROR(err, "clGetDeviceInfo CL_DEVICE_EXTENSIONS");
#if FLOW_TRACK_SWITCH
        LOGE("CL_DEVICE_EXTENSIONS: %s\n", extensions);
#endif

        err = flGetDeviceInfo(mEnv.device[0],
                              CL_DEVICE_VERSION,
                              0,
                              NULL,
                              &extensions_len);
        CHECK_OPENCL_ERROR(err, "clGetDeviceInfo CL_DEVICE_VERSION length");

        err = flGetDeviceInfo(mEnv.device[0],
                              CL_DEVICE_VERSION,
                              extensions_len,
                              extensions,
                              NULL);
        CHECK_OPENCL_ERROR(err, "clGetDeviceInfo CL_DEVICE_VERSION");
#if FLOW_TRACK_SWITCH
        LOGE("DEVICE_VERSION:%s\n", extensions);
#endif

        if (strncmp(cl_version, extensions, strlen(cl_version)) >= 0) {
            mEnv.flag = 0;
        }
        else {
            mEnv.flag = 1;
        }

        err = flGetDeviceInfo(mEnv.device[0],
                              CL_DEVICE_LOCAL_MEM_SIZE,
                              sizeof(cl_ulong),
                              &localSize,
                              NULL);
        CHECK_OPENCL_ERROR(err, "clGetDeviceInfo CL_DEVICE_LOCAL_MEM_SIZE");
#if FLOW_TRACK_SWITCH
        LOGE("CL_DEVICE_LOCAL_MEM_SIZE: %lld\n", localSize);
#endif

        return 0;
    }
    
    for(i = 0; i < mEnv.numOfPlatform; i ++){
        err = flGetDeviceIDs(mEnv.platform[i], CL_DEVICE_TYPE_CPU, 0, NULL, &mEnv.numOfDevice);
        
        if (CL_DEVICE_NOT_FOUND == err) {
            break;
        }

        CHECK_OPENCL_ERROR(err, "clGetDeviceIDs");

        if(0 != mEnv.numOfDevice){
            mEnv.device = (cl_device_id*)malloc(mEnv.numOfDevice * sizeof(cl_device_id));
            if(NULL == mEnv.device){
                LOGE("malloc err, size: %zd\n", mEnv.numOfDevice * sizeof(cl_device_id));
                OWN_ERROR_RETURN(0x002, "malloc error");
            }
            err = flGetDeviceIDs(mEnv.platform[i], CL_DEVICE_TYPE_CPU, mEnv.numOfDevice, mEnv.device, 0);
            CHECK_OPENCL_ERROR(err, "clGetDeviceIDs");
            
            temp = mEnv.platform[0];
            mEnv.platform[0] = mEnv.platform[i];
            mEnv.platform[i] = temp;
            
            break;
        }
    }
    
    if (0 != mEnv.numOfDevice) {
        err = flGetDeviceInfo(mEnv.device[0],
                              CL_DEVICE_EXTENSIONS,
                              0,
                              NULL,
                              &extensions_len);
        CHECK_OPENCL_ERROR(err, "clGetDeviceInfo CL_DEVICE_EXTENSIONS length");

        err = flGetDeviceInfo(mEnv.device[0],
                              CL_DEVICE_EXTENSIONS,
                              extensions_len,
                              extensions,
                              NULL);
        CHECK_OPENCL_ERROR(err, "clGetDeviceInfo CL_DEVICE_EXTENSIONS");
        LOGE("CL_DEVICE_EXTENSIONS: %s\n", extensions);

        err = flGetDeviceInfo(mEnv.device[0],
                              CL_DEVICE_VERSION,
                              0,
                              NULL,
                              &extensions_len);
        CHECK_OPENCL_ERROR(err, "clGetDeviceInfo CL_DEVICE_VERSION length");

        err = flGetDeviceInfo(mEnv.device[0],
                              CL_DEVICE_VERSION,
                              extensions_len,
                              extensions,
                              NULL);
        CHECK_OPENCL_ERROR(err, "clGetDeviceInfo CL_DEVICE_VERSION");
        LOGE("DEVICE_VERSION:%s\n", extensions);

        if (strncmp(cl_version, extensions, strlen(cl_version)) >= 0) {
            mEnv.flag = 0;
        }
        else {
            mEnv.flag = 1;
        }

        err = flGetDeviceInfo(mEnv.device[0],
                              CL_DEVICE_LOCAL_MEM_SIZE,
                              sizeof(cl_ulong),
                              &localSize,
                              NULL);
        CHECK_OPENCL_ERROR(err, "clGetDeviceInfo CL_DEVICE_LOCAL_MEM_SIZE");

        LOGE("CL_DEVICE_LOCAL_MEM_SIZE: %lld\n", localSize);

        return 0;
    }

    OWN_ERROR_RETURN(0x001, "There is not gpu/cpu device found on this platform\n");
}

static int initContext(alvadt dataType){

#undef FUNC_CODE
#define FUNC_CODE 0x04

    int err = -1;
    
#if OS_ALVA==OS_Android
    cl_context_properties prop_texture[] = {//获得OpenGL上下文
                                              CL_CONTEXT_PLATFORM, (cl_context_properties) mEnv.platform[0],
                                              CL_GL_CONTEXT_KHR,   (cl_context_properties) eglGetCurrentContext(),
                                              CL_EGL_DISPLAY_KHR,      (cl_context_properties) eglGetCurrentDisplay(),		//获得OpenGl设备信息
                                              0
                                           };
    
    cl_context_properties prop_memory[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties)mEnv.platform[0], 0};
    
    if (dataType == Alva_Texture){
		mEnv.ctx = flCreateContext(prop_texture, mEnv.numOfDevice, mEnv.device, NULL, NULL, &err);
        CHECK_OPENCL_ERROR(err, "clCreateContext Texture");
    }
    else if (dataType == Alva_Memory){
        mEnv.ctx = flCreateContext(prop_memory, mEnv.numOfDevice, mEnv.device, NULL, NULL, &err);	
        CHECK_OPENCL_ERROR(err, "clCreateContext buffer");
    }
    else{
        OWN_ERROR_RETURN(0x001, "parameter err not Alva_Memory or Alva_Texture\n");
    }
#else
    cl_context_properties prop_memory[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties)mEnv.platform[0], 0};
    if (dataType == Alva_Memory){
        mEnv.ctx = flCreateContext(prop_memory, mEnv.numOfDevice, mEnv.device, NULL, NULL, &err);	
        CHECK_OPENCL_ERROR(err, "clCreateContext buffer");
    }
    else if (dataType == Alva_Texture){
        OWN_ERROR_RETURN(0x001, "Alva_Texture is not supported beside on Android\n");
    }
    else{
        OWN_ERROR_RETURN(0x002, "parameter err not Alva_Memory\n");
    }
#endif
    
    return 0;
}

static int initCommandQueue(){
    
#undef FUNC_CODE
#define FUNC_CODE 0x05

    int err = 0;
    cl_uint i = 0;
    
    mEnv.cmd_queue = (cl_command_queue*)malloc(sizeof(cl_command_queue) * mEnv.numOfDevice);
    if(NULL == mEnv.cmd_queue){
        LOGE("malloc err, size: %zd\n", sizeof(cl_command_queue) * mEnv.numOfDevice);
        OWN_ERROR_RETURN(0x001, "malloc error\n");
    }
    
    for(i = 0; i < mEnv.numOfDevice; i ++){
        mEnv.cmd_queue[i] = flCreateCommandQueue(mEnv.ctx, mEnv.device[i], CL_QUEUE_PROFILING_ENABLE, &err);
        CHECK_OPENCL_ERROR(err, "clCreateCommandQueue");
    }
    
    return 0;
}

int initCLEnv(alvadt dataType){

#undef FUNC_CODE
#define FUNC_CODE 0x06

    ERROR_RETURN(InitCLFunctions(), "InitCLFunctions");

    ERROR_RETURN(initPlatform(), "initPlatform");
    
    ERROR_RETURN(initDevices(), "initDevices");
    
    ERROR_RETURN(initContext(dataType), "initContext");
    
    ERROR_RETURN(initCommandQueue(), "initCommandQueue");

    mEnv.inited = 1;
    
    return 0;
}

int unitCLEnv(){

#undef FUNC_CODE
#define FUNC_CODE 0x07

    cl_uint i = 0;
    
    for(i = 0; i < mEnv.numOfDevice; i ++){
        flReleaseCommandQueue(mEnv.cmd_queue[i]);
    }
    
    flReleaseContext(mEnv.ctx);
    
    memset(&mEnv, 0, sizeof(cl_env));
    
    ModuleFree();

    return 0;
}