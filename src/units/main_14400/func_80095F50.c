#include "common.h"

typedef struct VTable80095F50 VTable80095F50;

typedef struct {
    char pad0[0x20];
    s32 stride;
    char pad24[0x4C - 0x24];
    VTable80095F50 *vtbl;
} Obj80095F50;

/* Widget vtable view: slot 14 (+0x70 this-adjust, +0x74 method) returns the child
 * widget of a flattened item index (null = none). */
struct VTable80095F50 {
    char pad0[0x70];
    short adjust70;
    void *(*func74)(void *self, s32 index);
};

typedef struct {
    s32 column;
    s32 row;
} Pos80095F50;

/* Child widget of the item at a cursor position (the slot result is forwarded). */
void *func_80095F50(Obj80095F50 *obj, Pos80095F50 *pos) {
    VTable80095F50 *vtbl = obj->vtbl;

    return vtbl->func74((char *)obj + vtbl->adjust70, pos->column + obj->stride * pos->row);
}
