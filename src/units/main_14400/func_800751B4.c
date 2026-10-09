#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_00[2]; u8 scale_02; } GraphicHeader;
typedef struct { GraphicHeader *header; s32 field_04; } GraphicEntry;
/* +4/+5 are separate bytes (ROM 0x800745A4/A8: sb); neither is used here. */
typedef struct {
    u16 field_00; short kind_02; u8 pad_04[2]; u8 level_06; u8 pad_07[3]; u16 flags_0A;
    u8 pad_0C[0x10]; float scale_1C, scale_20; u8 pad_24[0x22]; u8 reset_46;
    u8 pad_47[0x29]; u8 states_70[8]; u8 pad_78[0x38];
} DisplayUnit;
extern DisplayUnit D_801DEAB4[], D_801D2C2C[];
s32 func_80074114(void);
GraphicEntry *func_80074784(s32 kind, s32 level);
void func_80074778(DisplayUnit *unit);
void func_80079560(s32 first, s32 id, s32 mode);
/* The original call explicitly supplies the reset-mode argument. */
void func_80074F34(s32 id, s32 reset_mode);
/* Original success and failure exits return 0 and -1. */
s32 func_800751B4(s32 id, s32 flags)
{
    DisplayUnit *unit = &D_801DEAB4[id];
    GraphicEntry *entry;
    DisplayUnit *shadow;
    if (unit->kind_02 == -1) {
        return -1;
    }
    if (unit->kind_02 == 0xD6) {
        flags &= 0x8000;
    }
    if (unit->flags_0A == flags) {
        return 0;
    }
    func_80074114();
    entry = func_80074784(unit->kind_02, unit->level_06);
    func_80074784(0x17, D_801DEAB4[0].level_06);
    shadow = &D_801D2C2C[id];
    if (shadow->kind_02 != -1) {
        func_80074778(shadow);
    }
    unit->states_70[0] = 0xFF;
    unit->states_70[1] = 0xFF;
    unit->states_70[2] = 0xFF;
    unit->states_70[3] = 0;
    unit->states_70[4] = 0;
    unit->states_70[5] = 0;
    unit->states_70[6] = 0;
    unit->states_70[7] = 0;
    unit->reset_46 = 0xFF;
    unit->scale_1C = entry->header->scale_02 / 100.0f;
    unit->scale_20 = entry->header->scale_02 / 100.0f;
    func_80079560(0, id, 0);
    unit->flags_0A = flags;
    func_80074F34(id, 1);
    return 0;
}
