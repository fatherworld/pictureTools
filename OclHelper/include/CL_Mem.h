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
 * @AddBuffers and @AddImages��num���ܺ����ֵ
 */
#define MaxMemNum 50000

typedef struct _BufferInfo_{
    int index;                          //BufferList�е�����
    cl_mem_flags flags;                 //flags
    int pyramidNum;                     //��buffer�������Ľ���������
    size_t size[];                      //ÿ��������Ĵ�С
}BufferInfo, *pBufferInfo;

typedef struct _ImageSpecial_ {
    size_t width;
    size_t height;
    size_t image_row_pitch;
    cl_mem buffer;                      //copy memory
}ImageSpecial;

typedef struct _ImageInfo_{
    int index;                          //ImageList�е�����
    cl_mem_flags flags;                 //flags
    int pyramidNum;                     //��buffer�������Ľ���������
    cl_image_format format;             //format
    cl_mem_object_type type;            //type
    ImageSpecial special[];             //ÿ��������Ķ�������
}ImageInfo, *pImageInfo;

cl_mem* getMemList();

int AddBuffers(int num, pBufferInfo infolist);
int AddImages(int num, pImageInfo infolist);

int WriteBuffer(int memName, int blocking_write, size_t size, void* data);
int ReadBuffer(int memName, int blocking_write, size_t size,  void* data);

int UnitOneMem(int index);

int UnitMem(void);

#endif