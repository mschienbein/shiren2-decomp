#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Cursor8009C54C;

typedef struct Menu8009C54C Menu8009C54C;

typedef struct {
    s16 delta;
    s16 index;
    void (*func)(Menu8009C54C *self, Cursor8009C54C *cursor);
} VtEntry8009C54C;

struct Menu8009C54C {
    u8 pad0[0x34];
    Cursor8009C54C cursor;
    u8 pad3C[0x4C - 0x3C];
    VtEntry8009C54C *vtable;
    u8 pad50[4];
    s32 count;
};

void func_80045A24(s32 sound);

void func_8009C54C(Menu8009C54C *self, s32 dir) {
    Cursor8009C54C old;
    Cursor8009C54C cur;

    old = self->cursor;
    cur = self->cursor;
    switch (dir) {
    case 0:
        if (cur.x < (self->count - 1) * 4) {
            cur.x += 4;
        } else {
            cur.x = 0;
        }
        self->vtable[16].func((Menu8009C54C *)((u8 *)self + self->vtable[16].delta), &cur);
        break;
    case 1:
        if (cur.x > 0) {
            cur.x -= 4;
        } else {
            cur.x = (self->count - 1) * 4;
        }
        self->vtable[16].func((Menu8009C54C *)((u8 *)self + self->vtable[16].delta), &cur);
        break;
    }
    if (cur.y != old.y || cur.x != old.x) {
        func_80045A24(2);
    }
}
