#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { f32 key; u8 locked; u8 pad5[3]; } Entry8008CE58;
typedef struct { u8 pad0[0x8]; u32 count; Entry8008CE58 *entries; } Table8008CE58;
typedef struct { u8 pad0[0xC]; Table8008CE58 *table; } Info8008CE58;
typedef struct { u8 pad0[0xC]; f32 key; u8 pad10[0x64]; Info8008CE58 *info; } Obj8008CE58;

s32 func_8008CE58(Obj8008CE58 *obj) {
    Table8008CE58 *table = obj->info->table;
    u8 locked = 1;
    if (table != 0) {
        Entry8008CE58 *entries = table->entries;
        u32 i;
        for (i = 0; i < table->count; i++) {
            if (obj->key == entries[i].key) {
                locked = entries[i].locked;
                break;
            }
        }
    }
    return locked == 0;
}
