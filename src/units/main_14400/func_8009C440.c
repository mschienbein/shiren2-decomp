#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Arg_8009C440;

typedef struct {
    u8 pad0[0x50];
    void *unk50; /* Base of the caller's array of 0x20-byte records. */
    s32 unk54;
} Object_8009C440;

extern s32 D_80138E88;
u8 *func_8006A810(void *dst, s32 value, s32 size);
void func_8009543C(Object_8009C440 *obj, Arg_8009C440 *arg);

void func_8009C440(Object_8009C440 *obj, void *arg1, s32 arg2, s32 *arg3) {
    Arg_8009C440 copy;
    Arg_8009C440 arg;

    func_8006A810(&arg, 0, sizeof(arg));
    arg.unk0 = arg2 * 4;
    arg.unk4 = D_80138E88;
    arg.unk8 = arg3[0];
    arg.unkC = arg3[1];
    copy = arg;
    func_8009543C(obj, &copy);
    obj->unk50 = arg1;
    obj->unk54 = arg2;
}
