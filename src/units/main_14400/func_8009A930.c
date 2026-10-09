#include "common.h"

typedef unsigned char u8;

typedef struct { s32 x; s32 y; } Pair;
typedef struct { s32 columns; s32 width; } LayoutSize;
/* Menu layout record consumed by func_80097240 (see func_80096140). */
typedef struct {
    LayoutSize size;
    Pair origin;
} Layout;
typedef struct { s32 count; s32 width; } Selection;
typedef struct Menu Menu;
typedef struct MenuItem MenuItem;

extern LayoutSize D_80138E78;  /* {3, 10} */
extern MenuItem D_80141AA0[];
extern Selection D_80138E80;
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
extern void func_80097240(Menu *menu, MenuItem *items, Layout *layout, Selection *sel);

/* Opens the menu at origin with the default 3-column layout; func_80097240 receives
 * its own copy of the layout. */
void func_8009A930(Menu *menu, Pair *origin)
{
    Layout copy;
    Layout layout;

    func_8006A810((u8 *)&layout, 0, sizeof(layout));
    layout.size.columns = D_80138E78.columns;
    layout.size.width = D_80138E78.width;
    layout.origin.x = origin->x;
    layout.origin.y = origin->y;
    copy = layout;
    func_80097240(menu, D_80141AA0, &copy, &D_80138E80);
}
