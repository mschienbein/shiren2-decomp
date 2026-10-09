#include "common.h"
typedef unsigned char u8;
typedef struct { u8 reserved_00[9]; u8 kind_09, id_0A; u8 reserved_0B[0x13]; u8 flags_1E; } Object;
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern s32 func_800A44F4(void *, void *);
extern s32 func_800A58B8(void *);
extern s32 func_800B58E4(s32, s32, s32, s32);
s32 func_800EC6A4(Object *self, Object *target) {
    s32 relation;
    u8 flags;
    s32 kind;
    if ((D_80142F18.flags >> 2) & 1) return 1;
    kind = target->kind_09 & 15;
    if ((func_800B58E4(2, 2, kind, func_800A58B8(target)) ^ 1) != 0) return 0;
    relation = func_800A44F4(self, target);
    flags = target->flags_1E;
    if (flags & 1) return 1;
    if ((flags >> 1) & 1) return 0;
    if ((flags >> 3) & 1) return relation != 2;
    if ((flags >> 4) & 1) return relation == 1;
    if ((flags >> 5) & 1) {
        switch (target->id_0A) {
        case 0x57: case 0x5A: return relation != 2;
        case 0x58: case 0x59: case 0x5B: case 0x5D: return 0;
        case 0x5C: case 0x5E: return 1;
        default: return 0;
        }
    }
    return 1;
}
