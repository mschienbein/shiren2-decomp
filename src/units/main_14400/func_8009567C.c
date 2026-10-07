#include "common.h"
typedef short s16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { s16 delta; s16 index; void (*fn)(void *, Pos *); } VEntry;
typedef struct {
    char pad0[0x20];
    s32 width;
    s32 height;
    char pad28[0xC];
    Pos cursor;
    char pad3C[0x10];
    VEntry *vtbl;
} Menu;
void func_80045A24(s32);
void func_8009567C(Menu *menu, s32 dir) {
    Pos old = menu->cursor;
    Pos cur = menu->cursor;
    switch (dir) {
        case 0:
            if (cur.x < menu->width - 1) {
                cur.x++;
            } else {
                cur.x = 0;
            }
            break;
        case 1:
            if (cur.x > 0) {
                cur.x--;
            } else {
                cur.x = menu->width - 1;
            }
            break;
        case 2:
            if (cur.y < menu->height - 1) {
                cur.y++;
            } else {
                cur.y = 0;
            }
            break;
        case 3:
            if (cur.y > 0) {
                cur.y--;
            } else {
                cur.y = menu->height - 1;
            }
            break;
        default:
            goto done;
    }
    menu->vtbl[16].fn((char *)menu + menu->vtbl[16].delta, &cur);
done:
    if (cur.y != old.y || cur.x != old.x) {
        func_80045A24(2);
    }
}
