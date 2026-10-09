#include "common.h"
typedef unsigned char u8;
typedef struct ShirenDirection { signed char value; } ShirenDirection;
extern void func_800C27D0(void *iterator, void *origin, ShirenDirection direction, unsigned char limit, unsigned char mode);
extern void func_800C2B84(void *iterator);
void *func_800C2B40(void *iterator, void *origin, u8 *direction, s32 count) {
    ShirenDirection dir;
    dir.value = *direction;
    func_800C27D0(iterator, origin, dir, (unsigned char)(count + 1), 0);
    func_800C2B84(iterator);
    return iterator;
}
