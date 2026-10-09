#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[2];
    s16 kind;
    u8 pad4[8];
    s16 field_C;
    s16 field_E;
    u8 pad10[0x24];
    s8 field_34;
    u8 pad35[7];
    u16 field_3C;
    u8 field_3E;
    u8 field_3F;
    u8 pad40;
    u8 field_41;
    u8 field_42;
    u8 field_43;
    u8 pad44[2];
    u8 field_46;
    u8 field_47;
    u8 pad48[0x68];
} Unit;
typedef struct { s16 field_0; s16 field_2; s16 field_4; s16 field_6; s16 field_8; s16 field_A; } Slot;
extern Slot D_8013D904[];
extern Unit D_801D8FFC[];
extern Unit D_801DEAB4[];
extern u8 D_8014CC80[];
extern u8 D_8014CCA4[];
extern u8 D_8014CCC8[];
extern u8 D_8014CCEC[];
extern u8 D_8014CD10[];
extern u8 D_8014CD34[];
extern u8 D_8014CD58[];
extern u8 D_8014CD7C[];
extern u8 D_8014CDA0[];
extern u8 D_8014CDC4[];
extern u8 D_8014CDE8[];
extern u8 D_8014CE0C[];
extern u8 D_8014CF64[];
extern u8 D_8014CF6C[];
s32 func_80041C64(s32 who);
void func_80074778(Unit *unit);
void func_80079560(s32 a, s32 index, s32 flag);
void func_80074E70(Unit *unit, u8 *sequence, s32 elapsed);

/* Refreshes the two equipment display units (weapon at 2*i, shield at 2*i+1)
 * of both party members from their owners' facing and equipment state.
 * Each slot's unit is checked through its own pointer and then handed to the
 * shared `unit` pointer that the refresh code works on. */
void func_80077FE4(void) {
    s32 i;
    s32 j;
    s32 id;
    Unit *src;
    Unit *weapon;
    Unit *shield;
    Unit *unit;
    u8 *entry;
    u32 index;

    for (i = 0; i < 2; i++) {
        switch (i) {
        case 0:
            id = 0;
            break;
        case 1:
            id = func_80041C64(0x1A);
            break;
        default:
            continue;
        }
        if (id == -1) {
            for (j = 0; j < 2; j++) {
                func_80074778(&D_801D8FFC[j + 2 * i]);
                D_8013D904[i].field_0 = -1;
                D_8013D904[i].field_4 = -1;
                D_8013D904[i].field_6 = -1;
            }
            continue;
        }
        src = &D_801DEAB4[id];
        index = src->field_3F;
        if (D_8013D904[i].field_6 == -1 || index >= 0x20
            || (src->kind != 0x17 && src->kind != 0x1A)
            || (src->field_3C != 0xA9 && src->field_3C != 0x90)) {
            func_80079560(1, 2 * i, 1);
            func_80079560(1, 2 * i + 1, 1);
            continue;
        }
        if (D_8013D904[i].field_4 != -1) {
            shield = &D_801D8FFC[2 * i + 1];
            if (shield->kind != -1) {
                unit = shield;
                func_80079560(1, 2 * i + 1, 0);
                if (D_8013D904[i].field_4 == 0x6D) {
                    entry = &D_8014CD7C[index];
                    unit->field_3F = unit->field_3E = *entry & 0xF;
                    unit->field_34 = (*entry & 0x20) ? 1 : -1;
                    unit->field_C = (s8)(src->kind == 0x1A ? D_8014CD10[index] : D_8014CCC8[index]);
                    unit->field_E = (s8)(src->kind == 0x1A ? D_8014CD34[index] : D_8014CCEC[index]);
                } else {
                    entry = &D_8014CD58[index];
                    unit->field_3F = unit->field_3E = *entry & 0xF;
                    unit->field_34 = (*entry & 0x20) ? 2 : -1;
                    unit->field_C = (s8)(src->kind == 0x1A ? D_8014CDE8[index] : D_8014CDA0[index]);
                    unit->field_E = (s8)(src->kind == 0x1A ? D_8014CE0C[index] : D_8014CDC4[index]);
                }
                if (D_8013D904[i].field_A == 1) {
                    switch (D_8013D904[i].field_4) {
                    case 0x59:
                    case 0x5E:
                    case 0x61:
                        func_80074E70(unit, D_8014CF64, 1);
                        break;
                    default:
                        unit->field_41 = 0;
                        unit->field_43 = 0;
                        unit->field_42 = 0;
                        break;
                    }
                } else {
                    unit->field_41 = 0;
                    unit->field_43 = 0;
                    unit->field_42 = 0;
                }
                unit->field_46 = src->field_46;
                unit->field_47 = src->field_47;
            }
        }
        if (D_8013D904[i].field_0 != -1) {
            weapon = &D_801D8FFC[2 * i];
            if (weapon->kind != -1) {
                unit = weapon;
                func_80079560(1, 2 * i, 0);
                if ((u16)(D_8013D904[i].field_0 - 0x4C) < 2) {
                    entry = &D_8014CCA4[index];
                } else {
                    entry = &D_8014CC80[index];
                }
                unit->field_3F = unit->field_3E = *entry & 0xF;
                unit->field_34 = (*entry & 0x20) ? 1 : -1;
                unit->field_C = (s8)(src->kind == 0x1A ? D_8014CD10[index] : D_8014CCC8[index]);
                unit->field_E = (s8)(src->kind == 0x1A ? D_8014CD34[index] : D_8014CCEC[index]);
                if (D_8013D904[i].field_8 == 1) {
                    switch (D_8013D904[i].field_0) {
                    case 0x4A:
                        func_80074E70(unit, D_8014CF64, 1);
                        break;
                    case 0x3A:
                    case 0x53:
                    case 0x54:
                    case 0x55:
                        func_80074E70(unit, D_8014CF6C, 1);
                        break;
                    default:
                        unit->field_41 = 0;
                        unit->field_43 = 0;
                        unit->field_42 = 0;
                        break;
                    }
                } else {
                    unit->field_41 = 0;
                    unit->field_43 = 0;
                    unit->field_42 = 0;
                }
                unit->field_46 = src->field_46;
                unit->field_47 = src->field_47;
            }
        }
    }
}
