#include "common.h"

typedef struct {
    s32 field_0[9];
    s32 count;
    s32 pad28[3];
    s32 dirty;
    s32 cursor;
} Menu;

void func_80045A24(s32 sound);

void func_8009F950(Menu *menu, s32 input)
{
    s32 cursor = menu->cursor;

    switch (input) {
    case 2:
        if (cursor < menu->count - 1) {
            cursor++;
        } else {
            cursor = 0;
        }
        break;
    case 3:
        if (cursor != 0) {
            cursor--;
        } else {
            cursor = menu->count - 1;
        }
        break;
    }
    if (cursor != menu->cursor) {
        func_80045A24(2);
    }
    menu->cursor = cursor;
    menu->dirty = 0;
}
