#include "common.h"

extern void func_8012A2F8(s32 flags, s32 value);
extern void func_800533B8(void);
extern void func_800533DC(void);
extern void func_80053400(void);
extern void func_800535A4(void);
extern void func_80053100(void);

void func_8005348C(void) {
    func_8012A2F8(3, 1);
    func_800533B8();
    func_800533DC();
    func_80053400();
    func_800535A4();
    func_80053100();
}
