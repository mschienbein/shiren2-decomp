#include "common.h"

/* Collection iterator: index, collection, direction flag, current element (stored at +0xC
 * by func_800CEBA0). */
typedef struct { s32 field0; void *field4; s32 field8; void *fieldC; } Iter;
typedef struct Ent Ent;
extern Iter *func_800CEB20(Iter *, void *);
extern s32 func_800CEBA0(Iter *);
extern Ent *func_800CEC68(Iter *);
extern s32 func_800AE9AC(Ent *, s32, s32);

s32 func_800CDBC0(void *item, s32 mode)
{
    s32 total = 0;
    Iter iterator;
    func_800CEB20(&iterator, item);
    while (func_800CEBA0(&iterator)) {
        total += func_800AE9AC(func_800CEC68(&iterator), mode, 0);
    }
    return total;
}
