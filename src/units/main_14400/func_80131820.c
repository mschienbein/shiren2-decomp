#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
/* Compatible 10-byte slot view: +4 timer, +7 state, +8/+9 per-slot fields.
 * Four slots occupy [0x801D4CDC, 0x801D4D04); the opaque fields are not arrays
 * used for access to another slot. */
typedef struct { u8 pad0[4]; u16 timer; u8 pad6; u8 state; u8 field_8; u8 field_9; } Slot80131820;
typedef struct { u8 pad[0x68]; } Data80131820;
typedef void *ProbeMessage;
struct ProbeThread;
typedef struct {
    struct ProbeThread *receive_waiters;
    struct ProbeThread *send_waiters;
    s32 valid_count;
    s32 first;
    s32 capacity;
    ProbeMessage *messages;
} ProbeMessageQueue;
extern Slot80131820 D_801D4CDC[4];
extern Data80131820 D_801E00EC[];
extern ProbeMessageQueue D_801E028C;
extern u32 D_80148E40;
u32 func_8006C550(void);
s32 func_80131630(Slot80131820 *slot, u32 index);
s32 func_8002C964(ProbeMessageQueue *queue, Data80131820 *data, s32 index);
/* The D_80148E44 request-handler contract passes an unused message pointer. */
s32 func_80131820(void *message) {
    u32 i;
    func_8006C550();
    for (i = 0; i < 4; i++) {
        Slot80131820 *slot = &D_801D4CDC[i];
        switch (slot->state) {
        case 0:
            break;
        case 1:
            if (func_80131630(slot, i) != 0) slot->state = 0;
            break;
        case 2:
            if (slot->field_8 == 0) {
                if (slot->timer % D_80148E40 == 0 && func_8002C964(&D_801E028C, &D_801E00EC[i], i) == 0) {
                    slot->field_8 = 1;
                    slot->field_9 = 2;
                }
                slot->timer++;
            } else if (func_80131630(slot, i) != 0) {
                slot->timer = i;
                slot->field_8 = 0;
                slot->field_9 = 0;
            }
            break;
        case 0x81:
        case 0x82:
            if (slot->field_9 == 2) func_80131630(slot, i);
            break;
        }
    }
    return 0;
}
