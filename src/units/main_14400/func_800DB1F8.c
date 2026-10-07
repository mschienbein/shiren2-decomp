#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 f_0; void *vt; } S;
extern u8 D_80158448[];
void *func_800DA8A0(void *obj, s32 kind, void *src);
S *func_800DB1F8(S *s, void *src) { func_800DA8A0(s, 0xF, src); s->vt = D_80158448; return s; }
