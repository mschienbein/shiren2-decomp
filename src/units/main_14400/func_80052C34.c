#include "common.h"
typedef unsigned char u8;
extern u8 D_8016166D;
typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
/* Complete 0xC-byte sound state at D_80161644 (type +0, playback.handle +4,
 * playback.position +8/+9), the same layout as D_80161650 (func_800528BC
 * clears both records from one base each; func_80051E9C copies D_8016165C whole). */
typedef struct { short type; Playback playback; } State;
extern State D_80161644;
extern short D_8016166A;
extern s32 func_8012A4E4(s32 id);
extern s32 func_80042014(void);
extern void func_80051D14(short id);
void func_80052C34(void) { if (D_8016166D && !func_8012A4E4(D_80161644.playback.handle) && !(u8)func_80042014()) func_80051D14(D_8016166A); }
