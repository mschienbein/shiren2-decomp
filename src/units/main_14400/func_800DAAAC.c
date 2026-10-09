#include "common.h"

typedef unsigned char u8;
typedef struct Ent Ent;

typedef struct {
    s32 x;
    s32 y;
} Pair;

/* 0x20-byte actor message; kind 0 carries the sending unit at +4. */
typedef struct {
    s32 kind;
    void *source;
    u8 pad8[0x4];
    u8 code;
    u8 padD[0x3];
    Pair pos;
    u8 pad18[0x8];
} Msg;

typedef struct {
    u8 pad0[0x60];
    short delta;
    short index;
    s32 (*fn)(void *self, Ent *ent, s32 flag);
} OwnerVTable;

typedef struct {
    s32 field_0;
    OwnerVTable *vt;
} Owner;

typedef struct {
    u8 pad0[0x38];
    short delta;
    short index;
    s32 (*fn)(void *self, Msg *msg);
} EntVTable;

struct Ent {
    u8 pad0[0x8];
    EntVTable *vt;
};

typedef struct {
    Owner *owner;
    Ent *ent;
} Request;

typedef struct {
    u8 pad0[0x8];
    Request req;
} Obj;

extern void *D_801476B8;
void func_800DAD20(void *obj, void *target);
s32 func_800CD2BC(Owner *object, void *element);
void func_800DAD80(Obj *obj);

s32 func_800DAAAC(void *p, s32 kind)
{
    Obj *obj = p;
    Request *req = &obj->req;
    Ent *ent;
    Msg msg;
    Msg *m;

    if ((req->owner->vt->fn((u8 *)req->owner + req->owner->vt->delta, req->ent, 1) ^ 1) != 0) {
        return 0;
    }
    ent = req->ent;
    func_800DAD20(obj, ent);
    m = &msg;
    msg.kind = kind;
    m->source = D_801476B8;
    if (ent->vt->fn((u8 *)ent + ent->vt->delta, m)) {
        func_800CD2BC(obj->req.owner, ent);
    }
    func_800DAD80(obj);
    return 0;
}
