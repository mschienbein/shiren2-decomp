#include "common.h"
extern s32 func_8012A434(s32 id, s32 arg1);
extern s32 func_8012A4E4(s32 id);
void func_80052B0C(s32 id) { func_8012A434(id, 1); while (func_8012A4E4(id)) { func_8012A434(id, 1); } }
