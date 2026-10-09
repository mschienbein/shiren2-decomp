#include "common.h"
extern void func_80131168(const unsigned char *, unsigned char *, u32);
void func_80058D8C(const unsigned char *source, unsigned char *dest, u32 count) {
    func_80131168(source, dest, count);
}
