#include "common.h"

typedef struct Object Object;
/* g++ 2.x descriptor; index -1 selects the genuine function pointer member. */
typedef struct {
    signed short delta, index;
    union { void (*pfn)(void *); signed short delta2; } u;
} MemberFn;
typedef struct { s32 x, y; void *target; s32 handle; } Display;
typedef struct { s32 x, y, field_08, field_0C; } Description;
typedef struct { void *vtable; void *object; MemberFn callback; } CallbackObject;
struct Object {
    unsigned char pad_00[0x74];
    Display display_74;
    Description description_84;
    CallbackObject callback_94;
};
extern void func_8009F44C(void *object);
const MemberFn D_80152F38 = {0, -1, {func_8009F44C}};
/* The second descriptor word is a function address, not an integer. */
extern void func_80095E58(void *dst, Object *object, MemberFn callback);
extern void func_800486A4(Display *display, Description *description, void *target);

void func_8009F3D8(Object *object)
{
    CallbackObject *callback = &object->callback_94;
    func_80095E58(callback, object, D_80152F38);
    func_800486A4(&object->display_74, &object->description_84, callback);
}
