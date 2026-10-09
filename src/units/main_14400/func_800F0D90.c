#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct Object { u8 pad_0[0x1E]; u8 flags_1E; u8 pad_1F[0x5D]; u16 flags_7C; } Object;
typedef Object S;
typedef Object Obj_80049414;
typedef struct { Object *source_0; u8 pad_4[8]; u16 value_C, flags_E; } Pair;
extern u16 func_800E0ED0(S *s);
extern s16 func_800A0014(s16 x, u16 y);
extern s32 func_800E1CD4(S *, s32);
extern s32 func_800E1CC4(Obj_80049414 *obj, s32 kind);
short func_800F0D90(Object *self, Pair *event) {
    s16 value = event->value_C;
    s32 unchanged;
    if (!(event->flags_E & 8)) {
        u16 scale = func_800E0ED0(self);
        if (event->source_0 != 0 && ((event->source_0->flags_1E >> 4) & 1)) scale >>= 1;
        value = func_800A0014((s16)value, scale);
    }
    unchanged = func_800E1CD4(self, 15) == 1;
    if (!unchanged) {
        s32 invert = 0;
        if (((s16)value < 0 && ((self->flags_7C >> 2) & 1)) ||
            ((event->flags_E & 0x1000) && ((self->flags_7C >> 9) & 1) && !func_800E1CC4(self, 2))) invert = 1;
        if (invert) value = -value;
        else if ((event->flags_E & 0x800) && ((self->flags_7C >> 7) & 1)) return 0;
    }
    if ((s16)value == 0) value = 1;
    return (s16)value;
}
