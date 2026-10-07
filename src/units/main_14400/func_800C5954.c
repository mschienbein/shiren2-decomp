#include "common.h"

typedef unsigned short u16;
u16 func_800C58DC(void *ctx, u16 len);
s32 func_800C5954(void *ctx, u16 start, u16 end) {
    return start + func_800C58DC(ctx, end - start);
}
