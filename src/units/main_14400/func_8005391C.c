#include "common.h"

/* 0x420-byte text-window slot (same layout as func_80053B74's Slot). */
typedef struct {
    s32 window_0;
    u32 state_4;
    s32 mode_8;
    s32 timer_C;
    s32 field_10, field_14, field_18, field_1C;
    char messages_20[4][0x100];
} Slot;

extern Slot D_80161724[1];
s32 func_80083718(void);
void func_80053AE8(s32 index);
s32 func_80041FF8(void);
s32 func_8006D550(s32 mode);
void func_800836D4(s32 v);
s32 func_80054B48(void);
void func_800835AC(u32 index);

/* Per-frame update of the message-window slots, walked from the last slot down. */
void func_8005391C(void) {
    Slot *slots = D_80161724;
    Slot *slot = slots + 1;
    s32 index = 0;

    for (; index != -1; --index) {
        --slot;
        if (slot->window_0 != -1) {
            u32 state = slot->state_4;
            switch (state) {
            case 0:
                if (func_80083718() == 0) {
                    if (slot->timer_C > 0) {
                        --slot->timer_C;
                    } else if ((u32)(slot->mode_8 - 1) < 2U) {
                        func_80053AE8(index);
                    }
                }
                if (!(func_80041FF8() & 0xFF) && slot->mode_8 == 2 && func_8006D550(6) == 1) {
                    func_80053AE8(index);
                }
                break;
            case 1: {
                s32 input = func_8006D550(6);
                func_800836D4(1);
                if (input == state) {
                    slot->state_4 = 2;
                    func_80054B48();
                    func_800836D4(0);
                }
                break;
            }
            case 2:
            case 3:
                func_800835AC(slot->window_0);
                break;
            }
        }
    }
}
