#include "common.h"
typedef unsigned char u8;
typedef struct { u8 kind; u8 field_01; u8 flags_02; } Entry;
/* CEBA0 stores the selected entry at +0xC; keep the entire 16-byte iterator. */
typedef struct { s32 index; void *container; s32 mode; Entry *current; } Iterator;
extern void *func_800CEB20(Iterator *it, void *container);
extern s32 func_800CEBA0(Iterator *it);
extern Entry *func_800CEC68(Iterator *it);
/* Original success path returns the entry pointer from func_800CEC68. */
void *func_800CF370(void *container) {
    Iterator it;
    Entry *entry;
    func_800CEB20(&it, container);
    while (func_800CEBA0(&it)) {
        entry = func_800CEC68(&it);
        if (entry->flags_02 & 4) {
            switch (entry->kind) {
            case 5: case 11: case 12: case 13: return entry;
            }
        }
    }
    return 0;
}
