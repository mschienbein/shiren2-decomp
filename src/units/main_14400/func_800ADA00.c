#include "common.h"
#include "item_records.h"

s32 func_800B56F0(void *object);
s32 func_800AE324(Item *a, void *b);
s32 func_800B5650(void *p);
void func_800AD7E0(void *obj, void *pos, s32 notify);

s32 func_800ADA00(Item *a, void *b) {
    if (func_800B56F0(b) != 0) {
        if (func_800AE324(a, b) != 0) {
            return 0;
        }
        func_800B5650(b);
    }
    func_800AD7E0(a, b, 1);
    return 1;
}
