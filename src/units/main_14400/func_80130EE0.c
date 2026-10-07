#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    s32 result;
    s32 enabled;
} Obj;

typedef struct {
    Obj *obj;
    u8 *name;
    u8 *code;
    s32 arg4;
    s32 arg3;
} Request;

extern void *func_800262C0(const void *src, void *dst, s32 len);
extern void func_800265E0(u8 *buf, s32 len);
extern s32 func_80131F04(s32 kind, Request *req);

void func_80130EE0(Obj *obj, const void *src, u8 *code, s32 arg3, s32 arg4) {
    Request req;
    u8 name[0x10];
    u8 buf[4];

    if (obj->enabled != 0) {
        func_800262C0(src, name, 0x10);
        func_800265E0(buf, 4);
        buf[0] = *code;
        req.obj = obj;
        req.name = name;
        req.code = buf;
        req.arg4 = arg4;
        req.arg3 = arg3;
        obj->result = func_80131F04(0x203, &req);
    } else {
        obj->result = 1;
    }
}
