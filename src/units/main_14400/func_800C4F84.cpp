#include "common.h"

typedef short s16;

struct Pos800C4F84 {
    s32 x;
    s32 y;
};

struct Obj800C4F84 {
    char pad0[0x10];
    s16 unk10;
};

struct Buffer800C4F84;

/* E552C writes amount at +2 (VRAM 0x800E5900/0x800E591C), not
 * merely flags. The caller initializes only flags at VRAM 0x800C5044. */
struct Outcome800C4F84 {
    unsigned short flags;
    unsigned short amount;
};

extern "C" {
u32 func_800B1C6C(void *);
s32 func_800A58B8(Pos800C4F84 *pos);
s32 func_80049CB4(s32, ...);
void func_80136910(Buffer800C4F84 *, void *, u32, u32, u32);
void func_800A7A9C(Pos800C4F84 *pos, Buffer800C4F84 *buf, Outcome800C4F84 *out);
}

/* 0x14-byte message built by func_80136910. */
struct Buffer800C4F84 {
    void *field_0;
    u32 field_4;
    u32 field_8;
    unsigned short field_C;
    unsigned short field_E;
    unsigned char field_10;
    Buffer800C4F84(void *source, u32 value, u32 kind, u32 flag) { func_80136910(this, source, value, kind, flag); }
};

/* arg2 is supplied by the command slot; unused in this implementation. */
extern "C" void func_800C4F84(Obj800C4F84 *obj, void *arg1, unsigned char *arg2, Pos800C4F84 *pos)
{
    Pos800C4F84 local;
    Pos800C4F84 *lp = &local;
    s32 special;

    special = 0;
    lp->x = pos->x;
    lp->y = pos->y;
    if (func_800B1C6C(lp) & 0x2000) {
        special = func_800A58B8(pos) == 1;
    }
    if (special) {
        func_80049CB4(0x10B, lp);
    } else {
        func_80049CB4(6);
        func_80049CB4(0x114, lp);
        func_80049CB4(7);
    }
    Buffer800C4F84 buf(arg1, obj->unk10, 2, 1);
    Outcome800C4F84 result;
    result.flags = 0;
    func_80049CB4(6);
    func_800A7A9C(pos, &buf, &result);
    func_80049CB4(7);
}
