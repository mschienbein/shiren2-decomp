#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 field_0, field_1; u8 pad_2[2]; u8 field_4; u8 pad_5[7]; s32 field_C; } Obj;

typedef struct Rng Rng;
extern s32 D_80142F34;
/* Two threshold bytes in each four-byte-aligned rodata entry. */
extern const u8 D_8015371C[2], D_80153720[2], D_80153724[2];
extern u8 D_801C5224[3];

extern const u8 D_8015705C[5][2][3], D_8015709C[5][2][3], D_8015703C[5][2][3], D_8015701C[5][2][3], D_8015707C[5][2][3];
extern Rng D_80147620;
extern u16 func_800C58DC(void *rng, u16 range);
void func_800AB750(Obj *obj, s32 mode) {
    u8 tier, high, stage, value, flags;
    const u8 *weights, *cursor;
    s32 i;
    u16 sum;
    if (!D_80142F34) {
        u8 first = D_8015371C[0];
        u8 second = D_80153720[0];
        u8 third = D_80153724[0];
        D_80142F34 = 1;
        D_801C5224[0] = first;
        D_801C5224[1] = second;
        D_801C5224[2] = third;
    }
    stage = D_80142F24.index;
    if ((u8)(stage - 1) >= 3) { obj->field_C = 1; obj->field_4 = 1; return; }
    high = D_80142F24.count >= D_801C5224[stage - 1];
    if (mode == 0) tier = stage - 1;
    else if (mode == 2) tier = 3;
    else tier = 4;
    switch (obj->field_1) {
    case 0xE9: weights = D_8015705C[tier][high]; break;
    case 0xEA: weights = D_8015709C[tier][high]; break;
    case 0xEB: weights = D_8015703C[tier][high]; break;
    case 0xEC: weights = D_8015701C[tier][high]; break;
    default: weights = D_8015707C[tier][high]; break;
    }
    cursor = weights;
    value = 1;
    sum = 0;
    for (i = 2; i >= 0; i--) sum += *cursor++;
    if (sum != 0) {
        u16 roll = func_800C58DC(&D_80147620, sum - 1);
        s32 choice;
        for (choice = 1; choice < 4; choice++, weights++) {
            if (roll < *weights) { value = choice; break; }
            roll -= *weights;
        }
    }
    obj->field_C = value;
    flags = 1;
    if (value == 3) flags = 2;
    obj->field_4 = flags;
}
