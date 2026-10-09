#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    s32 key;
    s32 value;
} Entry800447EC;

typedef struct {
    u8 pad0[0xC];
    s32 found;
} Obj800447EC;


extern Entry800447EC D_8014A928[4];
extern Entry800447EC *D_80138BB0;

void func_800447EC(Obj800447EC *obj) {
    u32 key = D_80142F18.mode;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (key == D_8014A928[i].key) {
            D_80138BB0 = &D_8014A928[i];
            obj->found = 1;
            return;
        }
    }
    obj->found = 0;
}
