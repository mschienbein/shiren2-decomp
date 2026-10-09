#include "common.h"
extern char D_80147620[];
extern unsigned char func_800C57CC(void *, s32);
s32 func_800AB4B0(unsigned char *weights) {
    unsigned char *cursor = weights;
    s32 total = 0;
    s32 remaining;
    s32 choice;
    for (remaining = 4; remaining != -1; remaining--) total += *cursor++;
    choice = func_800C57CC(D_80147620, (total - 1) & 0xff) & 0xff;
    cursor = weights;
    for (remaining = 4; remaining != -1; remaining--, cursor++) {
        if (choice < *cursor) return (signed char)(cursor - weights) - 1;
        choice -= *cursor;
    }
    return 0;
}
