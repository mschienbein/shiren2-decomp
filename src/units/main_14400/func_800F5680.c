#include "common.h"

typedef struct {
    char pad0[0xA];
    unsigned char kind_A;
    char padB[0x1F - 0xB];
    unsigned char kind_1F;
    char pad20[4];
    s32 *vtbl_24;
    char pad28[4];
} Obj_800F5680;

extern s32 D_801597E0[];
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Obj_800F5680 *obj);

Obj_800F5680 *func_800F5680(void) {
    Obj_800F5680 *obj = func_800A38FC(sizeof(Obj_800F5680));

    func_800F4760(obj);
    obj->vtbl_24 = D_801597E0;
    obj->kind_A = 0xF;
    obj->kind_1F = 0xF;
    return obj;
}
