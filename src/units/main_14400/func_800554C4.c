#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Combiner state at task +0x14. */
typedef struct {
    s32 field_0;
    s32 field_4;
    u8 mux[16];
} Combiner;

/* 0x60-byte task record of the 32-entry table D_801D40DC; field_44 == -1 marks a free slot. */
typedef struct {
    u8 field_00;
    u8 field_01;
    u8 pad02[2];
    s32 field_04;
    s32 field_08;
    s32 field_0C;
    s32 field_10;
    Combiner combiner;
    float x;
    float y;
    float scale_x;
    float scale_y;
    u8 field_3C;
    u8 pad3D[3];
    s32 field_40;
    s32 owner;
    s32 field_48;
    u8 field_4C;
    u8 field_4D;
    u8 field_4E;
    u8 pad4F[0x5E - 0x4F];
    s16 field_5E;
} Task;

extern Task D_801D40DC[32];

s32 func_800554C4(s32 a, s32 b, s32 c, s32 d)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        if (D_801D40DC[i].owner == -1) {
            Combiner *cc;

            D_801D40DC[i].x = b;
            D_801D40DC[i].y = c;
            D_801D40DC[i].scale_x = 1.0f;
            D_801D40DC[i].scale_y = 1.0f;
            D_801D40DC[i].field_40 = d;
            D_801D40DC[i].field_3C = 0;
            D_801D40DC[i].field_00 = 2;
            D_801D40DC[i].field_01 = 9;
            D_801D40DC[i].field_08 = 0x200004;
            D_801D40DC[i].field_04 = 0;
            D_801D40DC[i].field_0C = 0x404240;
            D_801D40DC[i].field_10 = 0x104240;
            D_801D40DC[i].combiner.field_0 = -1;
            D_801D40DC[i].combiner.field_4 = -1;
            cc = &D_801D40DC[i].combiner;
            cc->mux[0] = 3;
            cc->mux[1] = 5;
            cc->mux[2] = 1;
            cc->mux[3] = 5;
            cc->mux[4] = 1;
            cc->mux[5] = 7;
            cc->mux[6] = 3;
            cc->mux[7] = 7;
            cc->mux[8] = 3;
            cc->mux[9] = 5;
            cc->mux[10] = 1;
            cc->mux[11] = 5;
            cc->mux[12] = 1;
            cc->mux[13] = 7;
            cc->mux[14] = 3;
            cc->mux[15] = 7;
            D_801D40DC[i].owner = a;
            D_801D40DC[i].field_48 = 0;
            D_801D40DC[i].field_4C = 0;
            D_801D40DC[i].field_4D = 0xFE;
            D_801D40DC[i].field_4E = 0;
            D_801D40DC[i].field_5E = 2;
            return i;
        }
    }
    return -1;
}
