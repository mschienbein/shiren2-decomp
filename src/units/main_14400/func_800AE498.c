#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct {
    u8 pad[0x10];
    s16 delta;
    s16 pad12;
    s32 (*fn)(void *);
} VTable;
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 flags;
    u8 pad3[5];
    VTable *vtable;
} Obj;
extern u32 D_8013960C;
s32 func_80049CB4(s32 id, ...);
char *func_800AE674(void *obj);
void func_800498E4(s32 message_id, ...);

s32 func_800AE498(Obj *obj)
{
    if (obj->flags & 4) {
        if (obj->vtable->fn((u8 *)obj + obj->vtable->delta)) {
            if (D_8013960C & 1) {
                func_80049CB4(0x12B);
                func_800498E4(0x2F, func_800AE674(obj));
            }
            return 0;
        }
    }
    return 1;
}
