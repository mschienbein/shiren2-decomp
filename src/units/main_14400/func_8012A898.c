#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 kind;
    u8 pad1[0x3];
    s32 value;
} Msg_8012A898;

extern s32 D_801CA704;

s32 func_8012AC2C(void *item);

s32 func_8012A898(s32 value) {
    Msg_8012A898 msg;

    msg.value = value;
    D_801CA704 = value;
    msg.kind = 2;
    return func_8012AC2C(&msg);
}
