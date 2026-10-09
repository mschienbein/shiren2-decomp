#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x300]; u8 rows[3][60]; s32 boundaries[4]; } Object;
s32 func_80098DD4(Object *object, s32 id) {
    s32 row;
    for (row = 0; row < 3; row++) {
        if (id < object->boundaries[row + 1]) return object->rows[row][id - object->boundaries[row]];
    }
    return 0;
}
