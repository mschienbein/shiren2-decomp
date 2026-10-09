#include "common.h"
typedef struct { s32 index; } Iterator;
extern s32 func_800A9070(Iterator *iterator, s32 kind);
extern void *func_800A910C(Iterator *iterator);
extern void func_800F8494(void *object);
/* The caller supplies a receiver, but this method iterates the global objects. */
void func_800EBF94(void *self) {
    Iterator iterator;
    iterator.index = 0;
    while (func_800A9070(&iterator, 0x5B)) func_800F8494(func_800A910C(&iterator));
}
