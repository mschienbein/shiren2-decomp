#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad[0x14]; u16 *unk14; } Arg;
s32 func_800E4B60(void *, Arg *);
u8 func_800E8B10(void *, Item **);
void func_80113640(Item *, void *);
void func_800EA8FC(void *self, Arg *arg) {
    Item *buf[2];
    s32 i;
    func_800E4B60(self, arg);
    if ((*arg->unk14 >> 4) & 1) {
        i = func_800E8B10(self, buf);
        while (--i != -1) {
            func_80113640(buf[i], self);
        }
    }
}
