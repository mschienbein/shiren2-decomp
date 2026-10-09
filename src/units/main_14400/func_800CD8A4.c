#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;
/* Item vtable at +8: slot +0x18/+0x1C is the s32 (self, s32 kind) item test (same view as func_800AE710). */
typedef struct { u8 pad_0[0x18]; s16 delta_18, index_1A; s32 (*test_1C)(void *, s32); } ItemVTable;
typedef struct { u8 field_0, id_1; u8 pad_2[3]; s8 field_5; u8 pad_6[2]; ItemVTable *vtable_8; } Obj;
/* List vtable at +4: slot +0x48/+0x4C is void remove(self, s32 index) (same view as func_800CD304). */
typedef struct { u8 pad_0[0x48]; s16 delta_48, index_4A; void (*remove_4C)(void *, s32); } ContainerVTable;
typedef struct { void *data_0; ContainerVTable *vtable_4; } Container;
extern s32 func_800CD090(void *container, void *element);
extern u16 func_800AE710(Obj *o);
extern void *func_800AC244(u8 id);
extern s16 func_800AE754(void *object, s16 add);
extern void func_800AE974(Obj *p, s8 v);

/* Take `item` out of `container`: a stack of two or more splits off one new copy
 * (decrementing the stack), otherwise the item itself is removed from the list. */
void *func_800CD8A4(void *container, Obj *item) {
    Obj *copy;
    s32 split;
    s32 index;
    ItemVTable *iv;
    ContainerVTable *v;
    index = func_800CD090(container, item);
    if (index < 0) return 0;
    split = 0;
    iv = item->vtable_8;
    if (iv->test_1C((char *)item + iv->delta_18, 0x1E))
        split = (u32)func_800AE710(item) >= 2;
    if (split) {
        copy = func_800AC244(item->id_1);
        if (copy != 0) {
            func_800AE754(item, -1);
            func_800AE974(copy, item->field_5);
        }
        return copy;
    } else {
        Container *self = container;
        v = self->vtable_4;
        v->remove_4C((char *)self + v->delta_48, index);
        return item;
    }
}
