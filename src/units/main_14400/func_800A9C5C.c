#include "common.h"
typedef short s16;
extern void func_800A9C8C(unsigned char kind, unsigned char variant);
extern s16 D_80142B14, D_80142B16;
/* unused: the sole caller func_800A9608 supplies a0 = 0 (0x800A9660, jal delay slot); this body
 * does not read it. */
void func_800A9C5C(s32 unused) { func_800A9C8C(0x14, 0); D_80142B14 = 0; D_80142B16 = 0; }
