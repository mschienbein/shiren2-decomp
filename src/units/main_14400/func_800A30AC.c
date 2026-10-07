#include "common.h"
typedef unsigned char u8;
typedef struct { s32 words[2]; } Value;
typedef struct { u8 value; } Dir;
extern void *func_800A25D8(Value *value, void *input, Dir type, s32 index);
static inline Dir *make_type(Dir *type, unsigned char value) {
    type->value = value;
    return type;
}
void func_800A30AC(Value *output, void *input, unsigned char index) {
    Value first;
    Value second;
    Dir first_type;
    Dir second_type;
    func_800A25D8(&first, input, *make_type(&first_type, 3), index);
    func_800A25D8(&second, input, *make_type(&second_type, 7), index);
    output[0] = first;
    output[1] = second;
}
