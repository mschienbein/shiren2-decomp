#include "common.h"

typedef short s16;
typedef struct { s32 pad0; s16 field_4; } Obj;
extern void func_8005493C(void);
extern s32 D_8013E924;

void func_800846F0(Obj *o) {
    func_8005493C();
    D_8013E924 = 1;
    o->field_4 = 4;
}
