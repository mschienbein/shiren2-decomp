#include "common.h"
typedef unsigned char u8;
typedef struct { unsigned char pad0[8]; short adjust_8; short padA; void (*destroy_C)(void *, s32); } Vtable;
typedef struct { unsigned char pad0[8]; Vtable *field_8; } Obj800AE444;
extern unsigned char D_80143094[];
extern s32 func_800AFD08(void *table, void *obj);
extern void func_800AFCD8(void *table, u8 id);
/* The supplied size ABI slot is unused when recycling this fixed-pool object. */
void *func_800AC5F4(s32 size, Obj800AE444 *obj) {
    u8 id = func_800AFD08(D_80143094, obj);
    if (id != 0xFF) {
        if (obj) obj->field_8->destroy_C((unsigned char *)obj + obj->field_8->adjust_8, 3);
        func_800AFCD8(D_80143094, id);
    }
    return obj;
}
