#include "common.h"
extern unsigned char func_800AE98C(unsigned char *item);
extern void func_8010BC98(unsigned char *item, unsigned char value);
extern const unsigned char D_80157560[32], D_80157580[32];
void func_8010CCD0(unsigned char *item) {
    s32 index = func_800AE98C(item);
    item[0xC] = D_80157560[index];
    func_8010BC98(item, D_80157580[index]);
}
