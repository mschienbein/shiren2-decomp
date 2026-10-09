#include "common.h"

typedef short s16;
typedef unsigned char u8;
typedef struct Obj Obj;
extern s16 func_800F3E68(Obj *obj, s16 amount, u8 max);
extern void func_800F6EDC(Obj *obj);

void func_800F72BC(Obj *obj, s16 amount) {
    if (func_800F3E68(obj, amount, 2) != 0) {
        func_800F6EDC(obj);
    }
}
