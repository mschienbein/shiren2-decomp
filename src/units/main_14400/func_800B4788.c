#include "common.h"
typedef unsigned char u8;
typedef struct { u8 value; } Dir;
typedef struct { char pad0[8]; Dir unk8; char pad9[7]; } Work;
s32 func_800B43BC(void *, s32, u8);
s32 func_80049CB4(s32 id, ...);
void *func_800A2594(void *out, void *from, Dir dir);
void func_800B4788(void *obj) {
    Work work;
    s32 i;
    func_800B43BC(obj, 1, 0);
    i = 0;
    while (1) {
        if (i >= 8) {
            break;
        }
        func_80049CB4(6);
        work.unk8.value = i & 7;
        func_800A2594(&work, obj, work.unk8);
        func_800B43BC(&work, 1, i);
        func_80049CB4(7);
        i++;
    }
}
