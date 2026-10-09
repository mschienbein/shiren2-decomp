#include "common.h"
typedef struct Obj800E0F40 Obj800E0F40;
extern s32 func_800E0F40(Obj800E0F40 *obj);
extern void func_800E0508(Obj800E0F40 *obj, s32 arg1, s32 arg2);
void func_800FFD24(Obj800E0F40 *obj) {
    unsigned char value = (unsigned char)func_800E0F40(obj) == 1 ? 2 : 1;
    func_800E0508(obj, 1, value);
}
