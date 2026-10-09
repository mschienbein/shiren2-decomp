#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef struct { u8 pad[0x1E]; u8 flags; } Info;

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
        s32 modeMatches = (D_80142F18.mode & 0xE0) == 0x20;
        if (modeMatches) {
            result = 1;
        }
    }
    return result;
}
