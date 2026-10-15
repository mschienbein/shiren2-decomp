#include "common.h"
#include "collection_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0; u8 kind; } Item800EC7C8;
void func_800D3650(Item800EC7C8 *item);
char *func_800AC990(void *obj);
void *func_800AC5F4(s32 size, Item800EC7C8 *item);
void *func_801198E0(void *obj);
char *func_800EC7C8(u8 *obj) {
    CollectionIterator it;
    Item800EC7C8 *item;
    char *result;
    func_800CEB20(&it, obj + 0xCC);
    while (func_800CEBA0(&it)) {
        item = func_800CEC68(&it);
        if (item->kind == 7) {
            func_800D3650(item);
            result = func_800AC990(item);
            func_801198E0(func_800AC5F4(0xC, item));
            return result;
        }
    }
    return (char *)0;
}
