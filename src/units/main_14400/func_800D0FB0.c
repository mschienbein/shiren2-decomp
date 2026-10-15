#include "common.h"
#include "item_records.h"

/* Item container: pool +0, +4 word, modified flag +8. Moves `arg1` from the pool at *arg2
 * into this container's pool. */
typedef struct { ItemPool *f0; s32 f4; s32 f8; } S;
void func_800D0FB0(S *s, Item *arg1, ItemPool **arg2) {
    func_800AFFD0(*arg2, arg1, s->f0);
    s->f8 = 1;
}
