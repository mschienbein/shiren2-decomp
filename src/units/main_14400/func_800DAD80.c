#include "common.h"
typedef struct { s32 field00; s32 field04; } Pair;
extern Pair *D_801476B8;
extern s32 D_80148260;
extern s32 func_80049CB4(s32 command, ...);
static inline void copy_pair(s32 *destination, Pair *source) {
    destination[0] = source->field00;
    destination[1] = source->field04;
}
/* Original callers supply their action receiver in a0 (e.g. 0x800DBC6C, 0x800DD7B8);
 * this implementation does not read it. */
void func_800DAD80(void *unused_receiver) {
    s32 pair[2];
    if (D_80148260 != 0) {
        copy_pair(pair, D_801476B8);
        func_80049CB4(0xD7, pair);
        D_80148260 = 0;
    }
}
