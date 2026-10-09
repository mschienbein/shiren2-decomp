#include "common.h"
typedef union Pos { s32 word; struct { unsigned short x, y; } p; } Pos;
typedef struct { unsigned char pad0[4]; short state4; unsigned char pad6[8]; short flagE; unsigned char pad10[4]; s32 id14; } Object;
extern void func_8004194C(s32 id, s32 *y, s32 *x);
extern void func_80061820(s32 y, s32 x);
extern void func_80061908(Pos y, Pos x);
extern void func_8005A234(void);
void func_80086A68(Object *object) {
    Pos y, x;
    func_8004194C(object->id14, &y.word, &x.word);
    func_80061820(y.word, x.word);
    func_80061908(y, x);
    func_8005A234();
    object->flagE = 1;
    object->state4 = 4;
}
