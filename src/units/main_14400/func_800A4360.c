#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u32 pad_bits : 7;
    u32 locked : 1;
    u8 rest[3];
} Flags800A4360;

typedef struct {
    u8 pad0[0x1C];
    u16 field_1C;
    u8 field_1E;
    u8 pad1F;
    Flags800A4360 field_20;
} Obj800A4360;

s32 func_800E1CD4(Obj800A4360 *obj, s32 kind);
u32 func_800B1C6C(void *arg0);

static inline s32 Flags_locked(Flags800A4360 *flags) { return flags->locked; }
static inline s32 Obj_locked(Obj800A4360 *obj) {
    Flags800A4360 flags = obj->field_20;
    return Flags_locked(&flags);
}

static inline u16 Obj_status(Obj800A4360 *obj) { return obj->field_1C; }

s32 func_800A4360(Obj800A4360 *obj, void *arg1) {
    s32 found = 0;

    if (((obj->field_1E & 0x7C) && func_800E1CD4(obj, 0xF) != 0)
        || (!(Obj_status(obj) & 0x20) && !(Obj_status(obj) & 0x10) && !Obj_locked(obj))) {
        found = 1;
    }
    if (found && (func_800B1C6C(arg1) & 0x2000)) {
        return 1;
    }
    return 2;
}
