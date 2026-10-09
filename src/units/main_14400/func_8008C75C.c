#include "common.h"
/* Record built by func_8008D3A0: tag, id, hook and apply slots precede the cleanup
 * callback at +0x10, which receives the record itself. */
typedef struct Item { unsigned char pad_0[0x10]; void (*field_10)(struct Item *); } Item;
/* Group list released by func_8008D6AC: byte count plus pointer to 0x14-byte records. */
typedef struct { unsigned char count; unsigned char pad_1[3]; void *records; } GroupList;
typedef struct { unsigned char field_0, field_1, field_2, field_3; s32 field_4; Item **field_8; void *field_C; GroupList field_10; u32 field_18; Item **field_1C; } Object;
extern void func_80091544(void *);
extern void func_8008D6AC(void *);
extern void func_8008DB10(void *);
void func_8008C75C(Object *self) {
    u32 i;
    if (!self->field_0) return;
    if (self->field_8) {
        for (i = 0; i < self->field_2; ++i) {
            Item *item = self->field_8[i];
            if (item) { item->field_10(item); func_80091544(self->field_8[i]); }
        }
        func_80091544(self->field_8);
        self->field_8 = 0;
    }
    if (self->field_1C) {
        for (i = 0; i < self->field_18; ++i) {
            Item *item = self->field_1C[i];
            if (item) { item->field_10(item); func_80091544(self->field_1C[i]); }
        }
        func_80091544(self->field_1C);
        self->field_1C = 0;
    }
    func_8008D6AC(&self->field_10);
    if (self->field_C) { func_8008DB10(self->field_C); func_80091544(self->field_C); self->field_C = 0; }
    self->field_0 = 0;
}
