#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    void *pool_00;
    const void *vtable_04;
    u8 *entries_08;
    u8 state_0C[4];
    void *owner_10;
    u16 text_14;
    u8 pad_16[2];
} List;
extern const u8 D_80154438[];
extern void *func_800CE620(void *, u8 *, u8);
void *func_800CEC90(void *object, void *owner, void *entries, u8 capacity, u16 text) {
    List *list = object;
    func_800CE620(list, entries, capacity);
    list->vtable_04 = D_80154438;
    list->owner_10 = owner;
    list->text_14 = text;
    return list;
}
