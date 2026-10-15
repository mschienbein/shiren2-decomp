#include "func_801E81F4_13A114/types.h"

extern s32 func_8005E708(u32 value);

s32 func_801E81F4_13A114(s32 n, u8 *s) {
    s32 i;
    s32 w = 0;
    u8 c;

    for (i = 0; i < n; i++, s++) {
        c = *s;
        if (c == 0) {
            break;
        }
        if ((c & 0xF0) == 0xF0) {
            w++;
            s++;
        } else if (c == '@' || c == '#') {
            s++;
            w++;
            if (*s == '-') {
                w++;
                s++;
            }
            while (func_8005E708(*s)) {
                w++;
                s++;
            }
        }
        w++;
    }
    return w;
}
