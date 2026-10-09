#include "common.h"

typedef unsigned char u8;

s32 func_80053CC8(const char *text, s32 lines)
{
    s32 count = 0;
    u8 c;
    while ((c = *text) != 0) {
        if ((c & 0xF0) == 0xF0) {
            ++count;
            ++text;
        } else if (c == 0xA) {
            if (--lines == 0)
                break;
        }
        ++text;
        ++count;
    }
    return count;
}
