#include "Config.h"
#include "Format.h"

#include "stdio_fl.h"

#define LOG_TAG "CL_Mem"
#include "Log.h"
#include "ErrorTools.h"

#include "SafeCL_GL.h"

#include "CL_Env.h"

#include "CL_Mem.h"

static cl_mem memList[MaxMemNum] = {0};

#undef FILE_NUM
#define FILE_NUM 0xB300

cl_mem* getMemList(){

#undef FUNC_CODE 
#define FUNC_CODE 0x01

    return memList;
}

int AddBuffers(int num, pBufferInfo infolist){

#undef FUNC_CODE 
#define FUNC_CODE 0x02

    int i = 0, j = 0;
    int err = 0;
    cl_env* env = getCLEnv();

    for(i = 0; i < num; i ++){
        for(j = 0; j < infolist[i].pyramidNum; j ++){
            memList[infolist[i].index + j] = flCreateBuffer(env->ctx, infolist[i].flags, infolist[i].size[j], NULL, &err);
            CHECK_OPENCL_ERROR(err, "clCreateBuffer error");
        }
    }
    return 0;
}

int AddImages(int num, pImageInfo infolist){

#undef FUNC_CODE 
#define FUNC_CODE 0x03

    int i = 0, j = 0;
    int err = 0;

    cl_image_desc img_desc;

    cl_env* env = getCLEnv();

    img_desc.image_type = CL_MEM_OBJECT_IMAGE2D;
    img_desc.image_width = 0;
    img_desc.image_height = 0;
    img_desc.image_depth = 0;
    img_desc.image_array_size = 0;
    img_desc.image_row_pitch = 0;
    img_desc.image_slice_pitch = 0;
    img_desc.num_mip_levels = 0;
    img_desc.num_samples = 0;
    img_desc.buffer = NULL;

    for(i = 0; i < num; i ++){
        for(j = 0; j < infolist[i].pyramidNum; j ++){
            img_desc.image_width = infolist[i].special[j].width;
            img_desc.image_height = infolist[i].special[j].height;

            img_desc.image_row_pitch = infolist[i].special[j].image_row_pitch;
            img_desc.buffer = infolist[i].special[j].buffer;

			if (env->flag != 0){
				memList[infolist[i].index + j] = flCreateImage(env->ctx, infolist[i].flags, &(infolist[i].format), &img_desc, NULL, &err);
			}
			else{
				memList[infolist[i].index + j] = flCreateImage2D(env->ctx, infolist[i].flags, &(infolist[i].format), img_desc.image_width, img_desc.image_height, 0, NULL, &err);
			}
            CHECK_OPENCL_ERROR(err, "clCreateImage error");
        }
    }
    return 0;
}

int WriteBuffer(int memName, int blocking_write, size_t size, void* data){

#undef FUNC_CODE 
#define FUNC_CODE 0x04

    int err = -1;
    cl_env* env = getCLEnv();

    err = flEnqueueWriteBuffer(env->cmd_queue[0], memList[memName], blocking_write, 0, size, data, 0, NULL, NULL);
    CHECK_OPENCL_ERROR(err, "clEnqueueWriteBuffer error");

    return 0;
}

int ReadBuffer(int memName, int blocking_write, size_t size,  void* data){

#undef FUNC_CODE 
#define FUNC_CODE 0x05

    int err = -1;
    cl_env* env = getCLEnv();

    err = flEnqueueReadBuffer(env->cmd_queue[0], memList[memName], blocking_write, 0, size, data, 0, NULL, NULL);
    CHECK_OPENCL_ERROR(err, "clEnqueueWriteBuffer error");

    return 0;
}

int UnitOneMem(int index) {

#undef FUNC_CODE 
#define FUNC_CODE 0x06

    if (memList[index] != 0)
    {
        flReleaseMemObject(memList[index]);
        memList[index] = 0;
    }

    return 0;
}

int UnitMem(void){

#undef FUNC_CODE 
#define FUNC_CODE 0x07
    
    int i = 0; 
    for(i = 0; i < MaxMemNum; i ++){
        if (memList[i] != 0)
        {
            flReleaseMemObject(memList[i]);
            memList[i] = 0;
        }

    }
    return 0;
}