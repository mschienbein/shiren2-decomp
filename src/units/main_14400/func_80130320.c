#include "common.h"
typedef struct { unsigned char pad0[0x88]; s32 field_88; } S;
typedef struct { unsigned char pad0[8]; S *field_8; } Slot8012B36C;
/* Pooled event (func_80130780 pops it from the free list): +0 is the link. */
typedef struct Msg { struct Msg *next; s32 field_4; short field_8; short field_A; s32 field_C; s32 field_10; } Msg;
typedef struct { unsigned char pad0[0x1C]; s32 field_1C; } Glob;
extern Glob *D_80148D84;
extern Msg *func_80130780(void);
extern s32 func_8012E5F8(S *p, s32 msg, Msg *arg);
void func_80130320(Slot8012B36C *slot) {
    if (slot->field_8) {
        Msg *msg = func_80130780();
        if (msg) {
            s32 time = D_80148D84->field_1C + slot->field_8->field_88;
            msg->field_8 = 15;
            msg->next = 0;
            msg->field_4 = time;
            func_8012E5F8(slot->field_8, 3, msg);
        }
    }
}
