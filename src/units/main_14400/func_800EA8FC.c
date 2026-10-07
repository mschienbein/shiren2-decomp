#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad[0x14]; u16 *unk14; } Arg;
s32 func_800E4B60(void *, Arg *);
u8 func_800E8B10(void *, u8 **);
void func_80113640(u8 *, void *);
void func_800EA8FC(void *self, Arg *arg) {
    u8 *buf[2];
    s32 i;
    func_800E4B60(self, arg);
    if ((*arg->unk14 >> 4) & 1) {
        i = func_800E8B10(self, buf);
        while (--i != -1) {
            func_80113640(buf[i], self);
        }
    }
}
