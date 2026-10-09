#include "common.h"
typedef struct { unsigned char field_0[0x4C]; const void *field_4C; unsigned char field_50[0x58]; const void *field_A8; unsigned char field_AC[0x11C]; const void *field_1C8; unsigned char field_1CC[0x108]; const void *field_2D4; unsigned char field_2D8[0x48]; } Part;
typedef struct { unsigned char field_0[0x4C]; const void *field_4C; unsigned char field_50[0x14]; Part field_64, field_384; } Object;
extern Object D_80141DB4;
/* 0x90-byte derived menu vtables in overlay_1339f0 (0x801E80A0..0x801E812F, 0x801E8130..0x801E81BF). */
extern const unsigned char D_801E80A0[144], D_801E8130[144];
extern const unsigned char D_80151E38[144], D_80152968[152];
static inline Part *begin_part(Part *part) { part->field_4C = D_801E8130; return part; }
static inline void destroy_object(Object *self) {
    Part *part;
    self->field_4C = D_801E80A0;
    part = begin_part(&self->field_384);
    self->field_384.field_2D4 = D_80151E38;
    self->field_384.field_1C8 = D_80151E38;
    part->field_4C = D_80152968;
    self->field_384.field_A8 = D_80151E38;
    part->field_4C = D_80151E38;
    part = begin_part(&self->field_64);
    self->field_64.field_2D4 = D_80151E38;
    self->field_64.field_1C8 = D_80151E38;
    part->field_4C = D_80152968;
    self->field_64.field_A8 = D_80151E38;
    part->field_4C = D_80151E38;
    self->field_4C = D_80151E38;
}
void func_8009D288(void) { s32 local[4]; destroy_object(&D_80141DB4); }
