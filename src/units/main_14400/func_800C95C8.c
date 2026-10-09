#include "common.h"
extern s32 func_80049CB4(s32, ...);
extern s32 func_800A8FC8(s32 *, s32);
extern void *func_800A910C(s32 *);
static inline s32 *iterator_ref(s32 *iterator) { *iterator = 0; return iterator; }
void func_800C95C8(void *self) {
    s32 iterator;
    s32 *cursor;
    func_80049CB4(0x131);
    cursor = iterator_ref(&iterator);
    for (;;) {
        void *item;
        if (!func_800A8FC8(cursor, 0x7C)) break;
        item = func_800A910C(cursor);
        if (item == self) continue;
        func_80049CB4(6);
        func_80049CB4(0x1F, item);
        func_80049CB4(7);
    }
    func_80049CB4(0x12D);
}
