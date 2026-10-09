#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    u8 pad0[0x24];
    void *vtbl;
} Obj80106400;

extern u8 D_8015BFB8[];
extern Obj80106400 *func_800EFC70(Obj80106400 *obj, s32 arg1, u8 arg2);
extern void func_80106444(Obj80106400 *obj);

Obj80106400 *func_80106400(Obj80106400 *obj, u8 arg1) {
    func_800EFC70(obj, 0x4A, arg1);
    obj->vtbl = D_8015BFB8;
    func_80106444(obj);
    return obj;
}
