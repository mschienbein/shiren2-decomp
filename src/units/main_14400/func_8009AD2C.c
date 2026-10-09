#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { u8 pad[0x4C]; void *x4C; u8 pad50[0xC]; const void *x5C; u8 pad60[0xC]; s32 x6C; } S;
extern S *func_800953C0(S *);
extern u8 D_80152620[];
extern const unsigned char D_80151DF8[24];
S *func_8009AD2C(S *p){ S *self = p; func_800953C0(p); self->x4C = D_80152620; self->x5C = D_80151DF8; self->x6C = -1; return self;}
