#include "common.h"

typedef unsigned char u8;

typedef struct Methods80121848 {
    u8 pad_00[8];
    short delta_08;
    short index_0A;
    void (*destroy_0C)(void *self, s32 flags);
} Methods80121848;

/* 0x14-byte item built by func_80128280: vtable at 0x08, flags at 0x0C. */
typedef struct Item80121848 {
    u8 pad_00[8];
    Methods80121848 *vtable_08;
    u8 flags_0C;
    u8 pad_0D[7];
} Item80121848;

/* Partial view: only the cell id at 0x2C is written. */
typedef struct Self80121848 {
    u8 pad_00[0x2C];
    u8 cell_2C;
} Self80121848;

/* Damage record filled by func_80136910 (0x14 bytes, padded to 0x18 on the stack);
 * field_0 holds the source pointer. */
typedef struct Damage80121848 {
    void *field_0;
    u32 field_4;
    u32 field_8;
    unsigned short field_C;
    unsigned short field_E;
    u8 field_10;
} Damage80121848;

void *func_800AC5B4(s32 size, s32 alternate);
void *func_80128280(void *p);
s32 func_800AC670(void *obj);
/* Copies the item fields from the source unit. */
void func_80128388(Item80121848 *item, void *source);
void *func_8011422C(u8 *obj);
s32 func_800CD5C0(void *container, void *item);
void func_80136910(Damage80121848 *obj, void *a, u32 c, u32 b, u32 e);
s32 func_80049CB4(s32 id, ...);
void func_800A7B68(void *entity, Damage80121848 *damage);

s32 func_80121848(Self80121848 *self, void *target)
{
    Damage80121848 damage;
    Item80121848 *item = func_80128280(func_800AC5B4(0x14, 0));
    s32 usable = func_800AC670(item) != 1;

    if (!usable) {
        return 0;
    }
    func_80128388(item, target);
    item->flags_0C |= 2;
    if (func_800CD5C0(func_8011422C((u8 *)self), item)) {
        Damage80121848 *dmg = &damage;

        self->cell_2C = 0xFF;
        func_80136910(dmg, 0, 0, 0x1E, 0);
        func_80049CB4(6);
        func_800A7B68(target, dmg);
        func_80049CB4(7);
        return 1;
    }
    if (item != 0) {
        item->vtable_08->destroy_0C((u8 *)item + item->vtable_08->delta_08, 3);
    }
    return 0;
}
