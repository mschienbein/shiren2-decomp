#include "common.h"
typedef struct { unsigned char pad_0[0x48]; short field_48; short field_4A; s32 (*field_4C)(void *); } Methods;
/* Whole 0x70-byte object, including the member constructed at +0x18. */
typedef struct SharedMenu {
    s32 index; void *records; s32 unknown_08; void *vtable_0C;
    s32 active_10; s32 flag_14;
    struct {
        s32 unknown_00[3]; s32 value_0C;
        s32 unknown_10[3]; s32 value_1C;
        s32 unknown_20[3]; const void *vtable_2C;
        s32 unknown_30[3]; const void *vtable_3C;
        s32 unknown_40[6];
    } member_18;
} SharedMenu;
extern SharedMenu D_80140080;
static inline s32 invoke(void *self, Methods *methods) {
    self = methods->field_48 + (unsigned char *)self;
    return methods->field_4C(self);
}
static inline s32 dispatch(SharedMenu *object) {
    s32 result;
    if (object->active_10) result = invoke(object, (Methods *)object->vtable_0C);
    else result = -1;
    return result;
}
s32 func_800426B4(void) {
    return dispatch(&D_80140080);
}
