#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 value; } Dir;
s32 func_800A3138(void*); s32 func_800A315C(void*);
void *func_800A33DC(void *out, void *rect);
s32 func_800B1E80(Pos*);
void *func_800A2594(void *out, void *from, Dir dir);
u32 func_800B1C6C(void *pos);
void func_800B1B58(Pos*, unsigned short);
void func_800B1BE0(Pos*, s32);
static inline s32 isValidDir(s32 i) { return i < 8; }
static inline s32 counterValue(s32 n) { return n; }
void func_800BD428(void *unused, void *map){
    u8 count;
    u8 tries;
    u8 placed;
    s32 i;
    s32 ok;
    Pos pos;
    Pos next;
    struct { Dir dir; u8 end[0]; } local;
    count = func_800A3138(map) * func_800A315C(map) / 4;
    tries = count * 10;
    placed = 0;
    while (counterValue(--tries) != 0xFF) {
        func_800A33DC(&pos, map);
        if (func_800B1E80(&pos)) continue;
        ok = 1;
        for (i = 0; isValidDir(i); i++) {
            local.dir.value = i & 7;
            func_800A2594(&next, &pos, local.dir);
            if (func_800B1C6C(&next) & 0x4000) { ok = 0; break; }
        }
        if (!ok) continue;
        func_800B1B58(&pos, 0x4000);
        func_800B1BE0(&pos, 0x200);
        if (++placed == count) break;
    }
}
