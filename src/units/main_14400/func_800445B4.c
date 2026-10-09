#include "common.h"
typedef struct { unsigned char pad0[0x18]; void *vtable; } Obj800445B4;
/* Address-only view of the 0x40-byte .data vtable at 0x80149F80 (header plus seven
 * {delta, pfn} entries, next label 0x80149FC0), as declared by its other users. */
extern unsigned char D_80149F80[];
void func_800442FC(void *obj);
void func_800445B4(Obj800445B4 *obj, s32 flags) {
    obj->vtable = D_80149F80;
    if (flags & 1) func_800442FC(obj);
}
