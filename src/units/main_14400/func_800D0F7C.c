#include "common.h"
#include "item_records.h"

/* Item container: pool +0, +4 word, modified flag +8. Moves `b` from this container's pool
 * into the pool at *c. */
typedef struct { ItemPool *field_0; s32 field_4; s32 field_8; } Obj;
void func_800D0F7C(Obj *o, Item *b, ItemPool **c) {
    func_800AFFD0(o->field_0, b, *c);
    o->field_8 = 1;
}
