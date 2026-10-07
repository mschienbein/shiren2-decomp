#include "common.h"
typedef unsigned char u8;
void func_8010BC98(u8 *p, u8 v) { if (v > 16) v = 16; p[0xE] = v; }
