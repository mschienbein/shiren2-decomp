#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct { u8 pad0[0x90]; short delta90, pad92; s32 (*action94)(void *, s32, s32, u8, s32); } VTable;
typedef struct { Position position; u8 pad8[0x1C]; VTable *vtable; } Object;
extern u32 D_8013960C;
extern s32 func_800E1CC4(Object *object, s32 kind);
extern s32 func_800E4454(Object *object);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800E4470(Object *object);
extern char *func_800A3B20(Object *object);
extern void func_800497F0(s32 id, ...);
extern void func_80049C90(s32 a, s32 b);
static __inline__ void snapshot(Position *out, Position *in) { out->x = in->x; out->y = in->y; }
s32 func_800E44EC(Object *object) {
    Position position;
    s32 first = func_800E1CC4(object, 1);
    s32 second = func_800E4454(object);
    if (first || second) {
        s32 token, message;
        snapshot(&position, &object->position);
        token = func_80049CB4(0xDA, &position);
        D_8013960C <<= 1;
        if (first) {
            object->vtable->action94((char *)object + object->vtable->delta90, 1, 1, 0, 0);
            message = 0x1B4;
            func_80049CB4(0xD7, &position);
        } else {
            func_800E4470(object);
            message = 0x226;
        }
        D_8013960C >>= 1;
        func_800497F0(message, token, func_800A3B20(object));
        func_80049C90(1, token);
        return 1;
    }
    return 0;
}
