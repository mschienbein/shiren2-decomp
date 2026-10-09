#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;

typedef struct {
    u8 field_0;
    u8 pad1[0xB];
    u8 field_C;
} Obj80043550;



extern void func_80062804(u32 mode, s32 arg1, s32 arg2, s32 arg3);
extern void func_80067814(s32 enable);

void func_80043550(Obj80043550 *obj) {
    s32 mode;
    u8 enable;

    func_80062804(3, obj->field_0, 0, obj->field_C);
    mode = D_80142F18.flags & 3;
    enable = 1;
    if (mode != 1) {
        enable = mode != 2;
    }
    func_80067814(enable);
}
