#include "common.h"

typedef struct { s32 x; s32 y; } Pair;
typedef struct {
    unsigned char field_00[0x34];
    Pair field_34;
} Object;
extern void func_800488F0(void *, const Pair *, s32, Pair *);

void func_80095634(Object *object, const Pair *value) {
    func_800488F0(object, value, 0, &object->field_34);
    object->field_34 = *value;
}
