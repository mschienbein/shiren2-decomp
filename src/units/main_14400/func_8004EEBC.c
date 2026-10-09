#include "common.h"
typedef struct { s32 first, second; } Pair;
extern s32 D_8013968C;
extern void func_80050E44(s32, Pair *);
extern s32 func_80084AA8(void);
extern void func_80084A9C(s32);
extern void func_8008865C(void *);
extern void func_800850F8(void (*)(void *), unsigned short);
static inline void copy(Pair *dest, const Pair *source) {
    dest->first = source->first;
    dest->second = source->second;
}
void func_8004EEBC(Pair *position, s32 flag) {
    Pair saved;
    s32 sound, previous;
    if (D_8013968C != 0xB7) return;
    copy(&saved, position);
    sound = flag ? 0x146 : 0x147;
    func_80050E44(sound, &saved);
    previous = func_80084AA8();
    if (previous) func_80084A9C(0);
    func_80050E44(sound, &saved);
    func_80050E44(sound, &saved);
    func_80084A9C(previous);
    func_800850F8(func_8008865C, 0x28);
}
