#include "common.h"
#include "collection_iterator.h"

typedef unsigned char u8;


s32 func_801199E0(void *arg0, void *item, u8 arg2, u8 arg3);
void func_800CD3D0(void *list, u32 index);

s32 func_80119A40(void *arg0, void *arg1, u8 arg2, u8 arg3) {
    CollectionIterator it;
    s32 found = 0;

    func_800CEB20(&it, arg1);
    while (func_800CEBA0(&it)) {
        if (func_801199E0(arg0, func_800CEC68(&it), arg2, arg3)) {
            func_800CD3D0(arg1, it.index + 1);
            found = 1;
        }
    }
    return found;
}
