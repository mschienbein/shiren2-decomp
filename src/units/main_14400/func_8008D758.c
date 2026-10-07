#include "common.h"
typedef struct { char pad[0xC]; s32 unkC; char pad10[4]; } Entry;
typedef struct { unsigned char count; char pad[3]; Entry *entries; } List;
unsigned char func_8008D758(List *l, unsigned char idx) {
    s32 i;
    if (idx >= l->count || l->entries[idx].unkC == 0) {
        for (i = 0; i < l->count; i++) {
            if ((unsigned char)i < l->count && l->entries[i].unkC != 0) { idx = i; break; }
        }
    }
    return idx;
}
