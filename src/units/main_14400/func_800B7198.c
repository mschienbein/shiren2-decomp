#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef struct { s32 first, second; } Pair;
typedef struct { short adjust; unsigned short reserved; void (*call)(void *); } Entry;
typedef struct { u8 pad[0x24]; Entry *field24; } Object;
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
extern s32 func_80046240(void);
extern Object *func_800AA63C(void);
extern s32 func_800A5B98(Object *, Pair *);
extern void func_800A58FC(Object *, Pair *);
extern void func_800F0130(Object *);
/* The map virtual slot supplies an owner; the spawning operation does not inspect it. */
void func_800B7198(void *owner) {
    s32 skip = 0;
    s32 i;
    Pair position;
    if (!func_80046240()) {
        if (((D_80142F18.flags >> 2) & 1) || ((D_80142F18.flags >> 4) & 1)) skip = 1;
    }
    if (skip) return;
    i = D_80142EF0.field_05;
    for (;;) {
        Object *obj;
        if (--i == -1) break;
        obj = func_800AA63C();
        if (!obj) continue;
        if (func_800A5B98(obj, &position)) {
            func_800A58FC(obj, &position);
            func_800F0130(obj);
        } else {
            obj->field24[3].call((u8 *)obj + obj->field24[3].adjust);
        }
    }
}
