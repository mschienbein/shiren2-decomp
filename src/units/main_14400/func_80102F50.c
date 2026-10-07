#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad[0x1E]; u8 flags; } Info;
extern u8 D_80142F20;
Info *func_800B4928(void *);
u8 *func_800B4D80(void *);
s32 func_800E0F40(void *);
s32 func_80102F50(void *self, void *target) {
    Info *info;
    u8 *kind;
    s32 result;
    info = func_800B4928(target);
    kind = func_800B4D80(target);
    result = 0;
    if (info != 0 && ((info->flags >> 1) & 1)) {
        result = 1;
    } else if (kind != 0 && *kind == 15 && (u8)func_800E0F40(self) == 3) {
        s32 modeMatches = (D_80142F20 & 0xE0) == 0x20;
        if (modeMatches) {
            result = 1;
        }
    }
    return result;
}
