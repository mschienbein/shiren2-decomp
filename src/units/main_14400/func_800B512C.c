#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 field_0;
    u8 field_1;
    u8 field_2;
    u8 field_3;
} Entry;

extern u8 D_80143393;
extern Entry D_80143394[];

extern s32 func_80049CB4(s32 id, ...);

void func_800B512C(s32 notify) {
    Entry *entry = D_80143394;
    s32 i = 0;

    while (1) {
        s32 args[2];

        if (i >= 40) {
            break;
        }
        if (notify != 0 && entry->field_2 != 0) {
            args[1] = entry->field_0;
            args[0] = entry->field_1;
            func_80049CB4(0xE1, args);
        }
        entry->field_3 = 0;
        entry->field_2 = 0;
        entry->field_1 = 0;
        entry->field_0 = 0;
        entry++;
        i++;
    }
    D_80143393 = 0;
}
