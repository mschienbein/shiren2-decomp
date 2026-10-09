#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct {
    u8 pad[0x98];
    s16 delta;
    s16 pad9A;
    void *(*fn)(void *);
} VTable;
typedef struct {
    u8 pad[0x24];
    VTable *vtable;
} Obj;
typedef struct {
    u8 data[0x18];
} Buffer;
typedef struct {
    s32 pos;
    s32 unk4;
} Iter;
extern Obj *D_801476B8;
void func_80136910(Buffer *, void *, u32, u32, u32);
s32 func_800A8FC8(Iter *, s32);
void *func_800A910C(Iter *);
void *func_800F0314(void *);
s32 func_800CD090(void *, void *);
/* Returns zero or func_80121848's forwarded status (0x800F0424/0x800F0428); this
 * statement call intentionally discards it. */
s32 func_800F0404(void *);
void func_800A7B68(void *, Buffer *);

void func_800EFE30(void)
{
    Obj *obj = D_801476B8;
    void *ctx = obj->vtable->fn((u8 *)obj + obj->vtable->delta);
    Buffer buf;
    Iter it;
    void *item;
    void *key;

    func_80136910(&buf, 0, 0, 0, 0);
    it.pos = 0;
    while (func_800A8FC8(&it, 0x10)) {
        item = func_800A910C(&it);
        key = func_800F0314(item);
        if (key != 0 && func_800CD090(ctx, key) != -1) {
            func_800F0404(item);
        } else {
            func_800A7B68(item, &buf);
        }
    }
}
