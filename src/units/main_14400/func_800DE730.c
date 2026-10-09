#include "common.h"

typedef struct {
    s32 unk00;
    const void *unk04;
    unsigned char unk08[0xBC];
    void *unkC4;
    s32 unkC8;
} Object;
extern const unsigned char D_801589B8[48];
extern Object *func_800DDAD0(Object *, s32);

Object *func_800DE730(Object *object, void *arg1) {
    func_800DDAD0(object, 0x2B);
    object->unk04 = D_801589B8;
    object->unkC4 = arg1;
    object->unkC8 = 0;
    return object;
}
