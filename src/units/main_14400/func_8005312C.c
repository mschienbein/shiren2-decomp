#include "common.h"

typedef unsigned char u8;
typedef struct AudioFade AudioFade;
extern AudioFade D_801616E8;
extern void func_800535D0(s32 mode, AudioFade *fade, s32 volume, u8 duration);

void func_8005312C(s32 volume, u8 duration) {
    func_800535D0(0, &D_801616E8, (u8)volume, duration);
}
