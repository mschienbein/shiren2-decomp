#include "common.h"

typedef unsigned short u16;
typedef struct { char pad0[0xC]; s32 unkC; } Child;
typedef struct { char pad0[0x6C]; u16 unk6C; char pad6E[2]; s32 unk70; Child unk74; } Obj;
void func_80048764(Child *);
void func_80048870(Child *, s32);
char *func_80048480(u16 id);
s32 func_800327C0(char *dst, const char *fmt, ...);
void func_800487EC(Child *, s32, s32, char *);
void func_8009F44C(Obj *obj) {
    char buf[0x80];
    func_80048764(&obj->unk74);
    func_80048870(&obj->unk74, 0x78000000);
    func_800327C0(buf, func_80048480(obj->unk6C), obj->unk70);
    func_800487EC(&obj->unk74, 0, 0, buf);
}
