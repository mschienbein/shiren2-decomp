#include "common.h"

typedef struct { s32 unk0; s32 unk4; s32 unk8; } Elem;
extern Elem D_801616D0[2];
void func_80053590(Elem *elem);
void func_80053400(void) {
    Elem *elem = D_801616D0;
    func_80053590(elem++);
    func_80053590(elem);
}
