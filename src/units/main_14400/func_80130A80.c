#include "common.h"

typedef unsigned char u8;
typedef struct {
    void *receive_waiters; void *send_waiters;
    long valid_count, first, capacity;
    void **messages;
} ProbeMessageQueue;
typedef struct Descriptor Descriptor;
typedef struct { unsigned short type; u8 status; u8 error; } ControllerStatus;
extern ProbeMessageQueue D_801D256C, D_801E4E80;
extern void *D_801CA960, *D_801CA964;
extern Descriptor D_80148DA8;
extern s32 D_801D8FF8;
extern ControllerStatus D_801D2C10[4];
/* Returns the restored interrupt mask; this caller discards it. */
extern u32 func_80132020(void);
extern void func_80027EA0(ProbeMessageQueue *queue, void **messages, long capacity);
extern void func_80131BA0(Descriptor *descriptor);
extern void func_801315D0(void);

u8 func_80130A80(void)
{
    s32 bit;
    u8 mask;
    s32 index;
    func_80132020();
    func_80027EA0(&D_801D256C, &D_801CA960, 1);
    func_80027EA0(&D_801E4E80, &D_801CA964, 1);
    func_80131BA0(&D_80148DA8);
    func_801315D0();
    bit = 1;
    mask = 0;
    index = 0;
    D_801D8FF8 = 0;
    for (; index < 4; ++index) {
        if (D_801D2C10[index].error == 0) {
            if ((D_801D2C10[index].type & 0x1F07) == 5) {
                mask |= bit;
                ++D_801D8FF8;
            }
            bit <<= 1;
        }
    }
    return mask;
}
