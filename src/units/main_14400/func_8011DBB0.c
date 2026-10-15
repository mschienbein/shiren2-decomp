#include "common.h"
#include "item_records.h"
typedef unsigned short u16;
extern unsigned char D_80147620[];
extern u32 D_8013960C;
extern s32 func_800ACEB4(void *object);
extern char *func_80111654(void *object, s32 mode);
extern void func_800C56D4(void *object);
extern Item *func_800D8BB0(void *object, s32 value);
extern void func_800ACD34(Item *object);
extern char *func_800ACC90(void *object);
extern void func_800C573C(void *object);
extern char *func_80048480(u16 id);
extern s32 func_8005EF08(char *dst, const char *fmt, ...);
extern char *func_801116E0(void *object, char *output, s32 value);
static inline s32 small_kind(s32 kind) { return kind < 3; }
char *func_8011DBB0(Item *object, char *output) {
    s32 kind = func_800ACEB4(object);
    char *text = func_80111654(object, kind);
    if (small_kind(kind) && kind > 0) {
        Item *child;
        s32 message;
        char *value;
        func_800C56D4(D_80147620);
        /* Id-0x98/0x99 constructors derive from func_8011DA10's child-charge family. */
        if (object->payload.charges.child10) child = func_800D8BB0(object, object->payload.charges.child10);
        else child = 0;
        message = 0x243;
        if (child) {
            if (object->payload.charges.childEnabled11) {
                D_8013960C <<= 1;
                func_800ACD34(child);
                D_8013960C >>= 1;
            }
            message = 0x245;
            if (object->field01 == 0x98) message = 0x246;
            value = func_800AE674(child);
            child->field08->method0C((unsigned char *)child + child->field08->delta08, 3);
        } else value = func_800ACC90(object);
        func_800C573C(D_80147620);
        func_8005EF08(output, func_80048480(message), value, text);
        return output;
    }
    return func_801116E0(object, output, kind);
}
