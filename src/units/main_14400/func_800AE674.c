#include "common.h"
#include "item_records.h"
extern s32 D_80143040;
/* One 0x80-byte object: four 0x20-byte name buffers used in rotation. */
extern char D_801C54E2[4][0x20];
char *func_800AE674(Item *arg0) {
    ItemMethods *dispatch = arg0->field08;
    s32 index = (D_80143040 + 1) & 3;
    D_80143040 = index;
    return dispatch->format34((Item *)((char *)arg0 + dispatch->delta30), D_801C54E2[index]);
}
