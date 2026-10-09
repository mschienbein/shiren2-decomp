#include "common.h"
/* Slot +0x0C targets func_800C5CA4 (0x80154064) and func_800C5E10 (0x80154094) return a
 * signed RNG word; consumers (0x800C59EC, 0x800CAC80) use this wrapper's unsigned result. */
typedef struct { unsigned char field_00[8]; short field_08; s32 (*field_0c)(void *); } Methods;
typedef struct { unsigned char field_00[12]; Methods *field_0c; } Object;
unsigned int func_800C598C(Object *self) {
    Methods *methods = self->field_0c;
    return (unsigned int)methods->field_0c((unsigned char *)self + methods->field_08);
}
