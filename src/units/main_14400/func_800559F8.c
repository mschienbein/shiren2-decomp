#include "common.h"
typedef struct {
    char fields_0[0x44];
    s32 field_44;
    char fields_48[8];
    short field_50;
    char fields_52[8];
    short field_5A, field_5C, field_5E;
} Entry;
extern s32 D_80139B18;
extern Entry D_801D40DC[];
void func_800559F8(s32 value) {
    Entry *entry;
    Entry *end;
    if (D_80139B18) {
        entry = D_801D40DC;
        end = entry + 32;
        for (; entry < end; entry++) {
            if (entry->field_44 != -1 && entry->field_5C == 1 && entry->field_50 != value) {
                entry->field_50 = value;
                entry->field_5A = 0;
            }
        }
    }
}
