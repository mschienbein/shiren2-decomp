#include "common.h"

typedef unsigned char u8;

/* 12-byte menu entry: text id, argument, availability kind. */
typedef struct { s32 id; s32 arg; s32 kind; } Entry;

/* Save menu (see func_8009CA80): available entries at +0x5C, counts of save and empty
 * slots at +0x168/+0x16C. */
typedef struct {
    u8 pad_00[0x5C];
    Entry entries[9];
    s32 count;
    u8 pad_CC[0x9C];
    s32 saves_168;
    s32 empties_16C;
} Menu;

/* The whole nine-entry table (0x80141D30..0x80141D9B). */
extern Entry D_80141D30[9];
extern s32 D_801E7894;
extern s32 func_800CC8A4(u8 id);

/* Collects the menu entries whose availability kind is satisfied. */
void func_8009CC34(Menu *menu)
{
    s32 i;

    menu->count = 0;
    for (i = 0; ; i++) {
        s32 include;

        if (i >= 9) break;
        include = 1;
        switch (D_80141D30[i].kind) {
        case 0:
        case 3:
        case 5:
            if (menu->saves_168 <= 0) include = 0;
            break;
        case 1:
            if (menu->empties_16C <= 0) include = 0;
            break;
        case 2:
            if (menu->saves_168 <= 0 || menu->empties_16C <= 0) include = 0;
            break;
        case 8:
            if (~D_801E7894 == 0) include = 0;  /* all bits set: never started */
            if (D_801E7894 <= 0 && menu->saves_168 <= 0) include = 0;
            break;
        case 4: {
            s32 found = 0;
            s32 j;

            for (j = 0; j < 6; j++) {
                if (func_800CC8A4(j)) found++;
            }
            if (found == 0) include = 0;
            break;
        }
        case 6:
        case 7:
            break;
        }
        if (include) menu->entries[menu->count++] = D_80141D30[i];
    }
}
