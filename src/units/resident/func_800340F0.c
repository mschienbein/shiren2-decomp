#include "common.h"

u32 func_8002FDE0(void *);

u32 func_800340F0(void *addr) {
    if ((u32)addr - 0x80000000 < 0x20000000) {
        return (u32)addr & 0x1FFFFFFF;
    } else if ((u32)addr - 0xA0000000 < 0x20000000) {
        return (u32)addr & 0x1FFFFFFF;
    } else {
        return func_8002FDE0(addr);
    }
}
