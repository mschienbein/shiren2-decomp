#include "common.h"

typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
/* Complete 0xC-byte sound state at D_80161644 (type +0, playback.handle +4,
 * playback.position +8/+9), the same layout as D_80161650 (func_800528BC
 * clears both records from one base each; func_80051E9C copies D_8016165C whole). */
typedef struct { short type; Playback playback; } State;
extern State D_80161644;
extern short D_8016166A;
extern unsigned char D_8016166D;
extern s32 func_80052D08(short id);
extern void func_80051D14(short id);
void func_800522FC(short a) {
    short *cur = &D_80161644.type;
    if (func_80052D08(*cur) == 0) D_8016166A = *cur;
    func_80051D14(a);
    D_8016166D = 1;
}
