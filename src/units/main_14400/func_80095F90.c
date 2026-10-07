#include "common.h"

typedef short s16;
typedef struct Obj Obj;
/* Widget vtable view: slot 13 (+0x68 this-adjust, +0x6C method) returns the value of a
 * flattened item index (0x80000000 = none). */
typedef struct { char pad[0x68]; s16 offset68; char pad6A[2]; s32 (*func6C)(void *self, s32 index); } VTable;
struct Obj { char pad[0x20]; s32 stride20; char pad24[0x28]; VTable *vtbl4C; };
/* Cursor position {column, row}. */
typedef struct { s32 base; s32 index; } Arg;
/* Widget vtable slot 15 (+0x7C): value of the item at a cursor position. Callers consume
 * the result (func_80046CB0, func_800957C0, func_80095C8C), so it is forwarded. */
s32 func_80095F90(Obj *obj, Arg *arg) {
    return obj->vtbl4C->func6C((char *)obj + obj->vtbl4C->offset68, arg->base + obj->stride20 * arg->index);
}
