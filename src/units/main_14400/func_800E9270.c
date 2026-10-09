#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct Entity { u8 pad0[0x1E]; u8 flags; } Entity;
typedef struct Actor Actor;
s32 func_800E1CC4(Entity *, s32);
void func_800498E4(s32 id, ...);
s32 func_80049CB4(s32 id, ...);
char *func_800A3B20(Entity *obj);
char *func_800AE674(void *obj);
void func_80049A04(u16 id, ...);
void func_800D3650(void *arg);
static inline s32 special(Entity *obj) { return (obj->flags >> 2) & 1; }
s32 func_800E9270(Entity *obj, Actor *actor, s32 message, s32 extra) {
    u16 msg = message;
    char *name;
    if (func_800E1CC4(obj, 2)) { func_800498E4(0x71); return 0; }
    func_80049CB4(0x1131);
    if (special(obj)) {
        func_80049CB4(6); func_80049CB4(0x1134, 0); func_80049CB4(7);
    }
    func_80049CB4(6);
    name = func_800A3B20(obj);
    func_80049A04(msg, name, func_800AE674(actor));
    func_80049CB4(7);
    func_80049CB4(0x102F, obj, actor);
    if (extra) func_80049CB4(0x12A);
    if (special(obj)) func_80049CB4(0x136);
    func_800D3650(actor);
    return 1;
}
