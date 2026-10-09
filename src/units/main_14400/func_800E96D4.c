#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 pad00[0x98]; s16 delta98, index9A; void *(*get9C)(void *); } VTable;
typedef struct { u8 pad00[0x24]; VTable *vtable24; } Obj;
extern u32 func_800E0E88(Obj *obj);
extern s32 func_800CF1C8(void *list, u8 kind);
extern const u16 D_80156A0E;

u32 func_800E96D4(Obj *self) {
    u32 amount = func_800E0E88(self);
    void *list = self->vtable24->get9C((u8 *)self + self->vtable24->delta98);
    if (list != 0) {
        amount += func_800CF1C8(list, 0x88) * D_80156A0E;
        if ((u16)amount >= 100) amount = 99;
    }
    return (u16)amount;
}
