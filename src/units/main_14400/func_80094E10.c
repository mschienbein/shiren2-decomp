#include "common.h"
typedef unsigned short u16;
typedef struct Obj80094DAC { char pad0[0x28]; s32 field28; } Obj80094DAC;
Obj80094DAC *func_800C9E10(void);
void func_800CB2D4(Obj80094DAC *obj, s32 flag);
void func_800CADBC(Obj80094DAC *self);
void func_80048240(u16 id, ...);
void func_80094E10(void *unused_receiver) { s32 flag; Obj80094DAC *obj = func_800C9E10(); flag = obj->field28 ^ 1; func_800CB2D4(obj, flag); func_800CADBC(obj); if (flag) func_80048240(0x23B); else func_80048240(0x23C); }
