#include "common.h"
typedef struct { s32 first, second; } Pair;
extern s32 D_8013968C;
extern void func_80050CEC(s32 code, s32 flags, Pair *first, Pair *second, float volume);
void func_8004FA4C(Pair *first, Pair *second) {
    if (D_8013968C == 0xD4) {
        func_80050CEC(0xA1, 0, first, second, 1.0f);
    }
}
