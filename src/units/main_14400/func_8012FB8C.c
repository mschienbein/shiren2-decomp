#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { u32 w0; u32 w1; } Acmd;
typedef struct { u8 filter[0x14]; s16 *base; s16 *input; s32 length; } ALFx;
u32 func_800340F0(void *addr); /* osVirtualToPhysical */
#define FIXED_SAMPLE 184
#define A_LOADBUFF 6
Acmd *func_8012FB8C(ALFx *r, s16 *curr_ptr, s32 buff, Acmd *p) {
    Acmd *ptr = p;
    Acmd *cmd;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base) {
        curr_ptr += r->length;
    }
    updated_ptr = curr_ptr + FIXED_SAMPLE;

    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        cmd = ptr++;
        cmd->w0 = (A_LOADBUFF << 24) | (((before_end << 1) & 0xFFF) << 12) | (buff & 0xFFF);
        cmd->w1 = func_800340F0(curr_ptr);
        cmd = ptr++;
        cmd->w0 = (A_LOADBUFF << 24) | (((after_end << 1) & 0xFFF) << 12) |
                  ((buff + (before_end << 1)) & 0xFFF);
        cmd->w1 = func_800340F0(r->base);
    } else {
        cmd = ptr++;
        cmd->w0 = (A_LOADBUFF << 24) | (((FIXED_SAMPLE << 1) & 0xFFF) << 12) | (buff & 0xFFF);
        cmd->w1 = func_800340F0(curr_ptr);
    }
    return ptr;
}
