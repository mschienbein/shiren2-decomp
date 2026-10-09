#include "common.h"
typedef struct { s32 column, row; } Position;
typedef struct { unsigned char pad00[0x80]; short adjustment; unsigned short pad82; void (*select)(void *, Position *); } Methods;
typedef struct {
    unsigned char pad00[0x24]; s32 rows; unsigned char pad28[0xC]; Position position;
    unsigned char pad3C[0x10]; Methods *methods; unsigned char columns, field51, field52, count;
} Menu;
extern void func_80045A24(s32 sound);
void func_8009D9E0(Menu *menu, s32 direction) {
    Position position = menu->position;
    s32 lastColumn = (menu->count - 1) % menu->columns;
    s32 columnLimit = menu->columns - 1;
    switch (direction) {
    case 0:
        if (menu->position.row == menu->rows - 1) columnLimit = lastColumn;
        if (position.column < columnLimit) position.column++;
        else position.column = 0;
        break;
    case 1:
        if (menu->position.row == menu->rows - 1) columnLimit = lastColumn;
        if (position.column > 0) position.column--;
        else position.column = columnLimit;
        break;
    case 2:
        if (position.row < menu->rows - 1) {
            position.row++;
            if (position.row == menu->rows - 1 && lastColumn < position.column) position.column = lastColumn;
        } else position.row = 0;
        break;
    case 3:
        if (position.row == 0) {
            position.row = menu->rows - 1;
            if (lastColumn < position.column) position.column = lastColumn;
        } else position.row--;
        break;
    }
    if (position.row != menu->position.row || position.column != menu->position.column) {
        menu->methods->select((unsigned char *)menu + menu->methods->adjustment, &position);
        func_80045A24(2);
    }
}
