#include "common.h"

typedef struct Self Self;
typedef struct { Self *field_00; s32 field_04; } Event;
typedef struct { s32 field_00; unsigned char pad_04[12]; Event *field_10; } Msg_800F96B4;
typedef Self Obj_800F96B4;
extern unsigned char D_801531A0[];
extern unsigned short D_8014767C;
extern s32 func_801F258C(s32 id, s32 kind);
extern void func_800F16C0(Self *self, s32 flag);
/* 800E42C8 dereferences the second argument as an event pointer. */
extern void func_800E42AC(Self *self, Event *event);
extern s32 func_800F27A4(Obj_800F96B4 *obj, Msg_800F96B4 *msg);
s32 func_80108710(Self *self, Msg_800F96B4 *msg)
{
    switch (msg->field_00) {
    case 0xA:
        if ((func_801F258C(0x55, 0) ^ 1) == 0) {
            Event *event = msg->field_10;
            if (!(D_801531A0[event->field_04] & 0x20)) {
                func_800F16C0(self, event->field_04 == 0x27);
            }
            func_800E42AC(self, event);
            D_8014767C |= 0x40;
            return 1;
        }
        break;
    case 0x15:
        return 0;
    }
    return func_800F27A4(self, msg);
}
