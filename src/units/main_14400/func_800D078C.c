#include "common.h"
#include "item_records.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* Partial item-container view: word 0 holds the container's item pool. */
typedef struct { ItemPool *table; } Obj800D078C;
void func_800D0E38(void *object);
void func_800D078C(Obj800D078C *arg0, void *arg1) {
    ItemPool *table;
    func_800D0E38(arg0);
    table = func_800AFB80(arg1);
    if (table != arg0->table) {
        func_800AFFD0(table, arg1, arg0->table);
    }
}
