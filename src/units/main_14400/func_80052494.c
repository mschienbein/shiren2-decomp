#include "common.h"
extern u32 func_80052F30(short id);
extern void func_80052B0C(s32 id);
extern void func_80053438(s32 index);
void func_80052494(short id) {
    func_80052B0C(func_80052F30(id));
    func_80053438(id);
}
