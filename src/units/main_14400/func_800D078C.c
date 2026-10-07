#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* Partial object view: word 0 holds the object's current descriptor table. */
typedef struct { void *table; } Obj800D078C;
void *func_800AFB80(void *obj);
u8 func_800AFFD0(void *src_table, void *obj, void *dst_table);
void func_800D0E38(void *object);
void func_800D078C(Obj800D078C *arg0, void *arg1) {
    void *table;
    func_800D0E38(arg0);
    table = func_800AFB80(arg1);
    if (table != arg0->table) {
        func_800AFFD0(table, arg1, arg0->table);
    }
}
