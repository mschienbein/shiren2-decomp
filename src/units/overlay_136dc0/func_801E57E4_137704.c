#include "func_801E57E4_137704/types.h"

void func_80059894(s32 value);
void func_801E5558_137478(s32 arg0);
void func_8005A464(s32 mode, void *params, s32 blend, s32 arg3);
void func_8005ADAC(s32 a, s32 b, s32 c);

void func_801E57E4_137704(s32 arg0) {
    func_80059894(1);
    func_801E5558_137478(0);
    func_8005A464(0, 0, arg0, 1);
    if (arg0 > 0) {
        func_8005ADAC(arg0, 1, 1);
    }
}
