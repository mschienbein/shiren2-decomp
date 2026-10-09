#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
/* Whole 0x26-byte floor record at D_80142EF0 (0x80142EF0..0x80142F15): func_800ABBA0
 * fills it with one func_8006AC30 copy (stride 0x26, count 1); bytes are read with lbu
 * at +0x00..+0x25 and the halfword at +0xE with lhu (func_800AB044). */
typedef struct {
    unsigned char field_00, field_01, field_02, field_03, field_04, field_05, field_06, field_07;
    unsigned char field_08, field_09, field_0A, field_0B, field_0C, field_0D;
    unsigned short field_0E;
    unsigned char field_10, field_11, field_12, field_13, field_14, field_15, field_16, field_17;
    unsigned char field_18, field_19, field_1A, field_1B, field_1C, field_1D, field_1E, field_1F;
    unsigned char field_20, field_21, field_22, field_23, field_24, field_25;
} FloorRecord;
extern FloorRecord D_80142EF0;
typedef struct { s32 x0, y0, x1, y1; } Rect;
extern Rect D_801429C0;
extern s32 func_80046240(void);
extern void *func_800AAC20(s32);
extern void *func_800AB9C4(void);
extern s32 func_800AE2A4(void *, s32, s32, void *);
/* The map virtual slot supplies an owner; placement uses the global bounds instead. */
void func_800B7274(void *owner) {
    s32 blocked = 0;
    if (!func_80046240()) { s32 flag = (D_80142F18.flags >> 2) & 1; if (flag) blocked = 1; else { flag = (D_80142F18.flags >> 4) & 1; if (flag) blocked = 1; } }
    if (!blocked) { s32 i = D_80142EF0.field_10; for (;;) { void *p; --i; if (i == -1) break; p = func_800AAC20(0); if (!p) continue; func_800AE2A4(p, 0, 0, &D_801429C0); } { void *p = func_800AB9C4(); if (p) func_800AE2A4(p, 0, 0, &D_801429C0); } }
}
