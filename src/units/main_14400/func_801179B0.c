#include "common.h"
typedef struct { s32 field_0, field_4; } Pair;
/* Actor status slot +0x94: func_800E115C / func_800F212C. */
typedef struct { char fields_0[0x90]; short adjustment_90; s32 (*method_94)(void *, s32, s32, unsigned char, s32); } Interface;
typedef struct { Pair position; char fields_8[0x1C]; Interface *field_24; } Object;
extern s32 func_800E1CC4(Object *, s32);
extern char *func_800A3B20(Object *);
extern s32 func_80049CB4(s32, ...);
extern void func_800497F0(s32, ...);
static inline void copy_pair(Pair *destination, const Pair *source) {
    destination->field_0 = source->field_0;
    destination->field_4 = source->field_4;
}
/* The original pair-action slot supplies both unused receiver and source pointers. */
void func_801179B0(void *unused0, void *unused1, Object *object) {
    Pair position;
    char *old_value;
    s32 message;
    if (func_800E1CC4(object, 1)) {
        old_value = func_800A3B20(object);
        object->field_24->method_94((char *)object + object->field_24->adjustment_90, 1, 1, 0, 0);
        copy_pair(&position, &object->position);
        message = func_80049CB4(0xDA, &position);
        func_800497F0(0xCB, message, old_value, func_800A3B20(object));
        func_80049CB4(0xD7, &position);
    }
}
