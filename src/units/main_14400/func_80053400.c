#include "common.h"

/* 12-byte fade record: only its address is used here. */
typedef struct { s32 unk0; unsigned char pad4[8]; } Elem;
extern Elem D_801616D0[2];
void func_80053590(Elem *elem);
void func_80053400(void) {
    Elem *elem = D_801616D0;
    func_80053590(elem++);
    func_80053590(elem);
}
