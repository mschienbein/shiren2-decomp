#include "common.h"

/* 12-byte menu entry: two payload words, then the availability kind tested here. */
typedef struct { s32 id; s32 arg; s32 kind; } Entry;
typedef struct { unsigned char pad_000[0x3B0]; Entry entries[3]; } Object;

/* The whole three-entry table (0x80142458..0x8014247B). */
extern Entry D_80142458[3];

/* Copies the entries available for (first, second) into obj->entries; returns how many. */
s32 func_8009D198(Object *obj, s32 first, s32 second)
{
    s32 count = 0;
    s32 i;

    for (i = 0; ; i++) {
        s32 include;

        if (i >= 3) break;
        include = 1;
        switch (D_80142458[i].kind) {
        case 1:
            if (second == 0) include = 0;
            if (first >= 3) include = 0;
            break;
        case 2:
            if (first >= 3 && second >= 2) include = 0;
        case 3:
            if (first == 0) include = 0;
            break;
        }
        if (include) obj->entries[count++] = D_80142458[i];
    }
    return count;
}
