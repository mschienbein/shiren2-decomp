#include "common.h"

typedef struct {
    unsigned char pad0[0x53];
    unsigned char count;
} Obj;

s32 func_8009E338(Obj *obj, s32 index) {
    if (index < 0 || index >= obj->count) {
        return 0x80000000;
    }
    return index;
}
