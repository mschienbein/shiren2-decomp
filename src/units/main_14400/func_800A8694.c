#include "common.h"

typedef unsigned char u8;

/* Factory slots (func_800F8E10..func_80108B90): variant byte plus nullable placement memory. */
typedef void *(*Fn)(u8 variant, void *mem);
extern Fn D_8015CC64[];
s32 func_800A3934(void *obj);
void *func_800A8694(u8 a, u8 b, void *mem) { void *r = D_8015CC64[a - 0x1D](b, mem); if (func_800A3934(r)) r = 0; return r; }
