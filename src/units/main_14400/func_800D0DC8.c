#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

s32 func_800AFD08(void *table, void *obj);

typedef struct {
    void *unk0;
} Obj800D0DC8;

/* Vtable slot +0x64 lookup(self, key, flags) (D_80154668+0x64), called by func_800D0DF0.
 * This target does not consume `flags`; it is declared to keep the slot contract. */
s32 func_800D0DC8(Obj800D0DC8 *obj, void *key, s32 flags) {
    return (u8)func_800AFD08(obj->unk0, key) != 0xFF;
}
