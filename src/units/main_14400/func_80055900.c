#include "common.h"
typedef struct { s32 x, y; } Pair;
extern s32 D_80139B18;
extern s32 D_801399B4;
extern s32 D_801399B0;
extern Pair D_801399B8[];
extern const char D_8014C0DC[];
extern s32 func_800327C0(char *dst, const char *fmt, ...);
extern s32 func_80055B28(s32 a, s32 kind, s32 b, s32 c, s32 d);

static inline s32 digit_x(s32 index) {
    return index * 16 - 34;
}

void func_80055900(void) {
    char text[9];
    s32 i;
    if (D_80139B18 != 0) {
        if (D_801399B4 != 0) {
            func_800327C0(text, D_8014C0DC, D_801399B4);
            i = 6;
            do {
                s32 x = digit_x(i);
                if (text[i] != ' ') {
                    func_80055B28(0x133, text[i] - '0', x - D_801399B8[D_801399B0].x + 8, 0x5A, 0);
                }
                i++;
            } while (i < 8);
        }
        if (D_801399B4 != 0) {
            func_80055B28(0x133, D_801399B0 + 10, 0x6E, 0x5A, 0);
        } else {
            func_80055B28(0x133, D_801399B0 + 10, 0x5E, 0x5A, 0);
        }
    }
}
