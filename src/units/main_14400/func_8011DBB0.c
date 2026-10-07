#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char pad_00[8]; short offset_08; short field_0A; void (*method_0C)(void *, s32); } Methods;
typedef struct { unsigned char pad_00[8]; Methods *field_08; } Child;
typedef struct { unsigned char field_00; unsigned char field_01; unsigned char pad_02[0xE]; unsigned char field_10; unsigned char field_11; } Object;
extern unsigned char D_80147620[];
extern u32 D_8013960C;
extern s32 func_800ACEB4(Object *object);
extern char *func_80111654(Object *object, s32 mode);
extern void func_800C56D4(void *object);
extern Child *func_800D8BB0(Object *object, s32 value);
extern void func_800ACD34(Child *object);
extern char *func_800AE674(Child *object);
extern char *func_800ACC90(Object *object);
extern void func_800C573C(void *object);
extern char *func_80048480(u16 id);
extern s32 func_8005EF08(char *dst, const char *fmt, ...);
extern void *func_801116E0(Object *object, void *output, s32 value);
static inline s32 small_kind(s32 kind) { return kind < 3; }
void *func_8011DBB0(Object *object, void *output) {
    s32 kind = func_800ACEB4(object);
    char *text = func_80111654(object, kind);
    if (small_kind(kind) && kind > 0) {
        Child *child;
        s32 message;
        char *value;
        func_800C56D4(D_80147620);
        if (object->field_10) child = func_800D8BB0(object, object->field_10);
        else child = 0;
        message = 0x243;
        if (child) {
            if (object->field_11) {
                D_8013960C <<= 1;
                func_800ACD34(child);
                D_8013960C >>= 1;
            }
            message = 0x245;
            if (object->field_01 == 0x98) message = 0x246;
            value = func_800AE674(child);
            child->field_08->method_0C((unsigned char *)child + child->field_08->offset_08, 3);
        } else value = func_800ACC90(object);
        func_800C573C(D_80147620);
        func_8005EF08(output, func_80048480(message), value, text);
        return output;
    }
    return func_801116E0(object, output, kind);
}
