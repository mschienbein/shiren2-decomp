#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 b; s32 a; } Key80042214;
typedef struct { u8 pad0; u8 kind; u8 pad2[0xE]; u8 unk10; } Entry80042214;
Entry80042214 *func_800B4D80(Key80042214 *key);
s32 func_80042214(s32 a, s32 b) {
    Key80042214 key;
    Entry80042214 *entry;
    key.a = a;
    key.b = b;
    entry = func_800B4D80(&key);
    if (entry != 0 && entry->kind == 0xD5) {
        return entry->unk10 != 0;
    }
    return 0;
}
