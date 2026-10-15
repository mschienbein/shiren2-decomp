#include "func_801E52EC_13720C/types.h"
void func_8006AAF0(void *dst, u32 devAddr, s32 size);
extern u8 D_001C6CB0[];

s32 func_801E52EC_13720C(void) {
    u8 *buf = D_801F6550_148470;
    u32 addr;
    u32 aligned;
    s32 offset;
    s32 base;

    addr = (u32)D_001C6CB0;
    addr += D_801F6710_148630->x2 * 4;
    offset = addr & ~7;
    aligned = offset;
    offset = addr - offset;
    base = D_801F6710_148630->x4 * 384;
    func_8006AAF0(buf, aligned, 32);
    addr = *(u32 *)(buf + offset) + (u32)D_001C6CB0;
    addr += base;
    offset = addr & ~7;
    aligned = offset;
    offset = addr - offset;
    func_8006AAF0(buf, aligned, 400);
    return offset;
}
