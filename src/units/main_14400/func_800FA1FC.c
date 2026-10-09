#include "common.h"
typedef struct { unsigned char pad0[0x20]; short adjustment20; short pad22; s32 (*test24)(void *); } ItemVTable;
typedef struct { unsigned char pad0[4]; ItemVTable *vtable4; } Item;
typedef struct { unsigned char pad0[0x98]; short adjustment98; short pad9A; Item *(*item9C)(void *); } ObjectVTable;
typedef struct { unsigned char pad0[0xA]; unsigned char kindA; unsigned char padB[0x13]; unsigned char flags1E; unsigned char pad1F[5]; ObjectVTable *vtable24; unsigned char pad28[0x4A]; unsigned char flags72; } Object;
/* The action predicate supplies its owner even though only the target is used. */
s32 func_800FA1FC(void *unused, Object *object) {
    if (object->flags1E & 0x7C) {
        if (object->flags1E & 0xC) {
            ObjectVTable *table = object->vtable24;
            Item *item = table->item9C((unsigned char *)object + table->adjustment98);
            if (item) {
                ItemVTable *item_table = item->vtable4;
                return item_table->test24((unsigned char *)item + item_table->adjustment20) != 0;
            }
        } else if (object->kindA == 0x5B) return 1;
        {
            s32 result = 0;
            if (object->flags72 & 8) result = 1;
            return result;
        }
    }
    return 0;
}
