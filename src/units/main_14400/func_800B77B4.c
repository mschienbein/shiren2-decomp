#include "common.h"

typedef struct {
    char pad0[0x28];
    short delta_28;
    short index_2A;
    void (*func_2C)(char *self, s32 arg1, void *arg2);
} VTable800B77B4;

typedef struct {
    char pad0[0x18];
    VTable800B77B4 *vtable_18;
} Obj800B77B4;

extern s32 D_80153B84;
extern s32 D_80147490;
void func_800CA4E8(Obj800B77B4 *obj, void *arg1);

void func_800B77B4(Obj800B77B4 *obj) {
    func_800CA4E8(obj, &D_80153B84);
    obj->vtable_18->func_2C((char *)obj + obj->vtable_18->delta_28, 1, &D_80147490);
}
