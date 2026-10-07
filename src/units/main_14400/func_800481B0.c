#include "common.h"
typedef struct { char pad[0x10]; s32 unk10; } Obj;
extern s32 D_80138BF0;
void func_8004633C(void);
void func_80045A24(s32);
void func_800581A4(s32);
void func_80060C54(u32 mode);
void func_800481B0(Obj *self, s32 arg) {
    self->unk10 = arg;
    D_80138BF0 = 1;
    func_8004633C();
    func_80045A24(3);
    func_800581A4(self->unk10);
    func_80060C54(8);
}
