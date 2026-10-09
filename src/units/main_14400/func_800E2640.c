#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x72]; u8 flags_72; } Object;
void func_800E2640(Object *p) { p->flags_72 &= 0xFD; }
