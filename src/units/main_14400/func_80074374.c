#include "common.h"
typedef struct { unsigned char field_0[0x50]; void *field_50, *field_54, *field_58; unsigned char field_5C[0x54]; } Entry;
extern Entry D_801DEAB4[], D_801D8FFC[], D_801DD378[];
extern unsigned char D_8014C9F0[], D_8014CA04[], D_8014CA20[], D_8013D924[], D_8013D948[];
extern s32 D_8013D8CC;
extern void func_80074174(s32);
extern void *func_80074040(void *, s32);
extern s32 func_800718CC(u32, void *, void *);
void func_80074374(void)
{
    s32 i;
    func_80074174(1);
    D_8013D8CC = 1;
    for (i = 0; i < 30; i++) {
        Entry *entry = &D_801DEAB4[i];
        entry->field_50 = func_80074040(D_8014C9F0, 0x10);
        entry->field_58 = func_80074040(D_8014CA04, 0x480);
        entry->field_54 = func_80074040(D_8014CA20, 0x24);
    }
    for (i = 0; i < 4; i++) {
        Entry *entry = &D_801D8FFC[i];
        entry->field_50 = func_80074040(D_8014C9F0, 0x10);
        entry->field_58 = func_80074040(D_8014CA04, 0xCC);
        entry->field_54 = func_80074040(D_8014CA20, 0x10);
    }
    for (i = 0; i < 32; i++) {
        Entry *entry = &D_801DD378[i];
        entry->field_50 = func_80074040(D_8014C9F0, 0x10);
        entry->field_58 = func_80074040(D_8014CA04, 0x480);
        entry->field_54 = func_80074040(D_8014CA20, 0x10);
    }
    if (func_800718CC(3, D_8013D924, D_8013D948)) D_8013D8CC = 0;
}
