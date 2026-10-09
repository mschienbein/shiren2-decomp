#include "common.h"

/*
 * g++ 2.8.1 constructor of the grid-room floor generator (C++ TU).
 * Evidence: base constructor func_800B7800 is called first, then the class
 * vtable D_80153C20 (g++ {s16 delta, s16 index, pfn} entries) is stored at +8,
 * then the 16-element member array of 0x10-byte rectangles (empty constructor)
 * is built at +0x2DC (the empty count-down loop g++ emits for array
 * construction), then the
 * body runs and `this` is returned. C cannot express the array-construction
 * loop, so the construction steps are written out with placement array new.
 */

typedef unsigned char u8;
typedef signed char s8;

inline void *operator new[](unsigned int, void *place) { return place; }

/* Whole 0x26-byte floor record at D_80142EF0 (same layout as the canonical C views). */
struct FloorRecord {
    unsigned char field_00, field_01, field_02, field_03, field_04, field_05, field_06, field_07;
    unsigned char field_08, field_09, field_0A, field_0B, field_0C, field_0D;
    unsigned short field_0E;
    unsigned char field_10, field_11, field_12, field_13, field_14, field_15, field_16, field_17;
    unsigned char field_18, field_19, field_1A, field_1B, field_1C, field_1D, field_1E, field_1F;
    unsigned char field_20, field_21, field_22, field_23, field_24, field_25;
};

struct Record_800B7800;

extern "C" {
extern FloorRecord D_80142EF0;
extern s8 *D_80153BDC[];
extern s8 *D_80153BF8[];
extern u8 D_80153C20[];
Record_800B7800 *func_800B7800(Record_800B7800 *record, unsigned char kind, signed char variant, s32 flag, u32 value);
void func_80043550(void *self);
}

struct Dims {
    u8 w;
    u8 h;
    u8 d;
};

struct Point {
    s32 x;
    s32 y;
};

/* 0x10-byte room rectangle (same layout as func_800BA04C's Rect rects[16]); empty constructor. */
struct Rect {
    Point start;
    Point end;
    Rect() {}
};

struct Cell { u8 kind, walls, x, y; };

/* Partial view of the 0x400-byte generator object. */
struct GridGenerator {
    char pad0[8];
    const void *vtable;
    char padC[4];
    Dims dims;
    /* ROM 0x8B6C8..0x8B6D0: the initializer owns both complete grids. */
    Cell cells[11][8];
    u8 pad173;
    s32 available[11][8];
    s8 *row_edges;
    s8 *col_edges;
    Rect rects[16];
    s32 room_count;
    char pad3E0[0x3E8 - 0x3E0];
    s32 unk3E8;
    s32 unk3EC;
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    s8 unk3FC;
};

extern "C" GridGenerator *func_800BA558(GridGenerator *self, u8 kind, s8 variant, s32 flag, s32 value)
{
    func_800B7800((Record_800B7800 *)self, kind, variant, flag, value);
    self->vtable = D_80153C20;
    new (self->rects) Rect[16];

    /* Grid dimensions come from bytes +1..+3 of the floor record. */
    const u8 *src = (const u8 *)&D_80142EF0;
    self->dims.w = src[1];
    src++;
    self->dims.h = src[1];
    self->dims.d = src[2];
    if (self->dims.w < 3) {
        self->dims.w = 3;
    }
    if (self->dims.h < 3) {
        self->dims.h = 3;
    }
    if (self->dims.w >= 9) {
        self->dims.w = 8;
    }
    if (self->dims.h >= 6) {
        self->dims.h = 5;
    }
    s8 *row_table = D_80153BDC[self->dims.w - 3];
    s8 *col_table = D_80153BF8[self->dims.h - 3];
    self->row_edges = row_table;
    self->col_edges = col_table;
    func_80043550(self);
    self->unk3E8 = 0;
    self->unk3EC = 0;
    self->unk3F0 = 0;
    self->unk3F4 = 0;
    self->unk3F8 = 0;
    self->unk3FC = -1;
    return self;
}
