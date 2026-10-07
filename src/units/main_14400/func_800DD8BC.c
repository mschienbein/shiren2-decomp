#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0xC]; void *child; } Obj800DD8BC;
void func_800DAD20(void *obj, void *child);
void func_80121B04(void *child);
void func_800DAD80(void *obj);
s32 func_800DD8BC(Obj800DD8BC *obj) {
    void *child = obj->child;
    func_800DAD20(obj, child);
    func_80121B04(child);
    func_800DAD80(obj);
    return 0;
}
