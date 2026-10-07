#include "common.h"

typedef unsigned char u8;

typedef short s16;
typedef unsigned short u16;
/* Part vtable (D_8015F308 family, vptr at +8): slot +0x1C is the kind query. */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*query)(void *self, s32 kind);
} QueryEntry800E8D0C;

typedef struct {
    u8 pad0[0x18];
    QueryEntry800E8D0C query_18;
} PartVTable800E8D0C;

typedef struct {
    s32 field0;
    s32 field4;
    PartVTable800E8D0C *vtable8;
} Part800E8D0C;

extern void *func_800E8A68(void *obj, u8 kind);
extern u16 func_8010EAF4(Part800E8D0C *part);
extern s32 func_8010CD1C(Part800E8D0C *part);

u16 func_800E8D0C(void *obj) {
    Part800E8D0C *first = func_800E8A68(obj, 3);
    Part800E8D0C *part;
    QueryEntry800E8D0C *entry;

    if (first != 0) {
        return func_8010EAF4(first);
    }
    part = func_800E8A68(obj, 4);
    if (part != 0) {
        entry = &part->vtable8->query_18;
        if (entry->query((u8 *)part + entry->delta, 2) != 0) {
            return (u16)func_8010CD1C(part);
        }
        return 0;
    }
    return 0;
}
