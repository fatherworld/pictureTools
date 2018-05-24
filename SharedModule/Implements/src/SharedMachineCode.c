#include "Config.h"
#include "Format.h"

#define LOG_TAG "SharedMachineCode"
#include "Log.h"
#include "ErrorTools.h"

#include "memory_fl.h"
#include "string_fl.h"

#include "SharedMachineCode.h"
#include "SharedMachineCodeInterface.h"

#undef FILE_NUM
#define FILE_NUM 0xA900

static unsigned char machineCode[41] = { 0 };

EXPORT_API int cGetMachineCode(unsigned char* code) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

    memcpy_fl(code, 41, machineCode, 41);

    return 0;
}

int shareMachineCode(unsigned char* code) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    memcpy_fl(machineCode, 40, code, 40);

    return 0;
}