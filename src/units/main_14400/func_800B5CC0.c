#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[4]; void *key; u8 pad8[0x10]; } Entry;
extern Entry D_80143330[];
extern u8 D_80143391;
extern s32 D_80143444;
extern u8 D_80143448;

static inline void *entry_key(Entry *e) { return e->key; }

Entry *func_800B5CC0(void *key) {
    Entry *table;
    s32 i;
    s32 count;
    s32 useFirst;

    if (key == 0 || D_80143444 == 0) {
        return 0;
    }
    i = 0;
    table = D_80143330;
    useFirst = 0;
    if (D_80143391 & 4) {
        useFirst = 1;
    }
    count = D_80143448;
    for (;;) {
        void *entryKey;
        if (i >= count) {
            break;
        }
        entryKey = entry_key(&table[i]);
        if (entryKey == 0) {
            return useFirst ? table : 0;
        }
        if (entryKey == key) {
            return &table[i];
        }
        i++;
    }
    return 0;
}
