#include "common.h"
typedef struct { s32 first, second; } Pair;
/* Method entry: receiver adjustment, call; entry 1 (+8/+0xC of D_801599F8) is func_800F6B60(self, flags). */
typedef struct { short adjust; unsigned short reserved; void (*call)(void *, s32); } Entry;
typedef struct { unsigned char pad0[0x24]; Entry *field24; unsigned char pad28[0x58]; void *field80; } Object;
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
extern Object *func_800F6004(unsigned char);
extern s32 func_800D1E90(void *, Object *, Pair *, Pair *, Pair *, s32);
extern void func_800A58FC(Object *, Pair *);
extern void *func_800A6538(unsigned char *, Object *, Pair *);
extern void func_800A665C(Object *, unsigned char *);
extern void func_800F61B4(Object *, Pair *, Pair *);
void func_800D2260(void *owner, Pair *origin) {
    Pair position, direction;
    unsigned char facing;
    Object *obj = func_800F6004(D_80142EF0.field_1F);
    if (obj) {
        s32 failed = func_800D1E90(owner, obj, origin, &position, &direction, 1) ^ 1;
        if (failed) {
            obj->field24[1].call((unsigned char *)obj + obj->field24[1].adjust, 3);
        } else {
            func_800A58FC(obj, &position);
            func_800A6538(&facing, obj, &direction);
            func_800A665C(obj, &facing);
            obj->field80 = owner;
            func_800F61B4(obj, &position, &direction);
        }
    }
}
