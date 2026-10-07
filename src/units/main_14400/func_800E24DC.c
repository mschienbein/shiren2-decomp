#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x73];
    u8 unk73;
} Obj800E24DC;

void func_800E24DC(Obj800E24DC *obj) {
    obj->unk73 = 3;
}
