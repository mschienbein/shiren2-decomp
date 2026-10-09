#include "common.h"
typedef unsigned char u8;
typedef struct { u8 reserved_00[0xA]; u8 id_0A; u8 reserved_0B[0x13]; u8 flags_1E; u8 reserved_1F[0x69]; s32 field_88; u8 reserved_8C[8]; void *field_94; u8 reserved_98[2]; unsigned short flags_9A; u8 reserved_9C[8]; void *field_A4; } Object;
extern s32 func_800A6FD0(Object *);
extern s32 func_800E4454(Object *);
/* self is supplied by the virtual caller; only the target and output are used. */
s32 func_800ECD20(void *self, Object *target, u8 *amount) {
    u8 flags;
    *amount = 10;
    if (!target) return 0;
    if (func_800A6FD0(target)) return 1;
    flags = target->flags_1E;
    if ((flags >> 3) & 1) {
        if (func_800E4454(target)) return 2;
        return target->field_88 != 0;
    }
    if ((flags >> 4) & 1) {
        s32 result = 2;
        if (target->flags_9A & 0x40) result = 1;
        return result;
    }
    if ((flags >> 1) & 1) { *amount = 5; return 2; }
    if ((flags >> 6) & 1) return 1;
    switch (target->id_0A) {
    case 0x58: case 0x59: case 0x5D: return 2;
    case 0x5A: if (target->field_A4) return 2; break;
    case 0x57: if (target->field_94) return 2; break;
    case 0x5B: case 0x5C: return 1;
    }
    return 0;
}
