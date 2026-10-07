#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern u8 D_80161B44;
extern char D_8014C0C4[];
extern u8 D_801398B1;
void func_80054524(char *fmt, ...);
void func_8005457C(u8 *src) {
    /* The NUL path leaves j == 0x101 before the optional newline store. */
    u8 buf[0x102];
    s32 i;
    s32 j;
    i = 0;
    while (i < 0x100) {
        for (j = i; j < 0x100; j++) {
            buf[j] = src[j];
            if ((src[j] & 0xF0) == 0xF0) {
                j++;
                buf[j] = src[j];
                continue;
            }
            if (src[j] == '\n') {
                buf[j] = 0;
                break;
            }
            if (src[j] == 0) j = 0x100;
        }
        if (j != 0) {
            if (D_80161B44 == 0) {
                func_80054524(D_8014C0C4, &buf[i]);
            } else {
                func_80054524(D_8014C0C4, &buf[i != 0 ? i - 1 : 0]);
            }
        }
        if (D_80161B44 != 0) buf[j] = '\n';
        i = j + 1;
    }
    D_801398B1 = 0;
}
