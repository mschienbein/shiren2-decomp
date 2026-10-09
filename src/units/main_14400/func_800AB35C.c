#include "common.h"
typedef struct { unsigned char kind; } Item;
extern void func_800AB3F4(void *);
extern void func_800AB548(void *);
extern void func_800AB5B8(void *);
extern void func_800AB6B8(void *);
extern void func_800AB704(void *);
extern void func_800AB750(void *, s32);
void func_800AB35C(Item *item, s32 mode) {
    if (item) {
        switch (item->kind) {
        case 3: case 4: func_800AB3F4(item); break;
        case 6: func_800AB548(item); break;
        case 5: func_800AB5B8(item); break;
        case 11: func_800AB6B8(item); break;
        case 12: func_800AB704(item); break;
        case 17: func_800AB750(item, mode); break;
        }
    }
}
