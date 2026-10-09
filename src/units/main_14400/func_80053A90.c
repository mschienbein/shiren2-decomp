#include "common.h"
typedef struct { s32 field_00; unsigned char field_04[0x41c]; } Entry;
extern Entry D_80161724[1];
extern s32 func_800827BC(void);
Entry *func_80053A90(void) {
    Entry *entry = D_80161724 + 1;
    s32 key = func_800827BC();
    s32 i;
    for (i = 0; i != -1; --i) {
        --entry;
        if (key == entry->field_00) return entry;
    }
    return 0;
}
