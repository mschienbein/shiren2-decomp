#include "common.h"
typedef struct { short field00; const void *field04; } Object;
extern const s32 D_80157FA8[]; /* Address-only view of the 0x30-byte vtable. */
extern const unsigned char D_80158AD8[48];
/* The parser factory supplies its payload pointer; this action has no payload. */
Object *func_800DF8E8(Object *object, unsigned char *unused_data) {
    object->field04 = &D_80157FA8;
    object->field00 = 1;
    object->field04 = D_80158AD8;
    return object;
}
