#include "common.h"

/* Whole 0x10-byte list; construction stores the pool/buffer and byte limits,
 * while this destructor replaces the vtable at +4 before clearing its items. */
typedef struct {
    void *pool;
    void *vtable;
    unsigned char *buffer;
    unsigned char capacity;
    unsigned char limit;
    unsigned char count;
    unsigned char padF;
} ItemList;

extern unsigned char D_80154390[];
extern unsigned char D_80154300[];
void func_800CD468(void *list);
void func_800D8FA8(void *obj);

void func_800CE6A0(ItemList *obj, s32 flags)
{
    obj->vtable = D_80154390;
    func_800CD468(obj);
    obj->vtable = D_80154300;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
