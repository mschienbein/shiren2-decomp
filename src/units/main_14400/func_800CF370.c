#include "common.h"
#include "collection_iterator.h"
typedef unsigned char u8;
typedef struct { u8 kind; u8 field_01; u8 flags_02; } Entry;
/* CEBA0 stores the selected entry at +0xC; keep the entire 16-byte iterator. */
/* Original success path returns the entry pointer from func_800CEC68. */
void *func_800CF370(void *container) {
    CollectionIterator it;
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
