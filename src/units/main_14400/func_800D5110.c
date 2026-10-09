#include "common.h"

typedef unsigned char u8;

typedef struct Object Object;
typedef struct Owner Owner;

/* 20-byte file-scope pool container built by func_800D4F14: pool-record pointer
 * +0 (cleared by func_8013687C), method table +4. */
typedef struct {
    void *pool00;
    void *vtable04;
    u8 pad08[0xC];
} Object800D4F14;

extern Object *func_800D0650(Object *object, Owner *owner);
extern Object800D4F14 *func_800D4F14(Object800D4F14 *obj);

extern Object D_80147FA0;
extern Object D_80147FAC;
extern Owner D_801430B4;
extern Owner D_801430C4;

/* Owned here (zero image, constructed at startup): gas fills the jal slot only for a local symbol. */
Object800D4F14 D_80147FB8 = { 0 };

/* Static constructor (D_80148E70 list): builds three file-scope objects. */
void func_800D5110(void)
{
    func_800D0650(&D_80147FA0, &D_801430B4);
    func_800D0650(&D_80147FAC, &D_801430C4);
    func_800D4F14(&D_80147FB8);
}
