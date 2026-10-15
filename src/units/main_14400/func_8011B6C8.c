#include "map_unit_iterator.h"
typedef unsigned char u8;
extern s32 func_800A692C(void *object, s32 kind);
extern s32 func_8011B5C0(void *object, void *entry);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 message_id, ...);
static inline void copy_value(MapPos *destination, MapPos *source) {
    destination->x = source->x;
    destination->y = source->y;
}
/* Item-effect slot +0x44 also supplies an item; this override does not use it. */
void func_8011B6C8(void *object, MapPos *input, void *item) {
    MapPos value;
    MapRect range;
    MapUnitIterator iterator;
    s32 success = 0;
    copy_value(&value, input);
    func_800A2FD0(&range, &value, 1);
    func_800A9244(&iterator, &range, &value);
    for (;;) {
        s32 active = func_800A9284(&iterator, 12);
        void *entry;
        s32 allowed;
        if (!active) break;
        entry = func_800A942C(&iterator);
        allowed = func_800A692C(entry, 10) != 1;
        if (allowed) success = (success | func_8011B5C0(object, entry)) != 0;
    }
    if (!success) {
        func_80049CB4(0x132);
        func_800498E4(0x222);
    }
}
