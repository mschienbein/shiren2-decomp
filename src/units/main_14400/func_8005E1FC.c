#include "common.h"

extern s32 func_8005E1B4(const signed char *text, s32 *position);

s32 func_8005E1FC(const signed char *text, s32 wanted) {
    const signed char *source = text;
    s32 cursor = 0;
    s32 value;
    for (;;) {
        value = func_8005E1B4(source, &cursor);
        if (value != 0) {
            if (value == wanted) {
                return cursor / 2;
            }
        } else {
            break;
        }
    }
    return 0;
}
