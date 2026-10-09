#include "common.h"
typedef unsigned char u8;
/* Collection iterator: index, collection, direction flag, current element. */
typedef struct { s32 field0; void *field4; s32 field8; void *fieldC; } Iter;
typedef struct { u8 kind; } Ent;
/* D_80158E80 family table at +0x24: +0x9C item list accessor. */
typedef struct { u8 pad_00[0x98]; short delta_98; short pad_9A; void *(*items_9C)(void *self); } UnitTable;
typedef struct { u8 pad_00[0x24]; UnitTable *table_24; } Unit;
extern Iter *func_800CEB20(Iter *, void *);
extern s32 func_800CEBA0(Iter *);
extern Ent *func_800CEC68(Iter *);
extern s32 func_800E0F40(void *obj);
s32 func_80104C9C(void *object, Unit *context, s32 desired) {
    Iter iterator;
    s32 count = 0;
    func_800CEB20(&iterator, context->table_24->items_9C((u8 *)context + context->table_24->delta_98));
    while (func_800CEBA0(&iterator)) {
        s32 code = func_800CEC68(&iterator)->kind;
        u8 kind = code;
        u32 level = (u8)func_800E0F40(object);
        if (kind == 7 || (level >= 2 && kind == 9) || (level >= 3 && (u32)(code - 3) < 2)) {
            count++;
            if (count == desired) return iterator.field0 + 1;
        }
    }
    return count;
}
