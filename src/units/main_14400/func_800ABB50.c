#include "common.h"

typedef unsigned char u8;

typedef struct { void *unk0; u8 pad4[0x14]; } Entry;
extern Entry D_80142B1C[];
extern s32 D_00194FC0;
void func_8006AC30(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5);
void func_800ABB50(void *arg0, u8 index, u8 arg2) {
    func_8006AC30(arg0, &D_00194FC0, D_80142B1C[index].unk0, 0xC, arg2, 1);
}
