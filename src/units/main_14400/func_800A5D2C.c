#include "common.h"
typedef unsigned short u16;
typedef struct { s32 field_00; s32 field_04; } Value;
typedef struct { s32 fields[6]; } Iterator;
extern void *func_800C5280(Iterator *iterator, Value *value, u16 flags);
extern s32 func_800C559C(Iterator *iterator);
extern void *func_800C532C(Value *value, Iterator *iterator);
extern s32 func_800A4314(void *object, Value *value);
/* flags is an int: func_800A5DC4 forwards its own a2 unmasked (0x800A5DD8); the body
 * narrows it to 16 bits for func_800C5280 (andi a2,a2,0xFFFF at 0x800A5D5C). */
s32 func_800A5D2C(void *object, Value *output, s32 flags) {
    Iterator iterator;
    Value value;
    value.field_00 = output->field_00;
    value.field_04 = output->field_04;
    func_800C5280(&iterator, &value, flags);
    for (;;) {
        s32 active = func_800C559C(&iterator);
        if (!active) break;
        func_800C532C(&value, &iterator);
        if (func_800A4314(object, &value)) {
            *output = value;
            return 1;
        }
    }
    return 0;
}
