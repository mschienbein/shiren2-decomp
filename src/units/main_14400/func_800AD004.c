#include "common.h"
typedef struct { unsigned char field_00, field_01; } Object;
extern unsigned char D_8014303C, D_801C54C2[32];
typedef struct { Object *entries; unsigned char *occupancy; s32 capacity, count; } Pool;
extern Pool D_80143094;
extern Object *func_800AFD78(void *, unsigned char);
extern s32 func_800AC670(Object *);
extern void func_800AD188(unsigned char, unsigned char *, unsigned char *), func_800AD3F4(unsigned char), func_800ACF34(Object *), func_800AD340(unsigned char);
/* ODD_C: these accessors retain the original inline field, capacity and byte-predicate lowering. */
static inline u32 get_type(Object *object) { return object->field_00; }
static inline s32 entry_count(Pool *pool) { return pool->capacity; }
static inline unsigned char rejected(Object *object) { return func_800AC670(object) != 1; }
void func_800AD004(unsigned char value) {
    s32 i;
    if (D_8014303C == value) return;
    D_8014303C = value;
    for (i = 31; i != -1; i--) D_801C54C2[i] = 0;
    i = entry_count(&D_80143094);
    for (;;) {
        Object *object;
        unsigned char a, b;
        u32 id;
        unsigned char failed;
        unsigned char type;
        if (--i == -1) break;
        object = func_800AFD78(&D_80143094, i);
        if (!object) continue;
        failed = rejected(object);
        if (!failed) continue;
        type = get_type(object);
        id = object->field_01;
        D_801C54C2[id >> 3] |= 1 << (id & 7);
        func_800AD188(type, &a, &b);
        if (a == 100) func_800AD3F4(id);
        if (!b) continue;
        func_800ACF34(object);
    }
    for (i = 247; i != -1; i--) {
        if (!((D_801C54C2[i / 8] >> (i % 8)) & 1)) func_800AD340(i);
    }
}
