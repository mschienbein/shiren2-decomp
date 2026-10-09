#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_00[0xA]; u8 kind_0A; u8 pad_0B[0x73]; u16 field_7E; u8 pad_80[0x1A]; u16 field_9A; } Object;
typedef struct { u8 pad_00[0x1E]; u8 flags_1E; } Target;
extern u8 func_800A9958(void);
extern u8 func_801E9F70(s32 flag);
s32 func_800F2684(Object *self, Target *target) {
    s32 result;
    if (target) {
        if (self->field_9A & 0x40) {
            if (target->flags_1E & 0xC) return 0;
            if (((target->flags_1E >> 4) & 1) && self->kind_0A != 0x3E) {
                if (func_800A9958() != 7) return 0;
                if (!func_801E9F70(0x51)) return 0;
            }
        }
        result = self->field_7E;
    } else result = 0;
    return result;
}
