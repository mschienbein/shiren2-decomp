#include "common.h"

typedef short s16;

extern void func_800522FC(s16 arg0);

/* ROM 0x13D734 supplies a word; ROM 0x18CE4/0x18CF0 narrows here. */
void func_80045CA0(s32 arg0) {
    s16 id = arg0;
    func_800522FC(id);
}
