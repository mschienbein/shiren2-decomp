#include "common.h"
typedef unsigned char u8;
typedef struct { u8 *field_0; u8 *field_4; } Buffer;
typedef struct { Buffer *field_0; signed char field_4; } Object;
extern void func_80094B3C(void *, s32);
extern void *func_800D8FB0(u32);
extern void *func_800DA160(void *, unsigned short);
extern void func_80094AFC(Object *, void *);
extern void *func_80093A60(Object *);
extern void *func_800C9E10(void);
extern void func_800452C0(void *);
static inline s32 state(Object *object) {
    return object->field_4;
}
static inline s32 unavailable(Object *object) {
    return state(object) ^ 1;
}
void *func_80094C04(Object *object, u8 value) {
    void *message;
    if (!unavailable(object)) {
        if (object->field_0->field_4 - object->field_0->field_0 <= 0) {
            func_80094B3C(object, 0);
            message = func_800DA160(func_800D8FB0(0xC), value);
            func_80094AFC(object, message);
            func_80094B3C(object, 1);
            return message;
        }
        message = func_80093A60(object);
        if (message) return message;
    }
    message = func_800DA160(func_800D8FB0(0xC), value);
    func_80094AFC(object, message);
    func_800452C0(func_800C9E10());
    return message;
}
