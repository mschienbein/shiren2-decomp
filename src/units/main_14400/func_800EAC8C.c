#include "common.h"
typedef struct { char unk0[0x10]; s32 unk10; } State;
extern s32 func_800E5A78(void *, State *);
extern void func_800EACD4(void *);
void func_800EAC8C(void *arg0, State *arg1) {
    func_800E5A78(arg0, arg1);
    if (arg1->unk10 & 8) func_800EACD4(arg0);
}
