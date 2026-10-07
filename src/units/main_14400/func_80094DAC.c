#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char pad0[0x24]; s32 field_24; } Obj80094DAC;
Obj80094DAC *func_800C9E10(void);
void func_800CB288(Obj80094DAC *obj, s32 flag);
void func_800CADBC(Obj80094DAC *obj);
void func_800925E0(s32 flag);
void func_80048240(u16 id, ...);
void func_80094DAC(void) {
    Obj80094DAC *obj = func_800C9E10();
    s32 flag = obj->field_24 ^ 1;
    func_800CB288(obj, flag);
    func_800CADBC(obj);
    func_800925E0(flag);
    func_80048240(flag ? 0x239 : 0x23A);
}
