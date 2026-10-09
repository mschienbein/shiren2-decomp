#include "common.h"
extern unsigned char D_80139638[0x50];
extern unsigned char D_80139610;
extern s32 func_8004929C(void);
s32 func_800491C0(void *source) {
    unsigned char *cursor = source;
    s32 length = func_8004929C();
    s32 lines = 1;
    unsigned char *destination = D_80139638 + length;
    while (*cursor) {
        s32 width = 1;
        if ((*cursor & 0xF0) == 0xF0) width = 2;
        length += width;
        if (length >= 0x50) return 0;
        if (width == 2) { *destination++ = *cursor++; *destination++ = *cursor++; }
        else if (*cursor == 0xA) { cursor++; *destination++ = 0; lines++; }
        else *destination++ = *cursor++;
    }
    *destination = 0;
    D_80139610 += lines;
    return 1;
}
