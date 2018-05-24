#ifndef _emath_fl_h_
#define _emath_fl_h_

#include "Config.h"

/*
 * clamp
 * ²Ã¼ôÊý¾Ý
 * val < minV ? minv : (val > maxV) ? maxV : val
 */
#define clamp(val, minV, maxV) (((val) < (minV)) ? (minV) : ((val) > (maxV) ? (maxV) : (val)))

#endif