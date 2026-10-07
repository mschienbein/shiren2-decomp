#include "common.h"

typedef unsigned char u8;

typedef float f32;

typedef struct {
    s32 field0;
    f32 value4;
} Inner8008CE34;

typedef struct {
    u8 pad0[0xC];
    Inner8008CE34 *innerC;
} Mid8008CE34;

typedef struct {
    u8 pad0[0x74];
    Mid8008CE34 *mid74;
} Obj8008CE34;

f32 func_8008CE34(Obj8008CE34 *obj) {
    Inner8008CE34 *inner = obj->mid74->innerC;

    if (inner != 0) {
        return inner->value4;
    }
    return 0.0f;
}
