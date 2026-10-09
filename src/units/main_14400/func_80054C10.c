#include "common.h"
typedef unsigned int size_t;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
extern u16 D_80162B48;
extern s16 D_80162B4A[0x1CC];
extern u8 D_80162EE2[0x1CC];
extern char D_80161B45[0x1001];
extern size_t func_80032D70(const char *s);
extern char *func_80083C90(char *dst, char *src);
void func_80054C10(u8 kind, char *text) {
    s32 offset;
    char *savedText = text;
    s32 length = func_80032D70(savedText);
    offset = D_80162B4A[D_80162B48];
    if (length + offset + 1 > 0x1000) {
        D_80162B4A[D_80162B48] = 0;
        offset = 0;
        func_80083C90(D_80161B45, savedText);
    } else {
        func_80083C90(offset + D_80161B45, savedText);
    }
    offset = length + offset + 1;
    D_80161B45[offset] = 0;
    D_80162EE2[D_80162B48] = kind;
    if (++D_80162B48 == 0x1CC) D_80162B48 = 0;
    D_80162B4A[D_80162B48] = offset;
}
