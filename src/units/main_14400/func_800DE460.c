#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    u8 pad0[0x4];
    void *vtbl;
    u8 pad8[0xC4 - 0x8];
    void *value;
} Obj800DE460;

extern u8 D_80158988[];
extern Obj800DE460 *func_800DDAD0(void *obj, s32 kind);

static inline Obj800DE460 *construct_base(void *mem) {
    func_800DDAD0(mem, 0x2A);
    return mem;
}

unsigned char *func_800DE460(void *mem, void *value) {
    Obj800DE460 *obj = construct_base(mem);

    obj->vtbl = D_80158988;
    obj->value = value;
    return (unsigned char *)obj;
}
