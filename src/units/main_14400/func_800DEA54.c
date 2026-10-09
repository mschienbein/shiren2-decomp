#include "common.h"

typedef struct { short delta, index; void *(*call)(void *); } Entry;
typedef struct { char pad0[0x24]; Entry *vtbl_24; } Actor;
typedef struct { char pad0[0xA]; unsigned char kind_A; } Item;
typedef struct { char pad0[0xB8]; void **field_B8; char padBC[8]; void *field_C4; } Object;
extern Actor *D_801476B8;
extern void *func_800A6D40(Actor *obj);
extern void *func_800A6DE4(Item *obj);
/* Original table D_8015483C contains object addresses, not integer values. */
extern void *func_800D4EA0(u32 index);

void func_800DEA54(Object *obj) {
    Actor *actor = D_801476B8;
    Entry *entry = &actor->vtbl_24[19];
    void *value = entry->call((char *)actor + entry->delta);
    if (*obj->field_B8 == value) {
        Item *item = func_800A6D40(D_801476B8);
        obj->field_C4 = func_800A6DE4(item);
        if (obj->field_C4 == 0) {
            if ((unsigned char)(item->kind_A + 0x6A) < 0x10) {
                obj->field_C4 = func_800D4EA0(1);
            } else {
                obj->field_C4 = func_800D4EA0(0);
            }
        }
    } else {
        obj->field_C4 = value;
    }
}
