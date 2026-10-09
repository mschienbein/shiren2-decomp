#include "common.h"

typedef struct Obj80120B30 Obj80120B30;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj80120B30 *func_80120B30(Obj80120B30 *obj);

Obj80120B30 *func_80120B68(void) {
    return func_80120B30(func_800AC5B4(0x2C, 0));
}
