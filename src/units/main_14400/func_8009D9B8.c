#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x53];
    u8 count;
} Obj8009D9B8;

s32 func_8009D9B8(Obj8009D9B8 *obj, s32 index) {
    if (index < 0 || index >= obj->count) {
        return 0x80000000;
    }
    return index;
}
