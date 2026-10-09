#include "common.h"
typedef struct { s32 x, y; } Position;
typedef struct { unsigned char pad0[8]; Position position8; } Object;
extern void func_800AD868(Position *position);
extern s32 func_80049CB4(s32 id, ...);
void func_800CFC7C(Object *object, s32 mode) {
    if (!mode) {
        func_800AD868(&object->position8);
        func_80049CB4(0xE1, &object->position8);
    }
}
