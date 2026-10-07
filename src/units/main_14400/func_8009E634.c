#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    s32 column;
    s32 row;
} Cursor;

typedef struct {
    s16 delta;
    s16 pad2;
    void (*func)(void *, Cursor *);
} VtblEntry;

typedef struct {
    u8 pad0[0x24];
    s32 rowCount;
    u8 pad28[0xC];
    Cursor cursor;
    u8 pad3C[0x10];
    VtblEntry *vtbl;
} Menu;

void func_80045A24(s32);

void func_8009E634(Menu *menu, s32 dir) {
    Cursor cursor = menu->cursor;

    switch (dir) {
    case 2:
        if (cursor.row < menu->rowCount - 1) {
            cursor.row++;
        } else {
            cursor.row = 0;
        }
        break;
    case 3:
        if (cursor.row != 0) {
            cursor.row--;
        } else {
            cursor.row = menu->rowCount - 1;
        }
        break;
    }
    if (cursor.row != menu->cursor.row) {
        menu->vtbl[16].func((u8 *)menu + menu->vtbl[16].delta, &cursor);
        func_80045A24(2);
    }
}
