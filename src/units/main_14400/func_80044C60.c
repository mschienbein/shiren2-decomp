#include "common.h"

typedef struct {
    unsigned int pad : 8;
    unsigned int flag : 1;
    unsigned int rest : 23;
} Flags;
typedef struct { char pad0[0x20]; Flags flags; } Obj;
typedef struct { Flags flags; s32 extra; } State;
typedef struct { s32 field_0; } Target;

extern Target D_80160AF0;
extern char D_00194FC0[];
extern char D_2000000[];
extern char D_2000318[];
void func_8006AC30(Target *target, void *src, void *dst, s32 a3, s32 frame, s32 count);

Target *func_80044C60(Obj *obj, unsigned char frame) {
    State state;
    s32 enabled;
    s32 saved;

    func_8006AC30(&D_80160AF0, D_00194FC0, D_2000000, 8, frame - 1, 1);
    enabled = obj->flags.flag;
    state.flags = obj->flags;
    if (enabled) {
        saved = D_80160AF0.field_0;
        func_8006AC30(&D_80160AF0, D_00194FC0, D_2000318, 8, frame - 1, 1);
        D_80160AF0.field_0 = saved;
    }
    return &D_80160AF0;
}
