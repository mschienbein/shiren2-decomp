#include "common.h"

/* Item picker: item kinds listed at 0x74 (count at 0x90), shown in rows of five. */
typedef struct {
    char pad0[0x34];
    s32 col_34;
    s32 row_38;
    char pad3C[0x74 - 0x3C];
    unsigned char items_74[0x1C];
    s32 count_90;
} Picker_8009E088;

unsigned char func_8009E088(Picker_8009E088 *picker) {
    return picker->items_74[picker->row_38 * 5 + picker->col_34];
}
