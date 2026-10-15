#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

typedef struct Object Object;

/* 20-byte file-scope pool container built by func_800D4F14: pool-record pointer
 * +0 (cleared by func_8013687C), method table +4. */
typedef struct {
    void *pool00;
    void *vtable04;
    u8 pad08[0xC];
} Object800D4F14;

/* func_800D0650 stores its second argument, the container's item pool, at +0. */
extern Object *func_800D0650(Object *object, void *descriptor);
extern Object800D4F14 *func_800D4F14(Object800D4F14 *obj);

extern Object D_80147FA0;
extern Object D_80147FAC;

/* Owned here (zero image, constructed at startup): gas fills the jal slot only for a local symbol. */
Object800D4F14 D_80147FB8 = { 0 };

/* Static constructor (D_80148E70 list): builds three file-scope objects. */
void func_800D5110(void)
{
    func_800D0650(&D_80147FA0, &D_801430B4);
    func_800D0650(&D_80147FAC, &D_801430C4);
    func_800D4F14(&D_80147FB8);
}
