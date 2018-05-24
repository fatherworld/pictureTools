#ifndef _ALVA_CL_MEM_H_
#define _ALVA_CL_MEM_H_

#ifdef AlvaPublic
#include "OclHelper_Hide.h"
#endif

#ifdef __cplusplus
#error "Do Not Use CL_Mem.h in .cpp file. CL_Mem.h support only .c file."
#endif

/*
 * please define buffers and images like following format in implementing file:
 *
 *   typedef enum _MEM_LIST_{
 *       MEM_Start                     = -1,
 *       //...
 *       MEM_Num                           ,
 *   }MEM_LIST;
 */

/*
 * MaxMemNum
 * @AddBuffers and @AddImages的num的总和最大值
 */
#define MaxMemNum 50000

typedef struct _BufferInfo_{
    int index;                          //BufferList中的名字
    cl_mem_flags flags;                 //flags
    int pyramidNum;                     //此buffer名包含的金字塔层数
    size_t size[];                      //每层金字塔的大小
}BufferInfo, *pBufferInfo;

typedef struct _ImageSpecial_ {
    size_t width;
    size_t height;
    size_t image_row_pitch;
    cl_mem buffer;                      //copy memory
}ImageSpecial;

typedef struct _ImageInfo_{
    int index;                          //ImageList中的名字
    cl_mem_flags flags;                 //flags
    int pyramidNum;                     //此buffer名包含的金字塔层数
    cl_image_format format;             //format
    cl_mem_object_type type;            //type
    ImageSpecial special[];             //每层金字塔的独立内容
}ImageInfo, *pImageInfo;

cl_mem* getMemList();

int AddBuffers(int num, pBufferInfo infolist);
int AddImages(int num, pImageInfo infolist);

int WriteBuffer(int memName, int blocking_write, size_t size, void* data);
int ReadBuffer(int memName, int blocking_write, size_t size,  void* data);

int UnitOneMem(int index);

int UnitMem(void);

#endif