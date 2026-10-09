#include "common.h"
typedef unsigned char u8;
typedef struct Object Object;
typedef struct { Object *source; s32 kind, field_08; short amount; unsigned short flags; } Damage;
typedef struct { u32 kind; void *field_04; u8 reserved_08[8]; Damage *damage; } Event;
struct Object { u8 reserved_00[0xA]; u8 id_0A; u8 reserved_0B[0x14]; u8 id_1F; u8 reserved_20[0xE4]; Object *target; };
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern s32 func_800E8694(Object *), func_800E1CC4(Object *, s32), func_800E8350(Object *), func_800E74E0(Object *, Object *, s32), func_800ED5FC(Object *);
extern void func_800ED72C(Object *, Event *), func_800EDA7C(Object *), func_800EDAE8(Object *), func_800EDB04(Object *, Event *);
extern s32 func_800EDF50(Object *, Event *), func_800EA67C(Object *, Event *), func_80049CB4(s32, ...);
extern void func_800EDF78(Object *, s32), func_800EDFF0(Object *, s32);
extern s32 D_80148090;
s32 func_800ED448(Object *self, Event *event) {
    switch (event->kind) {
    case 0: {
        s32 wait = 0;
        if (self->target) {
            if (func_800E8694(self)) wait = func_800E1CC4(self, 1) == 0;
            if (wait) return func_800E8350(self);
            return func_800E74E0(self, self->target, 3);
        }
        return 0;
    }
    case 1: return func_800ED5FC(self);
    case 2: func_800ED72C(self, event); return 1;
    case 4: func_800EDA7C(self); return 1;
    case 8: case 9:
        if ((D_80142F18.mode ^ 0x4F) != 0) {
            Damage *damage = event->damage;
            Object *source = damage->source;
            s32 special = 0;
            D_80148090 = 0;
            if (source && source->id_0A == 0x2D) special = self->id_1F == self->id_0A;
            if (special) damage->flags |= 0x4000;
            func_800EA67C(self, event);
            func_80049CB4(0x89, self);
        }
        return 1;
    case 12: func_800EDAE8(self); return 1;
    case 10: func_800EDB04(self, event); return 1;
    case 21: return func_800EDF50(self, event);
    case 22: func_800EDF78(self, 1); return 1;
    case 24: func_800EDFF0(self, 1); return 1;
    default: return func_800EA67C(self, event);
    }
}
