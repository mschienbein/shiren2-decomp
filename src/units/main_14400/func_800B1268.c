#include "common.h"

extern void func_800B1080(void);
extern void func_800B1294(void);
extern void func_800AA9FC(s32 value);

void func_800B1268(void) {
    func_800B1080();
    func_800B1294();
    func_800AA9FC(1);
}
