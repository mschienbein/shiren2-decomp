#include "common.h"

/* Entry filter called with the entry pointer; a result of 1 accepts the
 * entry (same slot as the callback in func_800981E8). */
typedef s32 (*EntryPredicate)(void *entry);

typedef struct {
    char pad0[0x2EC];
    EntryPredicate predicate;
} Obj;

void func_800479B0(Obj *obj, EntryPredicate predicate) {
    obj->predicate = predicate;
}
