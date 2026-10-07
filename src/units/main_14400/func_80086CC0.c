#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef union {
    s32 word;
    struct {
        u16 x;
        u16 y;
    } p;
} Pos;
typedef struct {
    u8 pad0[0xC];
    s16 unkC;
    s16 unkE;
    s16 unk10;
} Sprite;
typedef struct {
    u8 pad0[4];
    s16 state;
    u8 pad6[0xC];
    u16 flags;
    s32 unk14;
} Task;
Sprite *func_8007946C(s32, s32);
void func_8004194C(s32 id, void *outY, void *outX);
void func_80061820(s32 x, s32 y);
void func_80061908(Pos, Pos);
void func_8005A234(void);
s32 func_80062554(s32 x, s32 y);

void func_80086CC0(Task *task)
{
    Sprite *sprite = func_8007946C(0, task->unk14);
    Pos a;
    Pos b;

    func_8004194C(task->unk14, &a, &b);
    if (task->flags & 2) {
        func_80061820(a.word, b.word);
        func_80061908(a, b);
        func_8005A234();
    }
    {
        Pos first = a;
        Pos second;

        sprite->unkC = (a.p.y << 7) + 0x40;
        second = b;
        sprite->unk10 = (b.p.y << 7) + 0x40;
        sprite->unkE = func_80062554(first.word, second.word) << 2;
    }
    task->state = 4;
}
