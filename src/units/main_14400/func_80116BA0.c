#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0xC]; u8 flags_C; } Object;
void func_80116BA0(Object *p) { p->flags_C &= 0xEF; }
