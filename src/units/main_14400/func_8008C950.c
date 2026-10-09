#include "common.h"
typedef struct EntryB {
    unsigned char field_00, field_01, field_02, field_03, field_04, field_05;
    unsigned char pad_06[2];
    void *field_08;
    float scale_0C;
    unsigned char pad_10[0x40];
    void *entries_50[9];
    void *field_74;
} EntryB;
void func_8008C950(EntryB *entry) {
    s32 i;
    entry->field_00 = 0;
    entry->field_01 = 0;
    entry->field_02 = 0;
    entry->field_03 = 0;
    entry->field_04 = 0;
    entry->field_05 = 255;
    entry->field_08 = 0;
    entry->field_74 = 0;
    entry->scale_0C = 1.0f;
    for (i = 8; i >= 0; --i) entry->entries_50[i] = 0;
}
