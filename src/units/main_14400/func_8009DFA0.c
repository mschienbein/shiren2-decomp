#include "common.h"
typedef unsigned char u8;
typedef struct { u8 kind; u8 sub; u8 arg; u8 flag; } Cmd;
typedef struct { u8 pad0[0x74]; u8 list[0x1C]; s32 count; } Obj;
extern u8 D_8014AB47;
extern u8 D_80139000[];
s32 func_80112D60(u8 id);
u8 *func_8006A810(void *dst, s32 value, s32 size);
void func_8009D910(Obj *o, void *table, Cmd *cmd);
void func_8009DFA0(Obj *o) {
    u8 i;
    Cmd cmd;
    Cmd tmp;
    o->count = 0;
    for (i = 23; i < 50; i++) {
        if (i == 32) continue;
        if (func_80112D60(i)) {
            o->list[o->count++] = i;
        }
    }
    func_8006A810(&tmp, 0, 4);
    tmp.kind = 5;
    tmp.sub = 2;
    tmp.arg = D_8014AB47;
    tmp.flag = o->count;
    cmd = tmp;
    if (o->count == 0) cmd.flag = 1;
    func_8009D910(o, D_80139000, &cmd);
}
