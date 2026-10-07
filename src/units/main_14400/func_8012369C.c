#include "common.h"

typedef struct { short offset; short pad; void *fn; } VEntry;
typedef struct { char pad[8]; VEntry *vtbl; } Self;
typedef struct { s32 type; void *f4; void *f8; s32 fC; s32 f10[2]; s32 f18; } Msg;
void func_801237B8(void *owner, void *obj, void *context, s32 force);
void func_80123894(void *owner, void *obj, void *context);
void func_800D3650(Self*);
s32 func_80123658(Self*);
s32 func_8010C96C(Self*, Msg*);
static inline s32 Msg_type(Msg *m) { return m->type; }
s32 func_8012369C(Self *self, Msg *msg){
    switch (Msg_type(msg)) {
    case 0x12:
        func_801237B8(self, msg->f4, msg->f8, 0);
        func_800D3650(self);
        if (self) ((void (*)(void*, s32))self->vtbl[1].fn)((char*)self + self->vtbl[1].offset, 3);
        return 1;
    case 0x13:
        func_801237B8(self, msg->f4, msg->f8, msg->f18 != 0);
        return 1;
    case 0xE:
        func_80123894(self, msg->f4, msg->f10);
        func_800D3650(self);
        if (self) ((void (*)(void*, s32))self->vtbl[1].fn)((char*)self + self->vtbl[1].offset, 3);
        return 1;
    case 0x1A:
        func_80123658(self);
        return 1;
    default:
        return func_8010C96C(self, msg);
    }
}
