#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef float f32;
/* Default instrument record (see func_8012A7F8). */
typedef struct Record_8012A7C4 Record_8012A7C4;

/* 0x13C-byte voice record (see func_8012C138 / func_8012C4B0). */
typedef struct Voice8012C004 {
    u8 pad_00[0x4];
    u8 *active_04;
    u8 pad_08[0x10 - 0x8];
    s32 field_10;
    u8 pad_14[0x28 - 0x14];
    f32 field_28;
    u8 pad_2C[0x58 - 0x2C];
    f32 field_58;
    f32 field_5C;
    f32 field_60;
    u8 pad_64[0x6C - 0x64];
    f32 field_6C;
    u8 pad_70[0x7C - 0x70];
    Record_8012A7C4 *record_7C;
    u8 pad_80[0x98 - 0x80];
    u16 field_98;
    u16 field_9A;
    u16 field_9C;
    u16 field_9E;
    u16 field_A0;
    u16 field_A2;
    u16 field_A4;
    u8 pad_A6[0xA8 - 0xA6];
    u16 field_A8;
    u8 pad_AA[0xB0 - 0xAA];
    u16 field_B0;
    u8 pad_B2[0xBC - 0xB2];
    u8 field_BC;
    u8 field_BD;
    u8 field_BE;
    u8 field_BF;
    u8 pad_C0[0xC1 - 0xC0];
    u8 field_C1;
    u8 field_C2;
    u8 pad_C3[0xC6 - 0xC3];
    u8 field_C6;
    u8 field_C7;
    u8 field_C8;
    u8 field_C9;
    u8 pad_CA[0xCC - 0xCA];
    u8 field_CC;
    u8 pad_CD[0xD3 - 0xCD];
    u8 field_D3;
    u8 pad_D4[0x13C - 0xD4];
} Voice8012C004;

extern s32 D_801CA6E4;
extern Record_8012A7C4 *D_801CA6F8;
extern Record_8012A7C4 *D_801CA6FC;

/* Reset a voice to its defaults, keeping only byte 0xC9. */
void func_8012C004(Voice8012C004 *voice) {
    u8 keep = voice->field_C9;
    u8 *p = (u8 *)voice;
    u32 i;
    s32 period;
    Record_8012A7C4 *source;
    s32 none;
    s32 full;
    s32 one;

    voice->active_04 = 0;
    for (i = 0; i < sizeof(Voice8012C004); i++) {
        *p++ = 0;
    }
    none = 0xFF;
    voice->field_CC = none;
    voice->field_BE = none;
    period = 0x6000 / D_801CA6E4;
    full = 0x7F;
    one = 1;
    voice->field_D3 = full;
    voice->field_BD = 0x40;
    voice->field_9A = one;
    voice->field_C8 = 0xF;
    voice->field_BC = full;
    voice->field_BF = one;
    voice->field_C6 = one;
    voice->field_C1 = full;
    voice->field_C7 = none;
    voice->field_C2 = full;
    voice->field_A2 = one;
    voice->field_A0 = 0xFFFF;
    source = D_801CA6F8;
    voice->field_10 = -1;
    voice->field_9E = 0x80;
    voice->field_B0 = 0x80;
    voice->field_98 = 0x80;
    voice->field_28 = 99.9f;
    voice->field_6C = 0.03125f;
    voice->field_58 = 1.0f;
    voice->field_5C = 1.0f / 255.0f;
    voice->field_A8 = period;
    voice->field_A4 = one;
    voice->field_60 = 1.0f / 15.0f;
    voice->field_9C = period;
    if (source == 0) {
        source = D_801CA6FC;
    }
    voice->record_7C = source;
    voice->field_C9 = keep;
}
