#include "common.h"
void func_8006AA38(unsigned char *destination, unsigned char *source, u32 count) {
    if (count < 9 || ((u32)destination & 3) != ((u32)source & 3)) {
        while (count) {
            count--;
            *destination++ = *source++;
        }
        return;
    }
    while (count && ((u32)source & 3)) {
        count--;
        *destination++ = *source++;
    }
    while (count >= 4) {
        count -= 4;
        *(u32 *)destination = *(u32 *)source;
        source += 4;
        destination += 4;
    }
    while (count) {
        count--;
        *destination++ = *source++;
    }
}
