#include "common.h"
typedef unsigned char u8;
/* Pool header view: record storage pointer at +0, occupancy bitset pointer at +4. */
typedef struct { void *field0; u8 *field4; } Obj;
extern const u8 D_80154894[8];
void func_800AFCA8(Obj *p, u8 index) { s32 bit = index; p->field4[bit >> 3] &= D_80154894[bit & 7]; }
