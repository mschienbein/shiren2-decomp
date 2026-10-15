#include "func_801E6C30_135780/types.h"

extern u32 func_80032D70(const char *text);
extern void func_801E6B1C_13566C(const char *text, s32 length, u16 foreground, u16 background);

void func_801E6C30_135780(const char *text, u16 foreground, u16 background) {
    func_801E6B1C_13566C(text, func_80032D70(text), foreground, background);
}
