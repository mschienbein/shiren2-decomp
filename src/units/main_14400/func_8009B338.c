#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x80];
    s32 state;
} Obj8009B338;

void func_8009B374(Obj8009B338 *obj, s32 direction);
void func_8009B5D8(Obj8009B338 *obj, s32 direction);

void func_8009B338(Obj8009B338 *obj, s32 direction) {
    if (obj->state == 3) {
        func_8009B374(obj, direction);
    } else {
        func_8009B5D8(obj, direction);
    }
}
