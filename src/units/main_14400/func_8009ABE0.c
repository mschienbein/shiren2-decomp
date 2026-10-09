#include "common.h"

typedef struct Unit Unit;

/* Actor inventory list at actor+0xCC (vtable D_80154438, installed at 0x800CECC4):
 * slot +0x3C is func_800CE7A0(list, u32 index), returning the entry pointer. */
typedef struct LookupTable {
    unsigned char pad_00[0x38];
    short adjust_38;
    short field_3A;
    Unit *(*method_3C)(void *, u32);
} LookupTable;

typedef struct Lookup {
    void *pool_00;
    LookupTable *table_04;
} Lookup;

/* Actor vtable D_80159008: slot +0x9C is func_800EE07C (returns actor + 0xCC). */
typedef struct {
    unsigned char pad_00[0x98];
    short adjust_98;
    short field_9A;
    Lookup *(*method_9C)(void *);
} GlobalTable;

typedef struct {
    unsigned char pad_00[0x24];
    GlobalTable *table_24;
} Global;

/* Panel (vtable D_80152620): count_20 is the column count (2, from D_80139074 via
 * func_8009543C); func_8009A9D0 stores at most two list indices at +0x50 and their count at +0x58. */
typedef struct {
    unsigned char pad_00[0x20];
    s32 count_20;
    unsigned char pad_24[0x2C];
    u32 ids_50[2];
    s32 count_58;
} Panel;

typedef struct {
    s32 row;
    s32 column;
    char text[128];
} RenderBuffer;

extern Global *D_801476B8;
extern char *func_800514F0(Unit *unit, char *output, s32 mode, s32 scale, s32 offset, s32 kind, s32 flags);
extern void func_80048870(Panel *, s32);
extern void func_800487EC(Panel *, s32, s32, void *);

void func_8009ABE0(Panel *panel) {
    RenderBuffer buffer;
    char *text = buffer.text;
    GlobalTable *table = D_801476B8->table_24;
    Lookup *lookup = table->method_9C((unsigned char *)D_801476B8 + table->adjust_98);
    buffer.column = 0;
    buffer.row = 0;
    for (;;) {
        LookupTable *methods;
        if (buffer.row >= panel->count_20) {
            break;
        }
        methods = lookup->table_04;
        func_800514F0(methods->method_3C((unsigned char *)lookup + methods->adjust_38,
                                      panel->ids_50[buffer.row]), text, 7, 0, -1, 0, 0);
        func_80048870(panel, 0x78000000);
        func_800487EC(panel, buffer.row, 1, text);
        ++buffer.row;
    }
}
