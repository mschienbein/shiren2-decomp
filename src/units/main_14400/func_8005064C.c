#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { s32 x0, x4; } V2;
typedef struct { u8 pad[0x24]; s32 x24, x28; u8 pad2[0x30]; s32 x5C, x60, x64, x68, x6C, x70; } Task;
extern s32 D_8013968C; extern V2 *D_801476B8; extern void func_8008B5B0(Task *); extern Task *func_80085154(void (*)(Task *), s32); extern V2 *func_800C5F60(void);
void func_8005064C(s32 *a){ Task *t; if (D_8013968C == 0xDC) { t = func_80085154(func_8008B5B0, 0); t->x5C = a[1]; t->x68 = a[0]; t->x60 = a[3]; t->x6C = a[2]; t->x64 = D_801476B8->x4; t->x70 = D_801476B8->x0; t->x24 = func_800C5F60()->x4; t->x28 = func_800C5F60()->x0; } }
