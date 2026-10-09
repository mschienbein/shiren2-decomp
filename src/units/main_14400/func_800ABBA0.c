#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef struct { void *header, *descriptor, *table_a, *table_b, *table_c; u8 metadata[4]; } Resource;

extern Resource D_80142B1C[21];

extern u8 D_00194FC0[];
extern u8 D_80142E88[], D_80142DE0[], D_80142D28[];
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
extern void func_800ABB50(void *, u8, u8);
extern void func_8006AC30(void *, void *, void *, s32, s32, s32);
void func_800ABBA0(u8 index, u8 row) {
    void *address;
    func_800ABB50(&D_80142F18, index, row);
    func_8006AC30(&D_80142EF0, D_00194FC0, D_80142B1C[index].descriptor, 0x26, D_80142F18.row, 1);
    func_8006AC30(&address, D_00194FC0, D_80142B1C[index].table_a, 4, D_80142F18.row, 1);
    func_8006AC30(D_80142E88, D_00194FC0, address, 4, 0, 0x1A);
    func_8006AC30(&address, D_00194FC0, D_80142B1C[index].table_b, 4, D_80142F18.row, 1);
    func_8006AC30(D_80142DE0, D_00194FC0, address, 8, 0, 0x15);
    func_8006AC30(&address, D_00194FC0, D_80142B1C[index].table_c, 4, D_80142F18.row, 1);
    func_8006AC30(D_80142D28, D_00194FC0, address, 2, 0, 0x19);
}
