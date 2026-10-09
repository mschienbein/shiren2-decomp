#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 type; void *sender; s32 pad8; s32 padC; s32 kind; u8 *data; } Msg;
typedef struct { u8 pad[8]; u8 f_8; } Sender;
typedef struct { s16 delta; s16 index; s32 (*fn)(void *self, Msg *msg); } VEntry;
typedef struct { u8 pad[0x24]; VEntry *vt; } Obj;
Obj *func_800B4928(void *position);
static inline void msg_init(Msg *m, s32 type, void *sender, s32 kind, u8 *data) {
    m->type = type;
    m->kind = kind;
    m->sender = sender;
    m->data = data;
}
/* The caller supplies its actor in a0; this message uses only sender and position. */
s32 func_8010F838(void *unused, Sender *sender, void *position) {
    Obj *obj = func_800B4928(position);
    Msg msg;
    u8 data[1];
    data[0] = sender->f_8;
    msg_init(&msg, 0x13, sender, 3, data);
    return obj->vt[11].fn((u8 *)obj + obj->vt[11].delta, &msg);
}
