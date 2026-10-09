#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pair;

typedef struct {
    s32 kind;
    u8 pad4[0x8];
    u8 code;
    u8 padD[0x3];
    Pair pos;
    u8 pad18[0x8];
} Msg;

/* Actor table at +8, entry +0x38/+0x3C: message handler s32 (void *receiver, void *event)
 * (decided contract); only this entry is used here and its result is not needed. */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *receiver, void *event);
} MessageEntry;

typedef struct {
    u8 pad0[0x38];
    MessageEntry message_38;
} VTable;

typedef struct {
    u8 type;
    u8 pad1[0x7];
    VTable *vtable;
} Actor;

typedef struct {
    u8 pad0[0x10];
    void *handle;
} Obj;

extern Actor *func_800B4D80(Pair *pos);
extern s32 func_800ADB64(void *handle, void *arg, Pair *pos, s32 flag);

void func_800C4CA8(Obj *obj, void *arg, u8 *code, Pair *pos) {
    Actor *actor = func_800B4D80(pos);
    Msg msg;
    Msg *m;

    if (actor != 0 && actor->type == 0x10) {
        msg.kind = 0x15;
        msg.pos = *pos;
        m = &msg;
        m->code = *code;
        actor->vtable->message_38.func((u8 *)actor + actor->vtable->message_38.delta, m);
    }
    func_800ADB64(obj->handle, arg, pos, 1);
}
