#include "common.h"
typedef unsigned char u8;
extern void *func_800B51D4(void *object);
s32 func_800B5728(void *object){ u8 *p = func_800B51D4(object);
  if (p != 0) { u8 x = p[2]; if (x >= 0xFA) return 0xF6; if (x != 0) return 0xF5; }
  return 0; }
