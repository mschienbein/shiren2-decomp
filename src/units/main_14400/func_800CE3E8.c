#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

/* Concrete collection table slots: signed count and unsigned-index getter. */
typedef struct {
    u8 pad_00[0x20];
    s16 count_delta;
    s16 count_index;
    s32 (*count)(void *self);
    u8 pad_28[0x10];
    s16 get_delta;
    s16 get_index;
    void *(*get)(void *self, u32 index);
} VTable800CE3E8;

typedef struct List800CE3E8 {
    void *pool;
    VTable800CE3E8 *vtbl;
} List800CE3E8;

typedef struct Obj Obj;

extern s32 func_800CD538(void *self, Obj *obj);
extern void func_800CD304(List800CE3E8 *obj, u32 value);
extern void func_800CD468(void *list);

/* Moves every entry of list into sub, then releases list. */
void func_800CE3E8(void *sub, List800CE3E8 *list) {
    while (list->vtbl->count((u8 *)list + list->vtbl->count_delta) != 0) {
        func_800CD538(sub, list->vtbl->get((u8 *)list + list->vtbl->get_delta, 0));
        func_800CD304(list, 0);
    }
    func_800CD468(list);
}
