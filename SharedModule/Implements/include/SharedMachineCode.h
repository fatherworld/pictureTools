#ifndef _SHARED_MACHINE_CODE_H_
#define _SHARED_MACHINE_CODE_H_

#ifdef AlvaPublic
#include "SharedModule_Hide.h"
#endif

#ifdef __cplusplus
extern "C" 
{
#endif
    
    int shareMachineCode(unsigned char* code);

#ifdef __cplusplus
}
#endif

#endif