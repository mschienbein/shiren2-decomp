#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 type;
    u8 pad4[0xC];
    s32 *arg;
} Event;
s32 func_800F27A4(u8 *, Event *);
s32 func_800F069C(u8 *);
s32 func_800F0EC4(u8 *);
u16 func_800E08B0(u8 *);
s32 func_800E0F40(u8 *);
s32 func_80049CB4(s32, ...);
s32 func_800A529C(void *, s32);
s32 func_800FFE48(u8 *self, Event *event) {
    s32 trigger;

    if (event->type == 9) {
        trigger = 0;
        func_800F27A4(self, event);
        if (!func_800F069C(self) && !func_800F0EC4(self) && func_800E08B0(self) != 0 && *event->arg != 0) {
            trigger = (u8)func_800E0F40(self) >= 2;
        }
        if (trigger) {
            func_80049CB4(0x132);
            func_800A529C(self, 0);
        }
        return 1;
    }
    return func_800F27A4(self, event);
}
