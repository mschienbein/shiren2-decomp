#include "common.h"
/* g++ 2.x descriptor; D_80152E9C selects virtual index 6 at vptr offset 0x4C. */
typedef struct {
    signed short delta, index;
    union { void (*pfn)(void *); signed short delta2; } u;
} MemberFn;
typedef struct { s32 x, y; void *target; s32 handle; } Display;
typedef struct { s32 kind, x, y, z; } Descriptor;
typedef struct Object Object;
typedef struct { void *vtable; Object *owner; MemberFn fn; } Callback;
/* Display +0x1C4 and callback +0x1D4 each occupy 0x10 bytes. */
struct Object {
    unsigned char pad00[0x1C4];
    Display display1C4;
    Callback callback1D4;
};
extern const MemberFn D_80152E9C;
extern Descriptor D_801428A4;
extern void func_80095E58(void *, Object *, MemberFn);
extern void func_800486A4(Display *, Descriptor *, void *);
void func_8009F11C(Object *object) {
    Callback *callback = &object->callback1D4;
    func_80095E58(callback, object, D_80152E9C);
    func_800486A4(&object->display1C4, &D_801428A4, callback);
}
