#include "common.h"

/* Complete 0x18-byte embedded owner list; base destruction traverses its buffer. */
typedef struct {
    void *pool;
    void *vtable;
    unsigned char *entries;
    unsigned char capacity, limit, count, pad_0F;
    void *owner;
    unsigned short text_id, pad_16;
} List;
typedef struct {
    char pad00[0xA4];
    List list_A4;
} Obj800FB268;

extern void func_800CE6A0(List *obj, s32 flags);
extern void func_800EFD28(Obj800FB268 *obj, s32 flags);
extern void func_800A3918(Obj800FB268 *obj);

/* Deleting destructor: destroy the member list, run the base destructor, free on bit 0. */
void func_800FB268(Obj800FB268 *obj, s32 flags) {
    func_800CE6A0(&obj->list_A4, 2);
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
