#include "common.h"

typedef unsigned char u8;

typedef struct {
    u32 flags_0;
    u8 pad4[0x44 - 4];
    s32 id_44;
} Ctx80129BB0;

/* Optional notification hook (cleared by func_80129C2C, set by func_8012AAB0). */
extern void (*D_801CA710)(s32 id, s32 value);

/* Script opcode: one value byte, one flag byte (bit 7 adds a trailing byte). */
u8 *func_80129BB0(Ctx80129BB0 *ctx, u8 *cursor) {
    u8 value = *cursor++;

    if (*cursor++ & 0x80) {
        cursor++;
    }
    if ((ctx->flags_0 & 3) == 2 && D_801CA710 != 0) {
        D_801CA710(ctx->id_44, value);
    }
    return cursor;
}
