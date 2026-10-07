#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s32 x0; s32 x4; } Entry;
typedef struct { s32 x0; u32 count; Entry *entries; } Table;
extern Entry *func_80091450(u32);
extern u8 *func_8006A810(void *, s32, s32);
extern s32 func_8008DF04(void *);
extern void func_80091544(void *);
s32 func_8008D7E0(void *src, Table *t){
    s32 err = 0;
    s32 total = 0;
    u32 i;
    if (t->count != 0) {
        s32 size = t->count * 8;
        t->entries = func_80091450(size);
        if (t->entries == 0) {
            err = -1;
        } else {
            func_8006A810(t->entries, 0, size);
            for (i = 0; i < t->count; i++) {
                t->entries[i].x0 = func_8008DF04(src);
                total += 4;
            }
        }
    }
    if (err) {
        total = -1;
        if (t->entries) {
            func_80091544(t->entries);
            t->entries = 0;
        }
    }
    return total;
}
