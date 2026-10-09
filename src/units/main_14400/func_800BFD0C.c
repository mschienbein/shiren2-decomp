#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;


typedef struct { s32 x; s32 y; } Pos800BFD0C;
typedef struct { Pos800BFD0C start; Pos800BFD0C end; } Area800BFD0C;
typedef struct { Area800BFD0C area; u8 pad10[4]; } AreaEntry800BFD0C;
typedef struct { u8 pad0[0x3FC]; u8 side; u8 pad3FD[3]; s8 mode; } Ctx800BFD0C;

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
extern AreaEntry800BFD0C D_801431F0[];
extern Area800BFD0C D_801429C0;
void *func_800AAC20(s32 kind);
void *func_800AB9C4(void);
s32 func_800AE2A4(void *obj, s32 allowItem, s32 allowWater, void *area);

static inline void copy_pos(Pos800BFD0C *dst, Pos800BFD0C *src) {
    dst->x = src->x;
    dst->y = src->y;
}

static inline void copy_area(Area800BFD0C *dst, Area800BFD0C *src) {
    copy_pos(&dst->start, &src->start);
    copy_pos(&dst->end, &src->end);
}

void func_800BFD0C(Ctx800BFD0C *ctx) {
    s32 i;
    void *last;
    Area800BFD0C area;
    u32 bit2 = ((D_80142F18.flags >> 2) & 1);
    if (bit2 != 0) {
        return;
    }
    i = D_80142EF0.field_10;
    for (;;) {
        void *obj;
        if (--i == -1) {
            break;
        }
        obj = func_800AAC20(0);
        if (obj == 0) {
            continue;
        }
        if (ctx->mode == 11) {
            copy_area(&area, &D_801431F0[ctx->side ^ 1].area);
            func_800AE2A4(obj, 0, 0, &area);
        } else {
            func_800AE2A4(obj, 0, 0, &D_801429C0);
        }
    }
    last = func_800AB9C4();
    if (last != 0) {
        func_800AE2A4(last, 0, 0, &D_801429C0);
    }
}
