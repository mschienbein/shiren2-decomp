#include "common.h"

s32 func_800AC584(unsigned short value) { s32 result=value; if (result & 0x8000) result=(result & 0x7FFF)*1000; return result; }
